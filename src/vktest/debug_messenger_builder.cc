#include <vktest/debug_messenger_builder.h>
#include <vktest/utils/check.h>

namespace vkt {
DebugMessengerBuilder::DebugMessengerBuilder() {
  info = {};
  info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  info.pUserData = this;
  info.pfnUserCallback = _debugCb;
}

DebugMessengerBuilder::DebugMessengerBuilder(
    DebugMessengerBuilder const &other) {
  info = {};
  *this = other;
}

DebugMessengerBuilder &
DebugMessengerBuilder::operator=(DebugMessengerBuilder const &other) {
  if (this == &other)
    return *this;
  info = other.info;
  info.pUserData = this;
  cb = other.cb;
  return *this;
}

DebugMessengerBuilder::DebugMessengerBuilder(DebugMessengerBuilder &&other) {
  info = {};
  *this = std::move(other);
}

DebugMessengerBuilder &
DebugMessengerBuilder::operator=(DebugMessengerBuilder &&other) {
  if (this == &other)
    return *this;
  info = std::move(other.info);
  info.pUserData = this;
  cb = std::move(other.cb);
  return *this;
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
  return DebugMessenger(std::move(debugMsgr), std::move(instance));
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerBuilder::_debugCb(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_data) {
  auto *messenger = reinterpret_cast<DebugMessengerBuilder *>(user_data);
  return messenger->cb(severity, type, cb_data);
}
} // namespace vkt