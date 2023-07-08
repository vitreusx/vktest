#include <algorithm>
#include <stdexcept>
#include <vktest/avail_device_exts.h>
#include <vktest/utils/check.h>

namespace vkt {
AvailDeviceExts::AvailDeviceExts(VkPhysicalDevice const &physDev) {
  uint32_t extCount = 0;
  VK_CHECK(vkEnumerateDeviceExtensionProperties(physDev, nullptr, &extCount,
                                                nullptr));

  exts.resize(extCount);
  VK_CHECK(vkEnumerateDeviceExtensionProperties(physDev, nullptr, &extCount,
                                                exts.data()));
}

bool AvailDeviceExts::isSupported(std::string const &extName) const {
  for (auto ext : exts)
    if (ext.extensionName == extName)
      return true;
  return false;
}
} // namespace vkt