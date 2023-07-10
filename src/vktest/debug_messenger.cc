#include <vktest/debug_messenger.h>
#include <vktest/instance.h>

namespace vkt {
DebugMessenger::DebugMessenger(VkDebugUtilsMessengerEXT &&debugMsgr,
                               std::shared_ptr<vkt::Instance> inst,
                               Callback<DebugCbSig> cb) {
  data = std::move(debugMsgr);
  this->inst = std::move(inst);
  this->cb = std::move(cb);
}

DebugMessenger::~DebugMessenger() {
  if (data)
    vkDestroyDebugUtilsMessengerEXT(*inst, data, nullptr);
}
} // namespace vkt