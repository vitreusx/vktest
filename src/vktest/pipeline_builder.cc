#include <vktest/pipeline_builder.h>

namespace vkt {
ShaderStage::ShaderStage(std::shared_ptr<vkt::ShaderModule> module,
                         VkShaderStageFlagBits stage, std::string entrypoint) {
  this->module = std::move(module);
  this->entrypoint = std::move(entrypoint);

  data.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  data.stage = stage;
  data.module = *this->module;
  data.pName = this->entrypoint.c_str();
}

ShaderStage::ShaderStage(ShaderStage const &other)
    : ShaderStage(other.module, other.data.stage, other.entrypoint) {}

ShaderStage &ShaderStage::operator=(ShaderStage const &other) {
  if (this != &other) {
    module = other.module;
    entrypoint = other.entrypoint;
    data = other.data;
    data.module = *this->module;
    data.pName = entrypoint.c_str();
  }
  return *this;
}

PipelineBuilder::PipelineBuilder() {
  info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  dynamicStateInfo = {};
  dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  info.pDynamicState = &dynamicStateInfo;

  vertexInputInfo = {};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  info.pVertexInputState = &vertexInputInfo;

  inputAssemblyInfo = {};
  inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
  info.pInputAssemblyState = &inputAssemblyInfo;

  viewportInfo = {};
  viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  info.pViewportState = &viewportInfo;

  rasterizerInfo = {};
  rasterizerInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  info.pRasterizationState = &rasterizerInfo;

  multisamplingInfo = {};
  multisamplingInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  info.pMultisampleState = &multisamplingInfo;

  colorBlendInfo = {};
  colorBlendInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  info.pColorBlendState = &colorBlendInfo;

  info.basePipelineHandle = VK_NULL_HANDLE;
  info.basePipelineIndex = -1;
}

PipelineBuilder &
PipelineBuilder::setShaderStages(std::vector<ShaderStage> shaderStages) {
  this->shaderStages = std::move(shaderStages);
  shaderStageInfos.resize(this->shaderStages.size());
  for (auto const &[idx, stage] : enumerate(this->shaderStages))
    shaderStageInfos[idx] = stage;

  info.stageCount = (uint32_t)shaderStageInfos.size();
  info.pStages = shaderStageInfos.data();
  return *this;
}

PipelineBuilder &
PipelineBuilder::setDynamicStates(std::vector<VkDynamicState> dynamicStates) {
  this->dynamicStates = std::move(dynamicStates);
  dynamicStateInfo.dynamicStateCount = (uint32_t)this->dynamicStates.size();
  dynamicStateInfo.pDynamicStates = this->dynamicStates.data();
  for (auto &dynamicState : this->dynamicStates) {
    switch (dynamicState) {
    case VK_DYNAMIC_STATE_SCISSOR:
      viewportInfo.scissorCount = 1;
      viewportInfo.pScissors = VK_NULL_HANDLE;
      break;
    case VK_DYNAMIC_STATE_VIEWPORT:
      viewportInfo.viewportCount = 1;
      viewportInfo.pViewports = VK_NULL_HANDLE;
      break;
    default:
      break;
    }
  }
  return *this;
}

PipelineBuilder &
PipelineBuilder::setViewports(std::vector<VkViewport> viewports) {
  this->viewports = std::move(viewports);
  viewportInfo.viewportCount = (uint32_t)this->viewports.size();
  viewportInfo.pViewports = this->viewports.data();
  return *this;
}

PipelineBuilder &PipelineBuilder::setScissors(std::vector<VkRect2D> scissors) {
  this->scissors = std::move(scissors);
  viewportInfo.scissorCount = (uint32_t)this->scissors.size();
  viewportInfo.pScissors = this->scissors.data();
  return *this;
}

PipelineBuilder &PipelineBuilder::setColorBlendAttachments(
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments) {
  this->colorBlendAttachments = colorBlendAttachments;
  colorBlendInfo.attachmentCount = (uint32_t)this->colorBlendAttachments.size();
  colorBlendInfo.pAttachments = this->colorBlendAttachments.data();
  return *this;
}

PipelineBuilder &PipelineBuilder::setLayout(PipelineLayout &layout) {
  this->layout = &layout;
  info.layout = *this->layout;
  return *this;
}

PipelineBuilder &PipelineBuilder::setRenderPass(RenderPass &renderPass,
                                                uint32_t subpass) {
  this->renderPass = &renderPass;
  info.renderPass = *this->renderPass;
  info.subpass = subpass;
  return *this;
}

Pipeline PipelineBuilder::build(std::shared_ptr<Device> dev) const {
  VkPipeline pipeline;
  VK_CHECK(
      vkCreateGraphicsPipelines(*dev, nullptr, 1, &info, nullptr, &pipeline));
  return Pipeline(std::move(pipeline), std::move(dev));
}
} // namespace vkt