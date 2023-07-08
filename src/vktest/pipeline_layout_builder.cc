#include <vktest/pipeline_layout_builder.h>

namespace vkt {
PipelineLayoutBuilder::PipelineLayoutBuilder() {
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
}

PipelineLayout PipelineLayoutBuilder::build(std::shared_ptr<Device> dev) const {
  VkPipelineLayout layout;
  VK_CHECK(vkCreatePipelineLayout(*dev, &info, nullptr, &layout));
  return PipelineLayout(std::move(layout), std::move(dev));
}
} // namespace vkt