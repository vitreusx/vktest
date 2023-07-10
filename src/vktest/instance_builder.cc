#include <vktest/instance.h>
#include <vktest/instance_builder.h>
#include <vktest/utils/check.h>

namespace vkt {
InstanceBuilder::InstanceBuilder() {
  info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
}

InstanceBuilder::InstanceBuilder(VkInstanceCreateInfo const &info) {
  this->info = info;
  setAppInfo(*info.pApplicationInfo);

  layers = std::vector<std::string>(info.enabledLayerCount);
  for (size_t layerIdx = 0; layerIdx < layers.size(); ++layerIdx)
    layers[layerIdx] = info.ppEnabledLayerNames[layerIdx];
  setLayers(layers);

  exts = std::vector<std::string>(info.enabledExtensionCount);
  for (size_t extIdx = 0; extIdx < exts.size(); ++extIdx)
    exts[extIdx] = info.ppEnabledExtensionNames[extIdx];
  setExtensions(exts);
}

InstanceBuilder::InstanceBuilder(InstanceBuilder &other) {
  *this = other;
}

InstanceBuilder &InstanceBuilder::operator=(InstanceBuilder const &other) {
  if (this != &other) {
    info = other.info;
    setAppInfo(other.appInfo);
    setLayers(other.layers);
    setExtensions(other.exts);
  }
  return *this;
}

InstanceBuilder &InstanceBuilder::setAppInfo(vkt::AppInfo const &appInfo) {
  this->appInfo = appInfo;
  info.pApplicationInfo = &(VkApplicationInfo const &)this->appInfo;
  return *this;
}

InstanceBuilder &InstanceBuilder::setFlags(VkInstanceCreateFlags const &flags) {
  info.flags = flags;
  return *this;
}

InstanceBuilder &
InstanceBuilder::setLayers(std::vector<std::string> const &layers) {
  this->layers = layers;

  layerPointers = std::vector<char const *>(layers.size());
  for (size_t layerIdx = 0; layerIdx < layers.size(); ++layerIdx) {
    layerPointers[layerIdx] = layers[layerIdx].c_str();
  }

  info.enabledLayerCount = (uint32_t)layers.size();
  info.ppEnabledLayerNames = layerPointers.data();

  return *this;
}

InstanceBuilder &
InstanceBuilder::setExtensions(std::vector<std::string> const &extensions) {
  this->exts = extensions;

  extPointers = std::vector<char const *>(exts.size());
  for (size_t extIdx = 0; extIdx < exts.size(); ++extIdx) {
    extPointers[extIdx] = exts[extIdx].c_str();
  }

  info.enabledExtensionCount = (uint32_t)exts.size();
  info.ppEnabledExtensionNames = extPointers.data();

  return *this;
}

InstanceBuilder &
InstanceBuilder::enableDebugger(vkt::DebugMessengerBuilder debuggerBuilder) {
  this->debuggerBuilder = std::move(debuggerBuilder);
  info.pNext = &(VkDebugUtilsMessengerCreateInfoEXT &)this->debuggerBuilder;
  return *this;
}

Instance InstanceBuilder::build() const {
  VkInstance instance;
  auto result = vkCreateInstance(&info, nullptr, &instance);
  if (result != VK_SUCCESS)
    throw std::runtime_error("vkCreateInstance");
  return Instance(std::move(instance), std::move(debuggerBuilder.cb));
}
} // namespace vkt