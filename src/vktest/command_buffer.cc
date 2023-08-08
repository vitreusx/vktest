#include <vktest/command_buffer.h>

namespace vkt {
RenderPassBeginOp::RenderPassBeginOp(VkCommandBuffer cmdBuf) {
  data = {};
  data.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  this->cmdBuf = std::move(cmdBuf);
}

RenderPassBeginOp &
RenderPassBeginOp::setClearValues(std::vector<VkClearValue> clearValues) {
  this->clearValues = std::move(clearValues);
  data.clearValueCount = (uint32_t)this->clearValues.size();
  data.pClearValues = this->clearValues.data();
  return *this;
}

void RenderPassBeginOp::operator()() const {
  vkCmdBeginRenderPass(cmdBuf, &data, subpassContents);
}

CommandBuffer::CommandBuffer() : beginRenderPass{data} {}

CommandBuffer::CommandBuffer(VkCommandBuffer &&data) : beginRenderPass{data} {
  this->data = std::move(data);
}

void CommandBuffer::startRecording() {
  VkCommandBufferBeginInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  VK_CHECK(vkBeginCommandBuffer(data, &info));
}

void CommandBuffer::endRenderPass() {
  vkCmdEndRenderPass(data);
}
} // namespace vkt