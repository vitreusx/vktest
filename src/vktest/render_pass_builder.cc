#include <vktest/render_pass_builder.h>

namespace vkt {
RenderSubpass::RenderSubpass(RenderSubpass const &other) {
  *this = other;
}

RenderSubpass &RenderSubpass::operator=(RenderSubpass const &other) {
  if (this != &other) {
    this->data = other.data;
    setColorAttachments(other.color);
    setInputAttachments(other.input);
    setDepthStencilAttachment(other.depthStencil);
    setResolveAttachments(other.resolve);
  }
  return *this;
}

RenderSubpass &RenderSubpass::setColorAttachments(
    std::vector<VkAttachmentReference> attachments) {
  color = std::move(attachments);
  data.colorAttachmentCount = (uint32_t)color.size();
  data.pColorAttachments = color.data();
  return *this;
}

RenderSubpass &RenderSubpass::setInputAttachments(
    std::vector<VkAttachmentReference> attachments) {
  input = std::move(attachments);
  data.inputAttachmentCount = (uint32_t)input.size();
  data.pInputAttachments = input.data();
  return *this;
}

RenderSubpass &RenderSubpass::setDepthStencilAttachment(
    std::optional<VkAttachmentReference> attachment) {
  depthStencil = std::move(attachment);
  if (depthStencil.has_value())
    data.pDepthStencilAttachment = &depthStencil.value();
  else
    data.pDepthStencilAttachment = VK_NULL_HANDLE;
  return *this;
}

RenderSubpass &RenderSubpass::setResolveAttachments(
    std::vector<VkAttachmentReference> attachments) {
  resolve = std::move(attachments);
  if (!resolve.empty() && (resolve.size() != color.size()))
    throw std::invalid_argument(
        "# of resolve attachments must equal # of color attachments");

  data.pResolveAttachments = resolve.data();
  return *this;
}

RenderSubpass &
RenderSubpass::setPreserveAttachments(std::vector<uint32_t> attachments) {
  preserve = std::move(attachments);
  data.preserveAttachmentCount = (uint32_t)preserve.size();
  data.pPreserveAttachments = preserve.data();
  return *this;
}

RenderPassBuilder::RenderPassBuilder() {
  info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
}

RenderPassBuilder &RenderPassBuilder::setAttachments(
    std::vector<VkAttachmentDescription> attachments) {
  this->attachments = std::move(attachments);
  info.attachmentCount = (uint32_t)this->attachments.size();
  info.pAttachments = this->attachments.data();
  return *this;
}

RenderPassBuilder &
RenderPassBuilder::setSubpasses(std::vector<RenderSubpass> subpasses) {
  this->subpasses = std::move(subpasses);
  rawSubpasses.resize(this->subpasses.size());
  for (auto const &[idx, subpass] : enumerate(this->subpasses))
    rawSubpasses[idx] = this->subpasses[idx];

  info.subpassCount = (uint32_t)rawSubpasses.size();
  info.pSubpasses = rawSubpasses.data();
  return *this;
}

RenderPassBuilder &RenderPassBuilder::setDependencies(
    std::vector<VkSubpassDependency> dependencies) {
  this->dependencies = std::move(dependencies);
  info.dependencyCount = (uint32_t)this->dependencies.size();
  info.pDependencies = this->dependencies.data();
  return *this;
}

RenderPass RenderPassBuilder::build(std::shared_ptr<Device> dev) const {
  VkRenderPass renderPass;
  VK_CHECK(vkCreateRenderPass(*dev, &info, nullptr, &renderPass));
  return RenderPass(std::move(renderPass), std::move(dev));
}
} // namespace vkt