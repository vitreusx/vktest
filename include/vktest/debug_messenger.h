#pragma once
#include <memory>
#include <vktest/cb.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Instance;

using DebugCbSig = VkBool32 (*)(VkDebugUtilsMessageSeverityFlagBitsEXT,
                                VkDebugUtilsMessageTypeFlagsEXT,
                                const VkDebugUtilsMessengerCallbackDataEXT *);

class DebugMessenger : public Wrapper<VkDebugUtilsMessengerEXT> {
public:
  DebugMessenger() = default;
  DebugMessenger(VkDebugUtilsMessengerEXT &&debugMsgr,
                 std::shared_ptr<vkt::Instance> inst, Callback<DebugCbSig> cb);
  ~DebugMessenger();

  DebugMessenger(DebugMessenger &&) = default;
  DebugMessenger &operator=(DebugMessenger &&) = default;

private:
  std::shared_ptr<vkt::Instance> inst;
  Callback<DebugCbSig> cb;
};
} // namespace vkt