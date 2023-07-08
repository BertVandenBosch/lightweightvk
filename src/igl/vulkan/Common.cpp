/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Common.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>

#if defined(VK_USE_PLATFORM_WIN32_KHR)
#include <windows.h>
#endif

#include <igl/vulkan/VulkanHelpers.h>

namespace igl {
namespace vulkan {

Result getResultFromVkResult(VkResult result) {
  if (result == VK_SUCCESS) {
    return Result();
  }

  Result res(Result::Code::RuntimeError, ivkGetVulkanResultString(result));

  switch (result) {
  case VK_ERROR_LAYER_NOT_PRESENT:
  case VK_ERROR_EXTENSION_NOT_PRESENT:
  case VK_ERROR_FEATURE_NOT_PRESENT:
    res.code = Result::Code::Unimplemented;
    return res;
  case VK_ERROR_INCOMPATIBLE_DRIVER:
  case VK_ERROR_FORMAT_NOT_SUPPORTED:
    res.code = Result::Code::Unsupported;
    return res;
  case VK_ERROR_OUT_OF_HOST_MEMORY:
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
  case VK_ERROR_OUT_OF_POOL_MEMORY:
  case VK_ERROR_TOO_MANY_OBJECTS:
    res.code = Result::Code::ArgumentOutOfRange;
    return res;
  default:;
    // skip other Vulkan error codes
  }
  return res;
}

void setResultFrom(Result* outResult, VkResult result) {
  if (!outResult) {
    return;
  }

  *outResult = getResultFromVkResult(result);
}

VkFormat textureFormatToVkFormat(igl::TextureFormat format) {
  using TextureFormat = ::igl::TextureFormat;
  switch (format) {
  case TextureFormat::Invalid:
    return VK_FORMAT_UNDEFINED;
  case TextureFormat::A_UNorm8:
    return VK_FORMAT_UNDEFINED;
  case TextureFormat::R_UNorm8:
    return VK_FORMAT_R8_UNORM;
  case TextureFormat::R_UNorm16:
    return VK_FORMAT_R16_UNORM;
  case TextureFormat::R_F16:
    return VK_FORMAT_R16_SFLOAT;
  case TextureFormat::R_UInt16:
    return VK_FORMAT_R16_UINT;
  case TextureFormat::B5G5R5A1_UNorm:
    return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
  case TextureFormat::B5G6R5_UNorm:
    return VK_FORMAT_B5G6R5_UNORM_PACK16;
  case TextureFormat::ABGR_UNorm4:
    return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
  case TextureFormat::RG_UNorm8:
    return VK_FORMAT_R8G8_UNORM;
  case TextureFormat::RG_UNorm16:
    return VK_FORMAT_R16G16_UNORM;
  case TextureFormat::BGRA_UNorm8:
    return VK_FORMAT_B8G8R8A8_UNORM;
  case TextureFormat::BGRA_UNorm8_Rev:
    return VK_FORMAT_UNDEFINED;
  case TextureFormat::RGBA_UNorm8:
    return VK_FORMAT_R8G8B8A8_UNORM;
  case TextureFormat::RGBA_SRGB:
    return VK_FORMAT_R8G8B8A8_SRGB;
  case TextureFormat::BGRA_SRGB:
    return VK_FORMAT_B8G8R8A8_SRGB;
  case TextureFormat::RG_F16:
    return VK_FORMAT_R16G16_SFLOAT;
  case TextureFormat::RG_UInt16:
    return VK_FORMAT_R16G16_UINT;
  case TextureFormat::RGB10_A2_UNorm_Rev:
    return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
  case TextureFormat::RGB10_A2_Uint_Rev:
    return VK_FORMAT_A2R10G10B10_UINT_PACK32;
  case TextureFormat::BGR10_A2_Unorm:
    return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
  case TextureFormat::R_F32:
    return VK_FORMAT_R32_SFLOAT;
  case TextureFormat::RGBA_F16:
    return VK_FORMAT_R16G16B16A16_SFLOAT;
  case TextureFormat::RGBA_UInt32:
    return VK_FORMAT_R32G32B32A32_UINT;
  case TextureFormat::RGBA_F32:
    return VK_FORMAT_R32G32B32A32_SFLOAT;
  case TextureFormat::RGB8_ETC1:
    return VK_FORMAT_UNDEFINED;
  case TextureFormat::RGB8_ETC2:
    return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
  case TextureFormat::SRGB8_ETC2:
    return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
  case TextureFormat::RGB8_Punchthrough_A1_ETC2:
    return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
  case TextureFormat::SRGB8_Punchthrough_A1_ETC2:
    return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
  case TextureFormat::RGBA8_EAC_ETC2:
    return VK_FORMAT_UNDEFINED;
  case TextureFormat::SRGB8_A8_EAC_ETC2:
    return VK_FORMAT_UNDEFINED;
  case TextureFormat::RG_EAC_UNorm:
    return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
  case TextureFormat::RG_EAC_SNorm:
    return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
  case TextureFormat::R_EAC_UNorm:
    return VK_FORMAT_EAC_R11_UNORM_BLOCK;
  case TextureFormat::R_EAC_SNorm:
    return VK_FORMAT_EAC_R11_SNORM_BLOCK;
  case TextureFormat::RGBA_BC7_UNORM_4x4:
    return VK_FORMAT_BC7_UNORM_BLOCK;
  case TextureFormat::Z_UNorm16:
    return VK_FORMAT_D16_UNORM;
  case TextureFormat::Z_UNorm24:
    return VK_FORMAT_D24_UNORM_S8_UINT;
  case TextureFormat::Z_UNorm32:
    return VK_FORMAT_D32_SFLOAT;
  case TextureFormat::S8_UInt_Z24_UNorm:
    return VK_FORMAT_D24_UNORM_S8_UINT;
  default:
    IGL_ASSERT_MSG(false, "TextureFormat value not handled: %d", (int)format);
  }
  IGL_UNREACHABLE_RETURN(VK_FORMAT_UNDEFINED);
}

igl::ColorSpace vkColorSpaceToColorSpace(VkColorSpaceKHR colorSpace) {
  switch (colorSpace) {
  case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
    return ColorSpace::SRGB_NONLINEAR;
  default:
    IGL_ASSERT_NOT_REACHED();
    return ColorSpace::SRGB_NONLINEAR;
  }
}

igl::TextureFormat vkFormatToTextureFormat(VkFormat format) {
  using TextureFormat = ::igl::TextureFormat;
  switch (format) {
  case VK_FORMAT_UNDEFINED:
    return TextureFormat::Invalid;
  case VK_FORMAT_R8_UNORM:
    return TextureFormat::R_UNorm8;
  case VK_FORMAT_R16_UNORM:
    return TextureFormat::R_UNorm16;
  case VK_FORMAT_R16_SFLOAT:
    return TextureFormat::R_F16;
  case VK_FORMAT_R16_UINT:
    return TextureFormat::R_UInt16;
  case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    return TextureFormat::B5G5R5A1_UNorm;
  case VK_FORMAT_B5G6R5_UNORM_PACK16:
    return TextureFormat::B5G6R5_UNorm;
  case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
    return TextureFormat::ABGR_UNorm4;
  case VK_FORMAT_R8G8_UNORM:
    return TextureFormat::RG_UNorm8;
  case VK_FORMAT_B8G8R8A8_UNORM:
    return TextureFormat::BGRA_UNorm8;
  case VK_FORMAT_R8G8B8A8_UNORM:
    return TextureFormat::RGBA_UNorm8;
  case VK_FORMAT_R8G8B8A8_SRGB:
    return TextureFormat::RGBA_SRGB;
  case VK_FORMAT_B8G8R8A8_SRGB:
    return TextureFormat::BGRA_SRGB;
  case VK_FORMAT_R16G16_UNORM:
    return TextureFormat::RG_UNorm16;
  case VK_FORMAT_R16G16_SFLOAT:
    return TextureFormat::RG_F16;
  case VK_FORMAT_R16G16_UINT:
    return TextureFormat::RG_UInt16;
  case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
    return TextureFormat::RGB10_A2_UNorm_Rev;
  case VK_FORMAT_A2R10G10B10_UINT_PACK32:
    return TextureFormat::RGB10_A2_Uint_Rev;
  case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    return TextureFormat::BGR10_A2_Unorm;
  case VK_FORMAT_R32_SFLOAT:
    return TextureFormat::R_F32;
  case VK_FORMAT_R16G16B16A16_SFLOAT:
    return TextureFormat::RGBA_F16;
  case VK_FORMAT_R32G32B32A32_UINT:
    return TextureFormat::RGBA_UInt32;
  case VK_FORMAT_R32G32B32A32_SFLOAT:
    return TextureFormat::RGBA_F32;
  case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
  case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
    return TextureFormat::RGB8_ETC2;
  case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
    return TextureFormat::SRGB8_ETC2;
  case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    return TextureFormat::RGB8_Punchthrough_A1_ETC2;
  case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
    return TextureFormat::SRGB8_Punchthrough_A1_ETC2;
  case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
    return TextureFormat::RG_EAC_UNorm;
  case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
    return TextureFormat::RG_EAC_SNorm;
  case VK_FORMAT_EAC_R11_UNORM_BLOCK:
    return TextureFormat::R_EAC_UNorm;
  case VK_FORMAT_EAC_R11_SNORM_BLOCK:
    return TextureFormat::R_EAC_SNorm;
  case VK_FORMAT_D16_UNORM:
    return TextureFormat::Z_UNorm16;
  case VK_FORMAT_BC7_UNORM_BLOCK:
    return TextureFormat::RGBA_BC7_UNORM_4x4;
  case VK_FORMAT_X8_D24_UNORM_PACK32:
    return TextureFormat::Z_UNorm24;
  case VK_FORMAT_D24_UNORM_S8_UINT:
    return TextureFormat::S8_UInt_Z24_UNorm;
  case VK_FORMAT_D32_SFLOAT:
    return TextureFormat::Z_UNorm32;
  default:
    IGL_ASSERT_MSG(false, "VkFormat value not handled: %d", (int)format);
  }

  return TextureFormat::Invalid;
}

uint32_t getBytesPerPixel(VkFormat format) {
  switch (format) {
  case VK_FORMAT_R8_UNORM:
    return 1;
  case VK_FORMAT_R16_SFLOAT:
    return 2;
  case VK_FORMAT_R8G8B8_UNORM:
  case VK_FORMAT_B8G8R8_UNORM:
    return 3;
  case VK_FORMAT_R8G8B8A8_UNORM:
  case VK_FORMAT_B8G8R8A8_UNORM:
  case VK_FORMAT_R8G8B8A8_SRGB:
  case VK_FORMAT_R16G16_SFLOAT:
  case VK_FORMAT_R32_SFLOAT:
    return 4;
  case VK_FORMAT_R16G16B16_SFLOAT:
    return 6;
  case VK_FORMAT_R16G16B16A16_SFLOAT:
  case VK_FORMAT_R32G32_SFLOAT:
    return 8;
  case VK_FORMAT_R32G32B32_SFLOAT:
    return 12;
  case VK_FORMAT_R32G32B32A32_SFLOAT:
    return 16;
  default:
    IGL_ASSERT_MSG(false, "VkFormat value not handled: %d", (int)format);
  }

  return 1;
}

VkMemoryPropertyFlags resourceStorageToVkMemoryPropertyFlags(igl::ResourceStorage resourceStorage) {
  VkMemoryPropertyFlags memFlags{0};

  switch (resourceStorage) {
  case ResourceStorage::Private:
    memFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    break;
  case ResourceStorage::Shared:
    memFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    break;
  case ResourceStorage::Memoryless:
    memFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
    break;
  }

  return memFlags;
}

VkCompareOp compareOpToVkCompareOp(igl::CompareOp func) {
  switch (func) {
  case igl::CompareOp_Never:
    return VK_COMPARE_OP_NEVER;
  case igl::CompareOp_Less:
    return VK_COMPARE_OP_LESS;
  case igl::CompareOp_Equal:
    return VK_COMPARE_OP_EQUAL;
  case igl::CompareOp_LessEqual:
    return VK_COMPARE_OP_LESS_OR_EQUAL;
  case igl::CompareOp_Greater:
    return VK_COMPARE_OP_GREATER;
  case igl::CompareOp_NotEqual:
    return VK_COMPARE_OP_NOT_EQUAL;
  case igl::CompareOp_GreaterEqual:
    return VK_COMPARE_OP_GREATER_OR_EQUAL;
  case igl::CompareOp_AlwaysPass:
    return VK_COMPARE_OP_ALWAYS;
  }
  IGL_ASSERT_MSG(false, "CompareFunction value not handled: %d", (int)func);
  return VK_COMPARE_OP_ALWAYS;
}

VkSampleCountFlagBits getVulkanSampleCountFlags(size_t numSamples) {
  if (numSamples <= 1) {
    return VK_SAMPLE_COUNT_1_BIT;
  }
  if (numSamples <= 2) {
    return VK_SAMPLE_COUNT_2_BIT;
  }
  if (numSamples <= 4) {
    return VK_SAMPLE_COUNT_4_BIT;
  }
  if (numSamples <= 8) {
    return VK_SAMPLE_COUNT_8_BIT;
  }
  if (numSamples <= 16) {
    return VK_SAMPLE_COUNT_16_BIT;
  }
  if (numSamples <= 32) {
    return VK_SAMPLE_COUNT_32_BIT;
  }
  return VK_SAMPLE_COUNT_64_BIT;
}

VkSurfaceFormatKHR colorSpaceToVkSurfaceFormat(igl::ColorSpace colorSpace, bool isBGR) {
  switch (colorSpace) {
  case igl::ColorSpace::SRGB_LINEAR:
    // the closest thing to sRGB linear
    return VkSurfaceFormatKHR{isBGR ? VK_FORMAT_B8G8R8A8_UNORM : VK_FORMAT_R8G8B8A8_UNORM,
                              VK_COLOR_SPACE_BT709_LINEAR_EXT};
  case igl::ColorSpace::SRGB_NONLINEAR:
    [[fallthrough]];
  default:
    // default to normal sRGB non linear.
    return VkSurfaceFormatKHR{isBGR ? VK_FORMAT_B8G8R8A8_SRGB : VK_FORMAT_R8G8B8A8_SRGB,
                              VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }
}

} // namespace vulkan
} // namespace igl
