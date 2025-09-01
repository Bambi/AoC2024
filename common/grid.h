#ifndef _GRID_H
#define _GRID_H
#include <cstddef>
#include <vector>
#include <iostream>
#include <iterator>

namespace aoc {
  template<typename T>
  struct grid {
    using row_type = std::vector<T>;
    using data_type = std::vector<row_type>;
    data_type _data;
    static size_t _end(const data_type &d) { return d.size() * d[0].size(); }

    struct iterator {
      data_type &_g;
      size_t _idx{};
      using iterator_category = std::random_access_iterator_tag;
      using value_type        = T;
      using difference_type   = std::ptrdiff_t;
      using pointer           = T*;
      using reference         = T&;
      iterator(data_type &g, size_t size=0) : _g(g), _idx(size) {}
      auto operator=(const iterator &o) -> iterator& { _g = o._g; _idx = o._idx; return *this; }
      auto _cols() const -> size_t { return _g[0].size(); }
      auto set(size_t row, size_t col) { _idx = row*_cols() + col; }
      auto operator*() -> reference { return _g[row()][col()]; }
      auto operator->() -> pointer { return &_g[row()][col()]; }
      // forward
      auto operator++() -> iterator& { ++_idx; return *this; }
      auto operator++(int) -> iterator {
        auto prev = *this;
        ++(*this);
        return prev;
      }
      // bidir
      auto operator--() -> iterator& { --_idx; return *this; }
      iterator operator--(int) { iterator prev = *this; --(*this); return prev; }
      // random access
      auto operator+(difference_type n) const -> iterator { return iterator(_g, _idx + n); }
      auto operator-(difference_type n) const -> iterator { return iterator(_g, _idx - n); }
      auto operator-(const iterator& o) const -> difference_type { return _idx - o._idx; }
      auto operator+=(difference_type n) -> iterator& { _idx += n; return *this; }
      auto operator-=(difference_type n) -> iterator& { _idx -= n; return *this; }
      auto operator[](difference_type n) -> reference { return _g[(_idx + n) / _cols()][(_idx + n) % _cols()]; }
      auto operator==(const iterator &o) -> bool { return _idx == o._idx; }
      auto operator!=(const iterator &o) -> bool { return !(*this == o); }
      auto operator<(const iterator& o) const -> bool { return _idx < o._idx; }
      auto operator>(const iterator& o) const -> bool { return _idx > o._idx; }
      auto operator<=(const iterator& o) const -> bool { return _idx <= o._idx; }
      auto operator>=(const iterator& o) const -> bool { return _idx >= o._idx; }
      auto row() const { return _idx / _cols(); }
      auto col() const { return _idx % _cols(); }
      auto N() const  { return iterator(_g, _idx >= _cols() ? (_idx - _cols()) : grid::_end(_g)); }
      auto E() const  { return iterator(_g, (_idx+1)%_cols() > _idx%_cols() ? _idx+1 : grid::_end(_g)); }
      auto S() const  { return iterator(_g, _idx/_cols() < _g.size()-1 ? _idx + _cols() : grid::_end(_g)); }
      auto W() const  { return iterator(_g, (_idx-1)%_cols() < _idx%_cols() ? _idx-1 : grid::_end(_g)); }
      auto NE() const { return iterator(_g, _idx - _cols()+1); }
      auto SE() const { return iterator(_g, _idx + _cols()+1); }
      auto SW() const { return iterator(_g, _idx + _cols()-1); }
      auto NW() const { return iterator(_g, _idx - _cols()-1); }
    };

    iterator begin() { return iterator(_data, 0); }
    iterator end()   { return iterator(_data, _end(_data)); }
    iterator at(size_t row, size_t col) { auto it=begin(); it.set(row, col); return it; }
    iterator begin() const { return iterator(const_cast<data_type&>(_data), 0); }
    iterator end()   const { return iterator(const_cast<data_type&>(_data), _end(_data)); }
  };
}

auto operator<<(std::ostream &out, typename aoc::grid<char>::iterator const& i) -> std::ostream&;
auto operator<<(std::ostream &out, aoc::grid<char>::row_type const&l) -> std::ostream&;
#endif // _GRID_H

#ifdef _GRID_H_IMPL
auto operator<<(std::ostream &out, typename aoc::grid<char>::iterator const& i) -> std::ostream& {
  out << '[' << i.row()+1 << ',' << i.col()+1 << ']';
  return out;
}

auto operator<<(std::ostream &out, aoc::grid<char>::row_type const&l) -> std::ostream& {
  std::copy(l.begin(), l.end(), std::ostream_iterator<char>(out));
  return out;
}
#endif
