#include <vktest/framebuffer_builder.h>

namespace vkt {
FramebufferBuilder::FramebufferBuilder() {
  info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  info.layers = 1;
}

FramebufferBuilder &
FramebufferBuilder::attachImageViews(std::vector<VkImageView> imageViews) {
  this->imageViews = std::move(imageViews);
  info.attachmentCount = (uint32_t)this->imageViews.size();
  info.pAttachments = this->imageViews.data();
  return *this;
}

FramebufferBuilder &FramebufferBuilder::setExtent(VkExtent2D ext) {
  info.width = ext.width;
  info.height = ext.height;
  return *this;
}

FramebufferBuilder &FramebufferBuilder::setRenderPass(RenderPass &renderPass) {
  this->renderPass = &renderPass;
  info.renderPass = *this->renderPass;
  return *this;
}

Framebuffer FramebufferBuilder::build(std::shared_ptr<Device> dev) const {
  VkFramebuffer fb;
  VK_CHECK(vkCreateFramebuffer(*dev, &info, nullptr, &fb));
  return Framebuffer(std::move(fb), std::move(dev));
}
} // namespace vkt