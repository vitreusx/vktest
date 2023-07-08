#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <vktest/builder.h>
#include <vktest/device.h>
#include <vktest/device_queue_info.h>
#include <vktest/utils/enumerate.h>
#include <vktest/vulkan.h>

namespace vkt {
class DeviceBuilder : public Builder<VkDeviceCreateInfo> {
public:
  DeviceBuilder();

  DeviceBuilder &setLayers(std::vector<std::string> const &layers);
  DeviceBuilder &setExts(std::vector<std::string> const &exts);
  DeviceBuilder &
  setQueueInfos(std::vector<vkt::DeviceQueueInfo> const &queueInfos);

  vkt::Device build(VkPhysicalDevice &physDev) const;

private:
  std::vector<std::string> layers;
  std::vector<char const *> layerPointers;

  std::vector<std::string> exts;
  std::vector<char const *> extPointers;

  std::vector<vkt::DeviceQueueInfo> queueInfos;
  std::vector<VkDeviceQueueCreateInfo> rawQueueInfos;
};
} // namespace vkt