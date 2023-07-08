#pragma once
#include <string>
#include <vector>
#include <vktest/builder.h>
#include <vktest/pipeline.h>
#include <vktest/pipeline_layout.h>
#include <vktest/render_pass.h>
#include <vktest/shader_module.h>
#include <vktest/utils/check.h>
#include <vktest/utils/enumerate.h>
#include <vktest/vulkan.h>

namespace vkt {
class ShaderStage : public Wrapper<VkPipelineShaderStageCreateInfo> {
public:
  ShaderStage(std::shared_ptr<vkt::ShaderModule> module,
              VkShaderStageFlagBits stage, std::string entrypoint);

  ShaderStage(ShaderStage const &other);
  ShaderStage &operator=(ShaderStage const &other);

  ShaderStage(ShaderStage &&) = default;
  ShaderStage &operator=(ShaderStage &&) = default;

private:
  std::shared_ptr<vkt::ShaderModule> module;
  std::string entrypoint;
};

class PipelineBuilder : public Builder<VkGraphicsPipelineCreateInfo> {
public:
  PipelineBuilder();

  PipelineBuilder &setShaderStages(std::vector<ShaderStage> shaderStages);

  PipelineBuilder &setDynamicStates(std::vector<VkDynamicState> dynamicStates);

  PipelineBuilder &setViewports(std::vector<VkViewport> viewports);

  PipelineBuilder &setScissors(std::vector<VkRect2D> scissors);

  PipelineBuilder &setColorBlendAttachments(
      std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments);

  PipelineBuilder &setLayout(PipelineLayout &layout);

  PipelineBuilder &setRenderPass(RenderPass &renderPass, uint32_t subpass);

  Pipeline build(std::shared_ptr<Device> dev) const;

public:
  VkPipelineRasterizationStateCreateInfo rasterizerInfo;
  VkPipelineMultisampleStateCreateInfo multisamplingInfo;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;

private:
  std::vector<ShaderStage> shaderStages;
  std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;

  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  std::vector<VkDynamicState> dynamicStates;

  VkPipelineVertexInputStateCreateInfo vertexInputInfo;

  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;

  VkPipelineViewportStateCreateInfo viewportInfo;
  std::vector<VkViewport> viewports;
  std::vector<VkRect2D> scissors;

  std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

  PipelineLayout *layout;
  RenderPass *renderPass;
};
} // namespace vkt