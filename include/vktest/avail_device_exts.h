#pragma once
#include <vector>
#include <vktest/vulkan.h>

namespace vkt {
class AvailDeviceExts {
public:
  AvailDeviceExts(VkPhysicalDevice const &physDev);

  bool isSupported(std::string const &extName) const;

private:
  std::vector<VkExtensionProperties> exts;
};
} // namespace vkt