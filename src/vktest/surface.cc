#include <vktest/surface.h>
#include <vktest/utils/check.h>

namespace vkt {
Surface::Surface(VkSurfaceKHR &&surface,
                 std::shared_ptr<vkt::Instance> instance) {
  data = std::move(surface);
  this->instance = std::move(instance);
}

Surface::~Surface() {
  if (data)
    vkDestroySurfaceKHR(*this->instance, data, nullptr);
}

SurfaceDetails Surface::getDetails(VkPhysicalDevice &physDev) {
  SurfaceDetails details;

  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDev, data,
                                                     &details.capabilities));

  uint32_t formatCount;
  VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physDev, data, &formatCount,
                                                nullptr));

  if (formatCount > 0) {
    details.formats.resize(formatCount);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physDev, data, &formatCount,
                                                  details.formats.data()));
  }

  uint32_t modeCount;
  VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physDev, data, &modeCount,
                                                     nullptr));

  if (modeCount > 0) {
    details.presentModes.resize(modeCount);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
        physDev, data, &modeCount, details.presentModes.data()));
  }

  return details;
}
} // namespace vkt