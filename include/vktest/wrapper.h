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
    data = std::move(other.data);
    other.data = {};
  }

  Wrapper &operator=(Wrapper &&other) {
    if (this != &other) {
      this->~Wrapper();
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

  Data *operator->() {
    return &data;
  }

  Data const *operator->() const {
    return &data;
  }

protected:
  Data data = {};
};

template <typename Data>
class Wrapper<Data *> {
public:
  Wrapper() = default;
  virtual ~Wrapper() = default;

  Wrapper(Wrapper const &) = delete;
  Wrapper &operator=(Wrapper const &) = delete;

  Wrapper(Wrapper &&other) {
    data = other.data;
    other.data = nullptr;
  }

  Wrapper &operator=(Wrapper &&other) {
    if (this != &other) {
      this->~Wrapper();
      std::swap(data, other.data);
    }
    return *this;
  }

  operator Data * const &() const {
    return data;
  }

  operator Data *&() {
    return data;
  }

  Data *&base() {
    return data;
  }

  Data *operator->() const {
    return data;
  }

protected:
  Data *data = nullptr;
};
} // namespace vkt