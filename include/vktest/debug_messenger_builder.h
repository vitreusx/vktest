#pragma once
#include <vktest/builder.h>
#include <vktest/cb.h>
#include <vktest/debug_messenger.h>
#include <vktest/instance.h>
#include <vktest/vulkan.h>

namespace vkt {
class DebugMessengerBuilder
    : public Builder<VkDebugUtilsMessengerCreateInfoEXT> {
public:
  DebugMessengerBuilder();

  DebugMessengerBuilder(DebugMessengerBuilder const &other);
  DebugMessengerBuilder &operator=(DebugMessengerBuilder const &other);

  DebugMessengerBuilder(DebugMessengerBuilder &&other);

  DebugMessengerBuilder &operator=(DebugMessengerBuilder &&other);

  DebugMessengerBuilder &
  setSeverity(VkDebugUtilsMessageSeverityFlagsEXT severity);

  DebugMessengerBuilder &setMessageType(VkDebugUtilsMessageTypeFlagsEXT type);

  DebugMessenger build(std::shared_ptr<vkt::Instance> instance) const;

  using DebugCbSig = VkBool32 (*)(VkDebugUtilsMessageSeverityFlagBitsEXT,
                                  VkDebugUtilsMessageTypeFlagsEXT,
                                  const VkDebugUtilsMessengerCallbackDataEXT *);
  Callback<DebugCbSig> cb;

private:
  static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCb(
      VkDebugUtilsMessageSeverityFlagBitsEXT severity,
      VkDebugUtilsMessageTypeFlagsEXT type,
      const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_data);
};
} // namespace vkt