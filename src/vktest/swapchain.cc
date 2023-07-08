#include <vktest/swapchain.h>
#include <vktest/utils/check.h>

namespace vkt {
Swapchain::Swapchain(VkSwapchainKHR &&swapchain,
                     std::shared_ptr<vkt::Device> dev) {
  data = std::move(swapchain);
  this->dev = std::move(dev);
}

std::vector<VkImage> Swapchain::getImages() {
  uint32_t imageCount;
  VK_CHECK(vkGetSwapchainImagesKHR(*dev, data, &imageCount, nullptr));

  std::vector<VkImage> images(imageCount);
  VK_CHECK(vkGetSwapchainImagesKHR(*dev, data, &imageCount, images.data()));

  return images;
}

Swapchain::~Swapchain() {
  if (data)
    vkDestroySwapchainKHR(*dev, data, nullptr);
}
} // namespace vkt