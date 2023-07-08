#include <vktest/device_builder.h>
#include <vktest/utils/check.h>

namespace vkt {
DeviceBuilder::DeviceBuilder() {
  info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
}

DeviceBuilder &
DeviceBuilder::setLayers(std::vector<std::string> const &layers) {
  this->layers = layers;
  layerPointers.resize(this->layers.size());
  for (auto const &[idx, layer] : enumerate(this->layers))
    layerPointers[idx] = layer.c_str();
  info.enabledLayerCount = (uint32_t)layerPointers.size();
  info.ppEnabledLayerNames = layerPointers.data();
  return *this;
}

DeviceBuilder &DeviceBuilder::setExts(std::vector<std::string> const &exts) {
  this->exts = exts;
  extPointers.resize(this->exts.size());
  for (auto const &[idx, ext] : enumerate(this->exts))
    extPointers[idx] = ext.c_str();
  info.enabledExtensionCount = (uint32_t)extPointers.size();
  info.ppEnabledExtensionNames = extPointers.data();
  return *this;
}

DeviceBuilder &DeviceBuilder::setQueueInfos(
    std::vector<vkt::DeviceQueueInfo> const &queueInfos) {
  this->queueInfos = queueInfos;

  rawQueueInfos.resize(queueInfos.size());
  for (auto const &[idx, dq_info] : enumerate(this->queueInfos))
    rawQueueInfos[idx] = dq_info;

  info.queueCreateInfoCount = (uint32_t)rawQueueInfos.size();
  info.pQueueCreateInfos = rawQueueInfos.data();
  return *this;
}

vkt::Device DeviceBuilder::build(VkPhysicalDevice &physDev) const {
  VkDevice device = {};
  auto result = vkCreateDevice(physDev, &info, nullptr, &device);
  if (result != VK_SUCCESS)
    throw std::runtime_error("vkCreateDevice");
  return vkt::Device(std::move(device));
}
} // namespace vkt