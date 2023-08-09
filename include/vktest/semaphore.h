#pragma once
#include <vktest/device.h>
#include <vktest/utils/check.h>
#include <vktest/utils/swap_shared_ptr.h>
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
  vkt::swap_shared_ptr<vkt::Device> dev;
};
} // namespace vkt