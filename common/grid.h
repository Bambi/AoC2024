#ifndef _GRID_H
#define _GRID_H
#include <cstddef>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cassert>

namespace aoc {
  template<typename T>
  struct grid {
    using data_type = std::vector<T>;
    data_type data_;
    size_t cols_; // number of columns
    enum dir { N, S, E, W };
    static const size_t npos = -1; // illegal index
    static auto inv(dir d) -> dir {
      switch (d) {
        case dir::N: return dir::S;
        case dir::S: return dir::N;
        case dir::E: return dir::W;
        case dir::W: return dir::E;
      }
      std::abort();
    }

    auto operator()(size_t r, size_t c) -> T& { return data_[r * cols_ + c]; }
    auto operator()(size_t idx) -> T& { return data_[idx]; }
    auto add_data(const std::string& l) { std::ranges::copy(l, std::back_inserter(data_)); cols_ = l.size(); }
    auto add_data(const T& d) { data_.push_back(d); }
    auto peek(size_t idx, dir d) const -> size_t {
      if (idx == npos) return npos;
      switch (d) {
        case N: return row(idx) == 0 ? npos : idx - cols_;
        case S: return (idx + cols_) < data_.size() ? idx + cols_ : npos;
        case E: return (idx + 1)%cols_ > idx%cols_ ? idx + 1 : npos;
        case W: return (idx - 1)%cols_ < idx%cols_ ? idx - 1 : npos;
      }
      assert(false);
    }
    auto row(size_t idx) const -> size_t { return idx / cols_; }
    auto col(size_t idx) const -> size_t { return idx % cols_; }
    auto rows() const -> size_t { return data_.size() / cols_; }
    auto cols() const -> size_t { return cols_; }

    struct dir_iterator {
      grid &g_;
      size_t idx_{};
      dir d_;
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type        = T;
      using difference_type   = std::ptrdiff_t;
      using pointer           = T*;
      using reference         = T&;

      dir_iterator(grid &g, dir dir, size_t idx) : g_(g), idx_(idx), d_(dir) {}
      auto operator*() const -> reference { return g_.data_[idx_]; }
      auto operator->() -> pointer { return &g_.data_[idx_]; }
      auto operator++() -> dir_iterator& { idx_ = g_.peek(idx_, d_); return *this; }
      auto operator++(int) -> dir_iterator { auto tmp = *this; idx_ = g_.peek(idx_, d_); return tmp; }
      auto operator--() -> dir_iterator& { idx_ = g_.peek(idx_, inv(d_)); return *this; }
      auto operator--(int) -> dir_iterator { auto tmp = *this; idx_ = g_.peek(idx_, inv(d_)); return tmp; }
      auto operator==(const dir_iterator &o) -> bool { return d_ != o.d_ || idx_ != o.idx_; }
      auto operator==(std::default_sentinel_t) const -> bool { return idx_ == npos; }
      auto move_card(dir dir) const -> dir_iterator { return { g_, d_, g_.peek(idx_, dir) }; }
    };

    struct iterator {
      grid &g_;
      size_t idx_{};
      using iterator_category = std::random_access_iterator_tag;
      using value_type        = T;
      using difference_type   = std::ptrdiff_t;
      using pointer           = T*;
      using reference         = T&;
      iterator(grid&g, size_t idx=0) : g_(g), idx_(idx) {}
      iterator(grid&g, size_t row, size_t col) : g_(g), idx_(row * g_.cols_ + col) {}
      auto operator=(const iterator &o) -> iterator& { g_ = o.g_; idx_ = o.idx_; return *this; }
      auto operator*() -> reference { return g_.data_[idx_]; }
      auto operator->() -> pointer { return &g_.data_[idx_]; }
      // forward
      auto operator++() -> iterator& { ++idx_; return *this; }
      auto operator++(int) -> iterator {
        auto prev = *this;
        ++(*this);
        return prev;
      }
      // bidir
      auto operator--() -> iterator& { --idx_; return *this; }
      auto operator--(int) -> iterator { iterator prev = *this; --(*this); return prev; }
      // random access
      auto operator+(difference_type n) const -> iterator { return iterator(g_, idx_ + n); }
      auto operator-(difference_type n) const -> iterator { return iterator(g_, idx_ - n); }
      auto operator-(const iterator& o) const -> difference_type { return idx_ - o.idx_; }
      auto operator+=(difference_type n) -> iterator& { idx_ += n; return *this; }
      auto operator-=(difference_type n) -> iterator& { idx_ -= n; return *this; }
      auto operator[](difference_type n) -> reference { return g_[idx_]; }
      auto operator==(const iterator &o) -> bool { return idx_ == o.idx_; }
      auto operator!=(const iterator &o) -> bool { return !(*this == o); }
      auto operator<(const iterator& o) const -> bool { return idx_ < o.idx_; }
      auto operator>(const iterator& o) const -> bool { return idx_ > o.idx_; }
      auto operator<=(const iterator& o) const -> bool { return idx_ <= o.idx_; }
      auto operator>=(const iterator& o) const -> bool { return idx_ >= o.idx_; }
      auto row() const { return g_.row(idx_); }
      auto col() const { return g_.col(idx_); }
    };

    iterator begin() { return iterator(*this, 0); }
    iterator end()   { return iterator(*this, data_.size()); }
    iterator at(size_t row, size_t col) { auto it=begin(); it.set(row, col); return it; }
    iterator begin() const { return iterator(const_cast<grid&>(*this), 0); }
    iterator end()   const { return iterator(const_cast<grid&>(*this), end_(data_)); }
  };
}

auto operator<<(std::ostream &out, typename aoc::grid<char>::iterator const& i) -> std::ostream&;
auto operator<<(std::ostream &out, aoc::grid<char> const& g) -> std::ostream&;
#endif // _GRID_H

#ifdef _GRID_H_IMPL
auto operator<<(std::ostream &out, typename aoc::grid<char>::iterator const& i) -> std::ostream& {
  out << '[' << (i.idx_/i.g_.cols_+1) << ',' << (i.idx_%i.g_.cols_+1) << ']';
  return out;
}

auto operator<<(std::ostream &out, aoc::grid<char> const& g) -> std::ostream& {
  for (size_t r=0; r<g.rows(); r++) {
    std::copy(g.data_.begin()+r*g.cols(), g.data_.begin()+(r+1)*g.cols(), std::ostream_iterator<char>(out));
    if (r < g.rows()-1)
      std::cout << std::endl;
  }
  return out;
}
void printit(size_t cols, size_t idx) { std::cout << '[' << (idx/cols+1) << ',' << (idx%cols+1) << ']' << std::endl; }
#endif
