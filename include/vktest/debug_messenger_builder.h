#pragma once
#include <vktest/builder.h>
#include <vktest/cb.h>
#include <vktest/debug_messenger.h>
#include <vktest/vulkan.h>

namespace vkt {
class Instance;
class InstanceBuilder;

class DebugMessengerBuilder
    : public Builder<VkDebugUtilsMessengerCreateInfoEXT> {
public:
  DebugMessengerBuilder();

  DebugMessengerBuilder(DebugMessengerBuilder const &other) = default;
  DebugMessengerBuilder &
  operator=(DebugMessengerBuilder const &other) = default;

  DebugMessengerBuilder(DebugMessengerBuilder &&other) = default;
  DebugMessengerBuilder &operator=(DebugMessengerBuilder &&other) = default;

  DebugMessengerBuilder &
  setSeverity(VkDebugUtilsMessageSeverityFlagsEXT severity);

  DebugMessengerBuilder &setMessageType(VkDebugUtilsMessageTypeFlagsEXT type);

  template <typename F>
  DebugMessengerBuilder &setCallback(F func) {
    cb = std::move(func);
    info.pUserData = &cb.func();
    return *this;
  }

  DebugMessenger build(std::shared_ptr<vkt::Instance> instance) const;

private:
  static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCb(
      VkDebugUtilsMessageSeverityFlagBitsEXT severity,
      VkDebugUtilsMessageTypeFlagsEXT type,
      const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_data);

  friend class InstanceBuilder;
  Callback<DebugCbSig> cb;
};
} // namespace vkt