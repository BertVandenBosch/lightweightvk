/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "VulkanImageView.h"

#include <igl/vulkan/Common.h>
#include <igl/vulkan/VulkanContext.h>

namespace lvk {

namespace vulkan {

VulkanImageView::VulkanImageView(const VulkanContext& ctx,
                                 VkDevice device,
                                 VkImage image,
                                 VkImageViewType type,
                                 VkFormat format,
                                 VkImageAspectFlags aspectMask,
                                 uint32_t baseLevel,
                                 uint32_t numLevels,
                                 uint32_t baseLayer,
                                 uint32_t numLayers,
                                 const char* debugName) :
  ctx_(ctx), device_(device) {
  IGL_PROFILER_FUNCTION_COLOR(IGL_PROFILER_COLOR_CREATE);

  VK_ASSERT(ivkCreateImageView(
      device_, image, type, format, VkImageSubresourceRange{aspectMask, baseLevel, numLevels, baseLayer, numLayers}, &vkImageView_));

  VK_ASSERT(ivkSetDebugObjectName(device_, VK_OBJECT_TYPE_IMAGE_VIEW, (uint64_t)vkImageView_, debugName));
}

VulkanImageView::~VulkanImageView() {
  IGL_PROFILER_FUNCTION_COLOR(IGL_PROFILER_COLOR_DESTROY);

  ctx_.deferredTask(
      std::packaged_task<void()>([device = device_, imageView = vkImageView_]() { vkDestroyImageView(device, imageView, nullptr); }));
}

} // namespace vulkan

} // namespace lvk
