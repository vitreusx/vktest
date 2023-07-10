#include <vktest/debug_messenger_builder.h>
#include <vktest/instance.h>
#include <vktest/utils/check.h>

namespace vkt {
DebugMessengerBuilder::DebugMessengerBuilder() {
  info = {};
  info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  info.pfnUserCallback = _debugCb;
}

DebugMessengerBuilder &DebugMessengerBuilder::setSeverity(
    VkDebugUtilsMessageSeverityFlagsEXT severity) {
  info.messageSeverity = severity;
  return *this;
}

DebugMessengerBuilder &
DebugMessengerBuilder::setMessageType(VkDebugUtilsMessageTypeFlagsEXT type) {
  info.messageType = type;
  return *this;
}

DebugMessenger
DebugMessengerBuilder::build(std::shared_ptr<vkt::Instance> instance) const {
  VkDebugUtilsMessengerEXT debugMsgr;
  VK_CHECK(
      vkCreateDebugUtilsMessengerEXT(*instance, &info, nullptr, &debugMsgr));
  return DebugMessenger(std::move(debugMsgr), std::move(instance),
                        std::move(cb));
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerBuilder::_debugCb(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_data) {
  using VirtCbType =
      decltype(&std::declval<DebugMessengerBuilder &>().cb.func());
  auto *cb = reinterpret_cast<VirtCbType>(user_data);
  return cb->impl(severity, type, cb_data);
}
} // namespace vkt