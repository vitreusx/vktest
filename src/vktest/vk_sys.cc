#include <vktest/utils/check.h>
#include <vktest/vk_sys.h>

namespace vkt {
void VkSys::load(VkInstance instance, VkPhysicalDevice physDev, VkDevice dev) {
  gladLoaderLoadVulkan(instance, physDev, dev);
}

std::vector<VkLayerProperties> VkSys::getLayerProps() {
  uint32_t propCount;
  VK_CHECK(vkEnumerateInstanceLayerProperties(&propCount, nullptr));

  std::vector<VkLayerProperties> layerProps(propCount);
  VK_CHECK(vkEnumerateInstanceLayerProperties(&propCount, layerProps.data()));

  return layerProps;
}
} // namespace vkt