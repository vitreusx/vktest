#include <vktest/pipeline_layout.h>

namespace vkt {
PipelineLayout::PipelineLayout(VkPipelineLayout &&data,
                               std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(data);
  this->dev = std::move(dev);
}

PipelineLayout::~PipelineLayout() {
  if (data)
    vkDestroyPipelineLayout(*dev, data, nullptr);
}
} // namespace vkt