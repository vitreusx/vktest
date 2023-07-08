#include <vktest/glfw_window.h>

namespace vkt {
template <std::size_t I, typename... Args>
struct GLFWWindow::_MakeCb<I, void (*)(GLFWwindow *, Args...)> {
  using Sig = void (*)(GLFWwindow *, Args...);
  operator Sig() const {
    return GLFWWindow::callback<I, Args...>;
  }
};

template <std::size_t I, typename... Args>
void GLFWWindow::callback(GLFWwindow *handle, Args... args) {
  void *user_ptr = glfwGetWindowUserPointer(handle);
  auto *window = reinterpret_cast<GLFWWindow *>(user_ptr);
  auto &func = window->on->get<I>();
  func(std::forward<Args>(args)...);
}

GLFWWindow::GLFWWindow(GLFWSys const &, int width, int height,
                       std::string const &title, int clientApi) {
  glfwWindowHint(GLFW_CLIENT_API, clientApi);

  data = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!data)
    throw std::runtime_error("glfwCreateWindow");

  on = std::make_shared<GLFWCallbacks>();
  glfwSetWindowUserPointer(data, this);

  glfwSetKeyCallback(data, _MakeCb<0, GLFWkeyfun>{});
  glfwSetWindowSizeCallback(data, _MakeCb<1, GLFWwindowsizefun>{});
}

vkt::GLFWWindow::~GLFWWindow() {
  if (data)
    glfwDestroyWindow(data);
}

GLFWWindow::GLFWWindow(GLFWWindow &&other) {
  *this = std::move(other);
}

GLFWWindow &GLFWWindow::operator=(GLFWWindow &&other) {
  if (this == &other)
    return *this;
  this->~GLFWWindow();

  data = other.data;
  other.data = nullptr;
  on = std::move(other.on);
  glfwSetWindowUserPointer(data, this);
  return *this;
}
} // namespace vkt