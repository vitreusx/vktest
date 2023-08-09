#pragma once
#include <functional>
#include <vktest/cb.h>
#include <vktest/glfw_sys.h>
#include <vktest/wrapper.h>

namespace vkt::detail {
template <typename Sig>
struct _GLFWCb;

template <typename Ret, typename... Args>
struct _GLFWCb<Ret (*)(GLFWwindow *, Args...)> {
  using type = Callback<Ret (*)(Args...)>;
};

template <typename Sig>
using GLFWCb = typename _GLFWCb<Sig>::type;
} // namespace vkt::detail

namespace vkt {
class GLFWWindow;

class GLFWCallbacks {
public:
  detail::GLFWCb<GLFWkeyfun> keyEvent;
  detail::GLFWCb<GLFWwindowsizefun> windowSizeEvent;
  detail::GLFWCb<GLFWframebuffersizefun> framebufferSizeEvent;

private:
  friend class GLFWWindow;

  template <std::size_t I>
  auto &get() {
    return std::get<I>(
        std::tie(keyEvent, windowSizeEvent, framebufferSizeEvent));
  }
};

class GLFWWindow : public Wrapper<GLFWwindow *> {
public:
  GLFWWindow() = default;
  GLFWWindow(GLFWSys const &, int width, int height, std::string const &title,
             int clientApi);
  ~GLFWWindow();

  GLFWWindow(GLFWWindow &&);
  GLFWWindow &operator=(GLFWWindow &&);

  std::shared_ptr<GLFWCallbacks> on;

private:
  template <std::size_t I, typename Sig>
  struct _MakeCb;

  template <std::size_t I, typename... Args>
  static void callback(GLFWwindow *handle, Args... args);
};
} // namespace vkt