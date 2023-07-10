#pragma once

namespace vkt {
template <typename CreateInfo>
class Builder {
public:
  Builder() = default;

  Builder(Builder const &) = default;
  Builder &operator=(Builder const &) = default;

  Builder(Builder &&other) {
    this->info = std::move(other.info);
    other.info = {};
  }

  Builder &operator=(Builder &&other) {
    if (this != &other) {
      this->info = std::move(other.info);
      other.info = {};
    }
    return *this;
  }

  operator CreateInfo &() {
    return info;
  }

  operator CreateInfo const &() const {
    return info;
  }

  CreateInfo &get() {
    return info;
  }

  CreateInfo const &get() const {
    return info;
  }

  CreateInfo *operator->() {
    return &info;
  }

  CreateInfo const *operator->() const {
    return &info;
  }

protected:
  CreateInfo info = {};
};
} // namespace vkt