#include <vktest/semaphore.h>

namespace vkt {
Semaphore::Semaphore(std::shared_ptr<vkt::Device> dev) {
  this->dev = std::move(dev);

  VkSemaphoreCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  VK_CHECK(vkCreateSemaphore(*this->dev, &info, nullptr, &data));
}

Semaphore::~Semaphore() {
  if (data)
    vkDestroySemaphore(*dev, data, nullptr);
}
} // namespace vkt