#pragma once
#include <memory>

namespace vkt {
template <typename T>
class swap_shared_ptr : public std::shared_ptr<T> {
public:
  template <typename... Args>
  swap_shared_ptr(Args &&...args)
      : std::shared_ptr<T>(std::forward<Args>(args)...) {}

  swap_shared_ptr(swap_shared_ptr &&other) = default;

  swap_shared_ptr &operator=(swap_shared_ptr &&other) {
    if (this != &other) {
      std::swap((std::shared_ptr<T> &)(*this), (std::shared_ptr<T> &)other);
    }
    return *this;
  }

  template <typename U>
  swap_shared_ptr &operator=(U &&other) {
    this->std::shared_ptr<T>::operator=(std::forward<U>(other));
    return *this;
  }
};
} // namespace vkt