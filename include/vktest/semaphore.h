#pragma once
#include <memory>
#include <vktest/device.h>
#include <vktest/utils/check.h>
#include <vktest/vulkan.h>
#include <vktest/wrapper.h>

namespace vkt {
class Semaphore : public Wrapper<VkSemaphore> {
public:
  Semaphore() = default;
  Semaphore(std::shared_ptr<vkt::Device> dev);
  ~Semaphore();

  Semaphore(Semaphore &&) = default;
  Semaphore &operator=(Semaphore &&) = default;

private:
  std::shared_ptr<vkt::Device> dev;
};
} // namespace vkt