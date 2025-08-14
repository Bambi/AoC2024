#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <algorithm>

struct pos_t {
  int l{}, c{}; // indexes: from 0 to size-1

  auto operator-(const pos_t &p) const -> pos_t { // returns a distance
    return pos_t(l-p.l, c-p.c);
  }
  auto operator+(const pos_t &p) const -> pos_t {
    return pos_t(l+p.l, c+p.c);
  }
};
auto operator<<(std::ostream &out, pos_t const& p) -> std::ostream& {
  out << '[' << p.l+1 << ',' << p.c+1 << ']';
  return out;
}
auto operator==(const pos_t &a, const pos_t &b) -> bool {
  return a.l == b.l && a.c == b.c;
}
auto operator<(const pos_t &a, const pos_t &b) -> bool
{
  return memcmp(&a, &b, sizeof(a)) < 0;
}

struct grid_t {
  std::vector<std::string> grid;

  auto operator[](size_t i) const -> const std::string& {
    return grid[i];
  }
  auto operator[](const pos_t &p) const -> char {
    return grid[p.l][p.c];
  }
  auto parse() {
    for (std::string line; std::getline(std::cin, line);) {
      grid.push_back(line);
    }
  }

  auto valid(pos_t &p) const -> bool {
    return p.l < grid.size() && p.c < grid[0].size();
  }
};

auto process(const pos_t &p, const grid_t &g, std::set<pos_t> &a) {
  for (unsigned l=0; l<g.grid.size(); l++) {
    auto c = g[l].begin();
    while ((c=std::find(c, g[l].end(), g[p])) != g[l].end()) {
      unsigned i = std::distance(g[l].begin(), c);
      if (!(l==p.l && i==p.c)) { // skip p
        auto cpos = pos_t(l, i);
        pos_t d = cpos - p; // compute distance
        pos_t npos = p - d;
        if (g.valid(npos))
          a.insert(npos);
      }
      ++c;
    }
  }
}

auto main() -> int {
  grid_t g;
  std::set<pos_t> antinodes;
  g.parse();
  for (unsigned i=0 ; i<g.grid.size() ; i++) {
    for (unsigned j=0 ; j<g.grid.size() ; j++) {
      if (g[i][j] != '.')
        process(pos_t(i, j), g, antinodes);
    }
  }
  std::cout << antinodes.size() << std::endl;
}
