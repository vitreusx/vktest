#include <vktest/framebuffer.h>

namespace vkt {
Framebuffer::Framebuffer(VkFramebuffer &&data,
                         std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(data);
  this->dev = std::move(dev);
}

Framebuffer::~Framebuffer() {
  if (data)
    vkDestroyFramebuffer(*dev, data, nullptr);
}
} // namespace vkt