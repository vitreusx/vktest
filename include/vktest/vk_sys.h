#pragma once
#include <vector>
#include <vktest/vulkan.h>

namespace vkt {
class VkSys {
public:
  static void load(VkInstance instance = nullptr,
                   VkPhysicalDevice physDev = nullptr, VkDevice dev = nullptr);
  static std::vector<VkLayerProperties> getLayerProps();
};
} // namespace vkt