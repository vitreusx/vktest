#include <vktest/avail_layers.h>
#include <vktest/utils/check.h>

namespace vkt {
AvailLayers::AvailLayers() {
  uint32_t layerCount = 0;
  VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

  layers = std::vector<VkLayerProperties>(layerCount);
  VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, layers.data()));
}

bool AvailLayers::isSupported(std::string const &layerName) const {
  for (auto const &layer : layers) {
    if (layer.layerName == layerName)
      return true;
  }
  return false;
}
} // namespace vkt