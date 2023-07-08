#include <vktest/fence.h>

namespace vkt {
Fence::Fence(std::shared_ptr<vkt::Device> dev, VkFenceCreateFlags flags) {
  this->dev = std::move(dev);

  VkFenceCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  info.flags = flags;
  VK_CHECK(vkCreateFence(*this->dev, &info, nullptr, &data));
}

Fence::~Fence() {
  if (data)
    vkDestroyFence(*dev, data, nullptr);
}
} // namespace vkt