/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "VulkanSampler.h"

#include <igl/vulkan/Common.h>
#include <igl/vulkan/VulkanContext.h>

namespace lvk {

namespace vulkan {

VulkanSampler::VulkanSampler(const VulkanContext& ctx,
                             VkDevice device,
                             const VkSamplerCreateInfo& ci,
                             const char* debugName) :
  ctx_(ctx), device_(device) {
  IGL_PROFILER_FUNCTION_COLOR(IGL_PROFILER_COLOR_CREATE);

  VK_ASSERT(vkCreateSampler(device_, &ci, nullptr, &vkSampler_));
  VK_ASSERT(
      ivkSetDebugObjectName(device_, VK_OBJECT_TYPE_SAMPLER, (uint64_t)vkSampler_, debugName));
}

VulkanSampler::~VulkanSampler() {
  IGL_PROFILER_FUNCTION_COLOR(IGL_PROFILER_COLOR_DESTROY);

  ctx_.deferredTask(std::packaged_task<void()>(
      [device = device_, sampler = vkSampler_]() { vkDestroySampler(device, sampler, nullptr); }));
}

} // namespace vulkan

} // namespace lvk
