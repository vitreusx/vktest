#include <vktest/debug_messenger.h>

namespace vkt {
DebugMessenger::DebugMessenger(VkDebugUtilsMessengerEXT &&debugMsgr,
                               std::shared_ptr<vkt::Instance> inst) {
  data = std::move(debugMsgr);
  this->inst = std::move(inst);
}

DebugMessenger::~DebugMessenger() {
  if (data)
    vkDestroyDebugUtilsMessengerEXT(*inst, data, nullptr);
}
} // namespace vkt