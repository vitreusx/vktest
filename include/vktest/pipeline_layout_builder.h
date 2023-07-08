#pragma once
#include <memory>
#include <vktest/builder.h>
#include <vktest/device.h>
#include <vktest/pipeline_layout.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>

namespace vkt {
class PipelineLayoutBuilder : public Builder<VkPipelineLayoutCreateInfo> {
public:
  PipelineLayoutBuilder();
  PipelineLayout build(std::shared_ptr<Device> dev) const;
};
} // namespace vkt