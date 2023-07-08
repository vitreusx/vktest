#pragma once
#include <vector>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class PhysicalDevice : public Wrapper<VkPhysicalDevice> {
public:
  PhysicalDevice(VkPhysicalDevice &&device);

public:
  VkPhysicalDeviceProperties deviceProps;
  VkPhysicalDeviceFeatures features;
  std::vector<VkQueueFamilyProperties> queueFamilyProps;
};
} // namespace vkt