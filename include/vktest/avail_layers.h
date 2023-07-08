#pragma once
#include <string>
#include <vector>
#include <vktest/vulkan.h>

namespace vkt {
class AvailLayers {
public:
  AvailLayers();

  bool isSupported(std::string const &layerName) const;

private:
  std::vector<VkLayerProperties> layers;
};
} // namespace vkt