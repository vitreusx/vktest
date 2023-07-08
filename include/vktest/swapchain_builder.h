#pragma once
#include <vector>
#include <vktest/builder.h>
#include <vktest/swapchain.h>
#include <vktest/vulkan.h>

namespace vkt {
class SwapchainBuilder : public Builder<VkSwapchainCreateInfoKHR> {
public:
  SwapchainBuilder();

  SwapchainBuilder &
  setQueueFamilyIndices(std::vector<uint32_t> const &queueFamilyIndices);

  Swapchain build(std::shared_ptr<vkt::Device> dev);

private:
  std::vector<uint32_t> queueFamilyIndices;
};
} // namespace vkt