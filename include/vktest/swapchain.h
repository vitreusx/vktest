#pragma once
#include <vktest/device.h>
#include <vktest/utils/swap_shared_ptr.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>


namespace vkt {
class Swapchain : public Wrapper<VkSwapchainKHR> {
public:
  Swapchain() = default;
  Swapchain(VkSwapchainKHR &&handle, std::shared_ptr<vkt::Device> dev);
  ~Swapchain();

  Swapchain(Swapchain &&) = default;
  Swapchain &operator=(Swapchain &&) = default;

  std::vector<VkImage> getImages();

private:
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt