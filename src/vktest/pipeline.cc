#include <vktest/pipeline.h>

namespace vkt {
Pipeline::Pipeline(VkPipeline &&data, std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(data);
  this->dev = std::move(dev);
}

Pipeline::~Pipeline() {
  if (data)
    vkDestroyPipeline(*dev, data, nullptr);
}
} // namespace vkt