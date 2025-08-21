#include <iostream>
#include <sys/types.h>
#include <vector>
#include <set>
#include <cstring>

struct grid_t;
struct pos_t {
  ushort l{}, c{}; // indexes: from 0 to size-1

  auto operator==(const pos_t &o) -> bool { return l == o.l && c == o.c; }
  auto operator!=(const pos_t &o) -> bool { return !(*this == o); }
  auto operator<(const pos_t &o) const -> bool { return memcmp(this, &o, sizeof(*this)) < 0; }
  [[nodiscard]] auto valid(const grid_t &g) const -> bool;
  auto N() -> pos_t { return pos_t(l-1, c); }
  auto E() -> pos_t { return pos_t(l  , c+1); }
  auto S() -> pos_t { return pos_t(l+1, c); }
  auto W() -> pos_t { return pos_t(l  , c-1); }
};
auto operator<<(std::ostream &out, pos_t const& p) -> std::ostream& {
  out << '[' << p.l+1 << ',' << p.c+1 << ']';
  return out;
}

struct pos_iterator_t {
  pos_t p;
  const grid_t &g;
  char x;
  pos_iterator_t(const grid_t &g, char t): p(), g(g), x(t) { advance(); }
  pos_iterator_t(const grid_t &g, pos_t p): p(p), g(g), x() {}

  auto next() -> void;
  auto advance() -> void;
  [[nodiscard]] auto valid() const -> bool;
  auto operator*() const -> pos_t { return p; }
  auto operator++() -> pos_iterator_t& {
    next();
    advance();
    return *this;
  }
  auto operator++(int) -> pos_iterator_t {
    auto prev = *this;
    ++(*this);
    return prev;
  }
  auto operator==(const pos_iterator_t &o) -> bool { return p == o.p; }
  auto operator!=(const pos_iterator_t &o) -> bool { return !(*this == o); }
};

struct grid_t {
  std::vector<std::string> g;

  auto parse(std::istream &f) {
    for (std::string line; std::getline(f, line);) {
      g.push_back(line);
    }
  }
  auto operator[](const pos_t &p) const -> char {
    return g[p.l][p.c];
  }
  [[nodiscard]] auto begin(char t) const -> pos_iterator_t;
  [[nodiscard]] auto end() const -> pos_iterator_t;
};

auto grid_t::begin(char t) const -> pos_iterator_t {
  return { *this, t };
}
auto grid_t::end() const -> pos_iterator_t {
  return { *this, pos_t(g.size()) };
}

auto pos_iterator_t::valid() const -> bool { return p.l < g.g.size(); }
auto pos_iterator_t::next() -> void {
  if (p.c == g.g[0].size()-1)
    p.l++;
  p.c = (p.c+1) % g.g[0].size();
}
auto pos_iterator_t::advance() -> void {
  while (valid() && g[p] != x) {
    next();
  }
}

auto pos_t::valid(const grid_t &g) const -> bool { return l<g.g.size() && c<g.g[0].size(); }

auto path(std::set<pos_t> &reach, unsigned &npath, grid_t &g, pos_t p, char level) -> void {
  if (p.valid(g) && g[p] == level) {
    if (level == '9') {
      reach.insert(p);
      npath++;
    } else {
      path(reach, npath, g, p.N(), level+1);
      path(reach, npath, g, p.E(), level+1);
      path(reach, npath, g, p.S(), level+1);
      path(reach, npath, g, p.W(), level+1);
    }
  }
}

auto main() -> int {
  unsigned reachable{}, nbpath{};
  grid_t grid;
  grid.parse(std::cin);
  for (auto it=grid.begin('0'); it!=grid.end(); ++it) {
    std::set<pos_t> trails;
    path(trails, nbpath, grid, *it, '0');
    reachable += trails.size();
  }
  std::cout << reachable << ' ' << nbpath << std::endl;
}
