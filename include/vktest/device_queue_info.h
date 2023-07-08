#pragma once
#include <vector>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class DeviceQueueInfo : public Wrapper<VkDeviceQueueCreateInfo> {
public:
  DeviceQueueInfo();

  DeviceQueueInfo(DeviceQueueInfo const &);
  DeviceQueueInfo &operator=(DeviceQueueInfo const &);

  DeviceQueueInfo(DeviceQueueInfo &&) = default;
  DeviceQueueInfo &operator=(DeviceQueueInfo &&) = default;

  DeviceQueueInfo &setFamilyIndex(uint32_t queueFamilyIndex);
  DeviceQueueInfo &setCount(uint32_t queueCount);
  DeviceQueueInfo &setPriorities(std::vector<float> const &queuePriorities);

private:
  std::vector<float> priorities;
};
} // namespace vkt