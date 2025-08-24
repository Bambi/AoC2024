#include <iostream>
#include <cstring>

struct pos_t {
  ushort r{}, c{}; // indexes: from 0 to size-1

  auto operator==(const pos_t &o) -> bool { return r == o.r && c == o.c; }
  auto operator!=(const pos_t &o) -> bool { return !(*this == o); }
  auto operator<(const pos_t &o) const -> bool { return memcmp(this, &o, sizeof(*this)) < 0; }
  auto N() const -> pos_t { return pos_t(r-1, c); }
  auto E() const -> pos_t { return pos_t(r  , c+1); }
  auto S() const -> pos_t { return pos_t(r+1, c); }
  auto W() const -> pos_t { return pos_t(r  , c-1); }
};
inline auto operator<<(std::ostream &out, pos_t const& p) -> std::ostream& {
  out << '[' << p.r+1 << ',' << p.c+1 << ']';
  return out;
}

// iterate over a grid with 'row_size' row elements
// from NW to SW
struct grid_iterator_t {
  const ushort rsize;
  pos_t p;
  grid_iterator_t(ushort row_size) : rsize(row_size), p({}) {}
  grid_iterator_t(ushort row_size, const pos_t &pos) : rsize(row_size), p(pos) {}

  auto operator*() const -> pos_t { return p; }
  auto operator++() -> grid_iterator_t& {
    if (p.c == rsize-1)
      p.r++;
    p.c = (p.c+1) % rsize;
    return *this;
  }
  auto operator++(int) -> grid_iterator_t {
    auto prev = *this;
    ++(*this);
    return prev;
  }
  auto operator==(const grid_iterator_t &o) -> bool { return p == o.p; }
  auto operator!=(const grid_iterator_t &o) -> bool { return !(*this == o); }
};

