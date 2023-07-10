#include <vktest/instance.h>
#include <vktest/utils/check.h>

namespace vkt {
Instance::Instance(VkInstance &&instance,
                   std::optional<Callback<DebugCbSig>> debugCb) {
  data = std::move(instance);
  this->debugCb = std::move(debugCb);
}

Instance::~Instance() {
  if (data)
    vkDestroyInstance(data, nullptr);
}

std::vector<vkt::PhysicalDevice> Instance::getPhysicalDevices() const {
  uint32_t devCount = 0;
  VK_CHECK(vkEnumeratePhysicalDevices(data, &devCount, VK_NULL_HANDLE));

  std::vector<VkPhysicalDevice> rawDevices(devCount);
  VK_CHECK(vkEnumeratePhysicalDevices(data, &devCount, rawDevices.data()));

  std::vector<vkt::PhysicalDevice> devices;
  devices.reserve(devCount);
  for (auto &device : rawDevices)
    devices.emplace_back(std::move(device));

  return devices;
}
} // namespace vkt