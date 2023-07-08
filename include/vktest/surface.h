#pragma once
#include <memory>
#include <vector>
#include <vktest/instance.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
struct SurfaceDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Surface : public Wrapper<VkSurfaceKHR> {
public:
  Surface(VkSurfaceKHR &&surface, std::shared_ptr<vkt::Instance> instance);
  ~Surface();

  Surface(Surface &&) = default;
  Surface &operator=(Surface &&) = default;

  SurfaceDetails getDetails(VkPhysicalDevice &physDev);

private:
  std::shared_ptr<vkt::Instance> instance;
};
} // namespace vkt