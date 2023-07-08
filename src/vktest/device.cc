#include <vktest/device.h>

namespace vkt {
Device::Device(VkDevice &&dev) {
  this->data = std::move(dev);
}

VkQueue Device::getQueue(uint32_t familyIndex, uint32_t queueIndex) const {
  VkQueue queue = {};
  vkGetDeviceQueue(this->data, familyIndex, queueIndex, &queue);
  return queue;
}

Device::~Device() {
  if (data)
    vkDestroyDevice(data, nullptr);
}
} // namespace vkt