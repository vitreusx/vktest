#pragma once
#include <memory>
#include <type_traits>

namespace vkt {
namespace detail {
template <typename Ret, typename... Args>
class VirtCb {
public:
  virtual Ret impl(Args... args) = 0;
};

template <typename F, typename Ret, typename... Args>
class ProxyCb : public VirtCb<Ret, Args...> {
public:
  explicit ProxyCb(F f) : f{std::move(f)} {}

  virtual Ret impl(Args... args) {
    if constexpr (std::is_void_v<Ret>)
      f(std::forward<Args>(args)...);
    else
      return f(std::forward<Args>(args)...);
  }

private:
  F f;
};

template <typename Ret, typename... Args>
class _Callback : public VirtCb<Ret, Args...> {
public:
  virtual Ret impl(Args... args) {
    if (cb) {
      if constexpr (std::is_void_v<Ret>)
        cb->impl(std::forward<Args>(args)...);
      else
        return cb->impl(std::forward<Args>(args)...);
    } else {
      if constexpr (!std::is_void_v<Ret>)
        return Ret();
    }
  }

  Ret operator()(Args... args) {
    if constexpr (std::is_void_v<Ret>)
      impl(std::forward<Args>(args)...);
    else
      return impl(std::forward<Args>(args)...);
  }

  template <typename F>
  _Callback &operator=(F f) {
    cb = std::make_shared<ProxyCb<F, Ret, Args...>>(std::move(f));
    return *this;
  }

private:
  std::shared_ptr<VirtCb<Ret, Args...>> cb;
};

template <typename Sig>
struct _SigCb;

template <typename Ret, typename... Args>
struct _SigCb<Ret (*)(Args...)> {
  using type = _Callback<Ret, Args...>;
};
} // namespace detail

template <typename Sig>
using Callback = typename detail::_SigCb<Sig>::type;
} // namespace vkt