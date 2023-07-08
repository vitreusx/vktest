#include <vktest/glfw_sys.h>

namespace vkt {
static std::mutex mut;
static std::shared_ptr<GLFWSys> instance = nullptr;

std::shared_ptr<GLFWSys> GLFWSys::getInstance() {
  std::lock_guard<std::mutex> guard(mut);
  if (!instance)
    instance = std::shared_ptr<GLFWSys>(new GLFWSys());
  return instance;
}

static void errorCb(int errorCode, char const *description) {
  GLFWSys::getInstance()->onError(errorCode, description);
}

GLFWSys::GLFWSys() {
  if (glfwInit() != GLFW_TRUE)
    throw std::runtime_error("glfwInit");

  glfwSetErrorCallback(vkt::errorCb);
}

GLFWSys::~GLFWSys() {
  glfwTerminate();
}

std::vector<std::string> GLFWSys::getVkInstanceExtensions() const {
  uint32_t numExts;
  auto **extsPointer = glfwGetRequiredInstanceExtensions(&numExts);
  std::vector<std::string> exts(extsPointer, extsPointer + numExts);
  return exts;
}
} // namespace vkt
