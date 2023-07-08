#include <vktest/device_queue_info.h>

namespace vkt {
DeviceQueueInfo::DeviceQueueInfo() {
  data.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
}

DeviceQueueInfo::DeviceQueueInfo(DeviceQueueInfo const &other) {
  data = other.data;
  setPriorities(other.priorities);
}

DeviceQueueInfo &DeviceQueueInfo::operator=(DeviceQueueInfo const &other) {
  if (this != &other) {
    data = other.data;
    setPriorities(other.priorities);
  }
  return *this;
}

DeviceQueueInfo &DeviceQueueInfo::setFamilyIndex(uint32_t familyIndex) {
  data.queueFamilyIndex = familyIndex;
  return *this;
}

DeviceQueueInfo &DeviceQueueInfo::setCount(uint32_t count) {
  setPriorities(std::vector<float>(count, 1.0));
  return *this;
}

DeviceQueueInfo &
DeviceQueueInfo::setPriorities(std::vector<float> const &priorities) {
  this->priorities = priorities;
  data.queueCount = (uint32_t)this->priorities.size();
  data.pQueuePriorities = this->priorities.data();
  return *this;
}
} // namespace vkt