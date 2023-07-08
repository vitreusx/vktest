#pragma once
#include <memory>
#include <vktest/device.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Swapchain : public Wrapper<VkSwapchainKHR> {
public:
  Swapchain(VkSwapchainKHR &&handle, std::shared_ptr<vkt::Device> dev);
  ~Swapchain();

  Swapchain(Swapchain &&) = default;
  Swapchain &operator=(Swapchain &&) = default;

  std::vector<VkImage> getImages();

private:
  std::shared_ptr<vkt::Device> dev;
};
} // namespace vkt