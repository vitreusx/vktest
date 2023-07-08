#pragma once
#include <string>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class AppInfo : public Wrapper<VkApplicationInfo> {
public:
  AppInfo();
  AppInfo(VkApplicationInfo const &appInfo);

  AppInfo(AppInfo const &other);
  AppInfo &operator=(AppInfo const &other);

  AppInfo(AppInfo &&other);
  AppInfo &operator=(AppInfo &&other);

  AppInfo &setAppName(std::string const &appName);

private:
  std::string appName;
};
} // namespace vkt