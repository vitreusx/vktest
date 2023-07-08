#pragma once
#include <memory>
#include <vktest/instance.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class DebugMessenger : public Wrapper<VkDebugUtilsMessengerEXT> {
public:
  DebugMessenger(VkDebugUtilsMessengerEXT &&debugMsgr,
                 std::shared_ptr<vkt::Instance> inst);
  ~DebugMessenger();

  DebugMessenger(DebugMessenger &&) = default;
  DebugMessenger &operator=(DebugMessenger &&) = default;

private:
  std::shared_ptr<vkt::Instance> inst;
};
} // namespace vkt