#pragma once
#include <type_traits>

namespace vkt {
template <typename Data>
class Wrapper {
public:
  Wrapper() = default;
  virtual ~Wrapper() = default;

  Wrapper(Wrapper const &) = delete;
  Wrapper &operator=(Wrapper const &) = delete;

  Wrapper(Wrapper &&other) {
    data = {};
    std::swap(data, other.data);
  }

  Wrapper &operator=(Wrapper &&other) {
    if (this != &other) {
      std::swap(data, other.data);
    }
    return *this;
  }

  operator Data const &() const {
    return data;
  }

  operator Data &() {
    return data;
  }

  Data &base() {
    return data;
  }

  Data const &base() const {
    return data;
  }

  auto *operator->() {
    if constexpr (std::is_pointer_v<Data>)
      return data;
    else
      return &data;
  }

  auto const *operator->() const {
    if constexpr (std::is_pointer_v<Data>)
      return data;
    else
      return &data;
  }

protected:
  Data data = {};
};
} // namespace vkt