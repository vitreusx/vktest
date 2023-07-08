#pragma once
#include <string>
#include <vector>
#include <vktest/vulkan.h>

namespace vkt {
class AvailExts {
public:
  AvailExts();

  bool isSupported(std::string const &extName) const;

private:
  std::vector<VkExtensionProperties> exts;
};
} // namespace vkt