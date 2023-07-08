#include <vktest/app_info.h>

namespace vkt {
AppInfo::AppInfo() {
  data.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  data.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  data.pEngineName = "N/A";
  data.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  data.apiVersion = VK_API_VERSION_1_0;
}

AppInfo::AppInfo(VkApplicationInfo const &appInfo) {
  this->data = appInfo;
  setAppName(appInfo.pApplicationName);
}

AppInfo::AppInfo(AppInfo const &other) {
  *this = other;
}

AppInfo &AppInfo::operator=(AppInfo const &other) {
  data = other.data;
  setAppName(other.appName);
  return *this;
}

AppInfo::AppInfo(AppInfo &&other) {
  *this = std::move(other);
}

AppInfo &AppInfo::operator=(AppInfo &&other) {
  data = std::move(other.data);
  other.appName = std::move(other.appName);
  return *this;
}

AppInfo &AppInfo::setAppName(std::string const &appName) {
  this->appName = appName;
  data.pApplicationName = this->appName.c_str();
  return *this;
}
} // namespace vkt