/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <igl/vulkan/Common.h>
#include <igl/vulkan/VulkanCommandPool.h>
#include <igl/vulkan/VulkanHelpers.h>

namespace igl {
namespace vulkan {

class VulkanImmediateCommands final {
 public:
  // the maximum number of command buffers which can similtaneously exist in the system; when we run
  // out of buffers, we stall and wait until an existing buffer becomes available
  static constexpr uint32_t kMaxCommandBuffers = 64;

  VulkanImmediateCommands(VkDevice device, uint32_t queueFamilyIndex, const char* debugName);
  ~VulkanImmediateCommands();
  VulkanImmediateCommands(const VulkanImmediateCommands&) = delete;
  VulkanImmediateCommands& operator=(const VulkanImmediateCommands&) = delete;

  struct SubmitHandle {
    uint32_t bufferIndex_ = 0;
    uint32_t submitId_ = 0;
    SubmitHandle() = default;
    explicit SubmitHandle(uint64_t handle) :
      bufferIndex_(uint32_t(handle & 0xffffffff)), submitId_(uint32_t(handle >> 32)) {
      IGL_ASSERT(submitId_);
    }
    bool empty() const {
      return submitId_ == 0;
    }
    uint64_t handle() const {
      return (uint64_t(submitId_) << 32) + bufferIndex_;
    }
  };

  static_assert(sizeof(SubmitHandle) == sizeof(uint64_t));

  struct CommandBufferWrapper {
    VkCommandBuffer cmdBuf_ = VK_NULL_HANDLE;
    VkCommandBuffer cmdBufAllocated_ = VK_NULL_HANDLE;
    SubmitHandle handle_ = {};
    VkFence fence_ = VK_NULL_HANDLE;
    VkSemaphore semaphore_ = VK_NULL_HANDLE;
    bool isEncoding_ = false;
  };

  // returns the current command buffer (creates one if it does not exist)
  const CommandBufferWrapper& acquire();
  SubmitHandle submit(const CommandBufferWrapper& wrapper);
  void waitSemaphore(VkSemaphore semaphore);
  VkSemaphore acquireLastSubmitSemaphore();
  SubmitHandle getLastSubmitHandle() const;
  bool isReady(SubmitHandle handle, bool fastCheckNoVulkan = false) const;
  void wait(SubmitHandle handle);
  void waitAll();

 private:
  void purge();

 private:
  VkDevice device_ = VK_NULL_HANDLE;
  VkQueue queue_ = VK_NULL_HANDLE;
  VulkanCommandPool commandPool_;
  const char* debugName_ = "";
  CommandBufferWrapper buffers_[kMaxCommandBuffers];
  SubmitHandle lastSubmitHandle_ = SubmitHandle();
  VkSemaphore lastSubmitSemaphore_ = VK_NULL_HANDLE;
  VkSemaphore waitSemaphore_ = VK_NULL_HANDLE;
  uint32_t numAvailableCommandBuffers_ = kMaxCommandBuffers;
  uint32_t submitCounter_ = 1;
};

} // namespace vulkan
} // namespace igl
