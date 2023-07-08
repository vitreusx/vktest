#include <vktest/phys_dev.h>
#include <vktest/utils/check.h>

namespace vkt {
PhysicalDevice::PhysicalDevice(VkPhysicalDevice &&device) {
  data = std::move(device);
  vkGetPhysicalDeviceProperties(data, &deviceProps);
  vkGetPhysicalDeviceFeatures(data, &features);

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(data, &queueFamilyCount,
                                           VK_NULL_HANDLE);
  queueFamilyProps.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(data, &queueFamilyCount,
                                           queueFamilyProps.data());
}
} // namespace vkt