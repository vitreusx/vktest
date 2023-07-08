#pragma once
#include <memory>
#include <vktest/device.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class PipelineLayout : public Wrapper<VkPipelineLayout> {
public:
  PipelineLayout(VkPipelineLayout &&data, std::shared_ptr<vkt::Device> dev);
  ~PipelineLayout();

  PipelineLayout(PipelineLayout &&) = default;
  PipelineLayout &operator=(PipelineLayout &&) = default;

private:
  std::shared_ptr<vkt::Device> dev;
};
} // namespace vkt