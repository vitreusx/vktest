#pragma once
#include <memory>
#include <optional>
#include <stdexcept>
#include <vktest/builder.h>
#include <vktest/device.h>
#include <vktest/render_pass.h>
#include <vktest/utils/check.h>
#include <vktest/utils/enumerate.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class RenderSubpass : public Wrapper<VkSubpassDescription> {
public:
  RenderSubpass() = default;

  RenderSubpass(RenderSubpass const &other);
  RenderSubpass &operator=(RenderSubpass const &other);

  RenderSubpass &
  setColorAttachments(std::vector<VkAttachmentReference> attachments);

  RenderSubpass &
  setInputAttachments(std::vector<VkAttachmentReference> attachments);

  RenderSubpass &
  setDepthStencilAttachment(std::optional<VkAttachmentReference> attachment);

  RenderSubpass &
  setResolveAttachments(std::vector<VkAttachmentReference> attachments);

  RenderSubpass &setPreserveAttachments(std::vector<uint32_t> attachments);

private:
  std::vector<VkAttachmentReference> color, resolve, input;
  std::vector<uint32_t> preserve;
  std::optional<VkAttachmentReference> depthStencil;
};

class RenderPassBuilder : public Builder<VkRenderPassCreateInfo> {
public:
  RenderPassBuilder();

  RenderPassBuilder &
  setAttachments(std::vector<VkAttachmentDescription> attachments);

  RenderPassBuilder &setSubpasses(std::vector<RenderSubpass> subpasses);

  RenderPassBuilder &
  setDependencies(std::vector<VkSubpassDependency> dependencies);

  RenderPass build(std::shared_ptr<Device> dev) const;

private:
  std::vector<VkAttachmentDescription> attachments;
  std::vector<VkSubpassDependency> dependencies;
  std::vector<RenderSubpass> subpasses;
  std::vector<VkSubpassDescription> rawSubpasses;
};
} // namespace vkt