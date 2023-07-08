#pragma once
#include <vector>
#include <vktest/builder.h>
#include <vktest/framebuffer.h>
#include <vktest/render_pass.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>

namespace vkt {
class FramebufferBuilder : public Builder<VkFramebufferCreateInfo> {
public:
  FramebufferBuilder();

  FramebufferBuilder &attachImageViews(std::vector<VkImageView> imageViews);

  FramebufferBuilder &setExtent(VkExtent2D ext);

  FramebufferBuilder &setRenderPass(RenderPass &renderPass);

  Framebuffer build(std::shared_ptr<Device> dev) const;

private:
  std::vector<VkImageView> imageViews;
  RenderPass *renderPass;
};
} // namespace vkt