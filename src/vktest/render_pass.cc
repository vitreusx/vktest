#include <vktest/render_pass.h>

namespace vkt {
RenderPass::RenderPass(VkRenderPass &&data, std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(data);
  this->dev = std::move(dev);
}

RenderPass::~RenderPass() {
  if (data)
    vkDestroyRenderPass(*dev, data, nullptr);
}
} // namespace vkt