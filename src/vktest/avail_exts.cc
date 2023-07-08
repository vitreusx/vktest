#include <vktest/avail_exts.h>
#include <vktest/utils/check.h>

namespace vkt {
AvailExts::AvailExts() {
  uint32_t extCount;
  VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr));

  exts = std::vector<VkExtensionProperties>(extCount);
  VK_CHECK(
      vkEnumerateInstanceExtensionProperties(nullptr, &extCount, exts.data()));
}

bool AvailExts::isSupported(std::string const &extName) const {
  for (auto const &ext : exts) {
    if (ext.extensionName == extName)
      return true;
  }
  return false;
}
} // namespace vkt