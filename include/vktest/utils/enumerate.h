#pragma once

namespace vkt {
template <typename Iter>
class enumerate {
public:
  enumerate(Iter begin, Iter end) : begin_{begin}, end_{end} {}

  template <typename Container>
  enumerate(Container const &container)
      : enumerate(container.begin(), container.end()) {}

  class iterator {
  public:
    iterator(Iter cur, std::size_t start = 0) : cur{cur}, idx{start} {}

    auto operator*() const {
      return std::tie(idx, *cur);
    }

    iterator &operator++() {
      ++cur;
      ++idx;
      return *this;
    }

    bool operator!=(iterator const &other) const {
      return cur != other.cur;
    }

  private:
    std::size_t idx;
    Iter cur;
  };

  auto begin() const {
    return iterator(begin_);
  }

  auto end() const {
    return iterator(end_);
  }

private:
  Iter begin_, end_;
};

template <typename Container>
enumerate(Container const &container) -> enumerate<decltype(container.begin())>;
} // namespace vkt