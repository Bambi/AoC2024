#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstring>
#include <set>
#include <tuple>

struct pos_t {
  size_t l{}, c{}; // indexes: from 0 to size-1
};
auto operator<<(std::ostream &out, pos_t const& p) -> std::ostream& {
  out << '[' << p.l+1 << ',' << p.c+1 << ']';
  return out;
}
auto operator==(const pos_t &a, const pos_t &b) -> bool {
  return a.l == b.l && a.c == b.c;
}

struct dir_t {
  static const std::array<std::function<pos_t (pos_t)>, 4> advance;
  enum Dir { N, E, S, W } dir{Dir::N};

  [[nodiscard]] auto rotate() const -> dir_t {
    return dir_t(static_cast<Dir>((dir + 1) % 4));
  }
};
const std::array<std::function<pos_t (pos_t p)>, 4> dir_t::advance = {
  [](pos_t p){ p.l -= 1; return p; }, // N
  [](pos_t p){ p.c += 1; return p; }, // E
  [](pos_t p){ p.l += 1; return p; }, // S
  [](pos_t p){ p.c -= 1; return p; }, // W
};
auto operator<<(std::ostream &out, dir_t const& p) -> std::ostream& {
  static const char l[] = { 'N', 'E', 'S', 'W' }; // NOLINT(modernize-avoid-c-arrays)
  out << l[p.dir];
  return out;
}

struct grid_t {
  std::vector<std::string> grid;

  auto parse() -> pos_t {
    pos_t res;
    for (std::string line; std::getline(std::cin, line);) {
      size_t start = line.find('^');
      if (start != std::string::npos) {
        // record starting point & replace ^ by .
        res = pos_t(grid.size(), start);
        line[start] = '.';
      }
      grid.push_back(line);
    }
    return res;
  }

  auto valid(pos_t &p) const -> bool {
    return p.l < grid.size() && p.c < grid[0].size(); 
  }

  [[nodiscard]] auto get_next(pos_t p, dir_t d) const -> char {
    auto np = dir_t::advance[d.dir](p);
    if (valid(np))
      return grid[np.l][np.c];
    else
      return '\0';
  }
};
  
auto operator<(const pos_t &a, const pos_t &b) -> bool
{
  return memcmp(&a, &b, sizeof(a)) < 0;
}
auto operator<(const std::tuple<pos_t, dir_t> &a, const std::tuple<pos_t, dir_t> &b) -> bool
{
  if (std::get<0>(a).l < std::get<0>(b).l) return true;
  if (std::get<0>(b).l < std::get<0>(a).l) return false;
  if (std::get<0>(a).c < std::get<0>(b).c) return true;
  if (std::get<0>(b).c < std::get<0>(a).c) return false;
  return std::get<1>(a).dir < std::get<1>(b).dir;
}

auto simulate(const grid_t g, pos_t p, dir_t d) -> bool {
  pos_t obs = dir_t::advance[d.dir](p); // obstacle psoition
  std::set<std::tuple<pos_t, dir_t>> visited{};
  d = d.rotate();
  unsigned nb{};
  while (g.valid(p)) {
    if (g.get_next(p, d) == '#' || dir_t::advance[d.dir](p) == obs) {
      d = d.rotate();
    } else {
      std::tuple<pos_t, dir_t> t = std::make_tuple(p, d);
      if (visited.contains(t)) {
        return true;
      } else {
        visited.insert(t);
        p = dir_t::advance[d.dir](p);
        nb += 1;
      }
    }
  }
  return false;
}
  
auto move(grid_t grid, pos_t pos) {
  size_t paradox{};
  std::set<pos_t> visited{};
  dir_t dir(dir_t::N);
  while(grid.valid(pos)) {
    if (grid.get_next(pos, dir) == '#') {
      dir = dir.rotate();
    } else {
      auto npos = dir_t::advance[dir.dir](pos);
      if (!visited.contains(npos) && simulate(grid, pos, dir)) {
        paradox +=1;
      }
      visited.insert(pos);
      pos = npos;
    }
  }
  std::cout << visited.size() << ' ' << paradox << std::endl;
}

auto main() -> int {
  grid_t grd;
  pos_t pos = grd.parse();
  move(grd, pos);
}
