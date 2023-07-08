#include <vktest/command_pool.h>

namespace vkt {
CommandPool::CommandPool(VkCommandPool &&data,
                         std::shared_ptr<vkt::Device> dev) {
  this->data = std::move(data);
  this->dev = std::move(dev);
}

CommandPool::~CommandPool() {
  if (data)
    vkDestroyCommandPool(*dev, data, nullptr);
}

std::vector<CommandBuffer>
CommandPool::allocateBuffers(uint32_t count, VkCommandBufferLevel level) {
  VkCommandBufferAllocateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  info.commandPool = data;
  info.level = level;
  info.commandBufferCount = count;

  std::vector<VkCommandBuffer> rawBuffers(count);
  VK_CHECK(vkAllocateCommandBuffers(*dev, &info, rawBuffers.data()));

  std::vector<CommandBuffer> buffers;
  for (auto &&vkBuffer : rawBuffers)
    buffers.emplace_back(std::move(vkBuffer));

  return buffers;
}

CommandBuffer CommandPool::allocateBuffer(VkCommandBufferLevel level) {
  auto buffers = allocateBuffers(1, level);
  return std::move(buffers[0]);
}
} // namespace vkt