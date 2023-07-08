#include <vktest/swapchain_builder.h>
#include <vktest/utils/check.h>

namespace vkt {
SwapchainBuilder::SwapchainBuilder() {
  info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  info.oldSwapchain = VK_NULL_HANDLE;
}

SwapchainBuilder &SwapchainBuilder::setQueueFamilyIndices(
    std::vector<uint32_t> const &queueFamilyIndices) {
  this->queueFamilyIndices = queueFamilyIndices;
  info.queueFamilyIndexCount = (uint32_t)this->queueFamilyIndices.size();
  info.pQueueFamilyIndices = this->queueFamilyIndices.data();
  return *this;
}

Swapchain SwapchainBuilder::build(std::shared_ptr<vkt::Device> dev) {
  VkSwapchainKHR swapchain;
  VK_CHECK(vkCreateSwapchainKHR(*dev, &info, nullptr, &swapchain));
  return Swapchain(std::move(swapchain), std::move(dev));
}
} // namespace vkt