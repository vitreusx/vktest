#include <vktest/command_pool_builder.h>

namespace vkt {
CommandPoolBuilder::CommandPoolBuilder() {
  info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
}

CommandPool CommandPoolBuilder::build(std::shared_ptr<Device> dev) {
  VkCommandPool pool;
  VK_CHECK(vkCreateCommandPool(*dev, &info, nullptr, &pool));
  return CommandPool(std::move(pool), std::move(dev));
}
} // namespace vkt