#pragma once
#include <vector>
#include <vktest/app_info.h>
#include <vktest/builder.h>
#include <vktest/instance.h>
#include <vktest/vulkan.h>

namespace vkt {
class InstanceBuilder : public Builder<VkInstanceCreateInfo> {
public:
  InstanceBuilder();
  InstanceBuilder(VkInstanceCreateInfo const &info);

  InstanceBuilder(InstanceBuilder &other);
  InstanceBuilder &operator=(InstanceBuilder const &other);

  InstanceBuilder(InstanceBuilder &&) = default;
  InstanceBuilder &operator=(InstanceBuilder &&) = default;

  InstanceBuilder &setAppInfo(AppInfo const &appInfo);
  InstanceBuilder &setFlags(VkInstanceCreateFlags const &flags);
  InstanceBuilder &setLayers(std::vector<std::string> const &layers);
  InstanceBuilder &setExtensions(std::vector<std::string> const &extensions);

  Instance build() const;

private:
  vkt::AppInfo appInfo;

  std::vector<std::string> layers;
  std::vector<char const *> layerPointers;

  std::vector<std::string> exts;
  std::vector<char const *> extPointers;
};
} // namespace vkt