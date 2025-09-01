#include <iostream>
#include <string>
#include <array>
#include <functional>
#include <set>
#include <sys/types.h>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  struct dir_t {
    enum Dir { N, E, S, W } dir{Dir::N};
    [[nodiscard]] auto rotate() const -> dir_t {
      return dir_t(static_cast<Dir>((dir + 1) % 4));
    }
    auto advance(iterator it) -> iterator {
      static const std::array<iterator (iterator::*)()const, 4> _adv = {
        &aoc::grid<char>::iterator::N,
        &aoc::grid<char>::iterator::E,
        &aoc::grid<char>::iterator::S,
        &aoc::grid<char>::iterator::W,
      };
      return std::invoke(_adv[dir], it);
    }
  };
  auto parse(std::istream &f) -> iterator {
    iterator res(_data);
    for (std::string line; std::getline(f, line);) {
      size_t start = line.find('^');
      if (start != std::string::npos) {
        // record starting point & replace ^ by .
        res.set(_data.size(), start);
        line[start] = '.';
      }
      _data.emplace_back(line.begin(), line.end());
    }
    return res;
  }
  auto move(iterator pos) {
    size_t paradox{};
    std::set<size_t> visited{};
    dir_t dir(dir_t::N);
    while(pos != end()) {
      if (peek(dir.advance(pos)) == '#') {
        dir = dir.rotate();
      } else {
        auto npos = dir.advance(pos);
        if (!visited.contains(npos._idx) && simulate(pos, dir)) {
          paradox +=1;
        }
        visited.insert(pos._idx);
        pos = npos;
      }
    }
    std::cout << visited.size() << ' ' << paradox << std::endl;
  }
  auto simulate(iterator p, dir_t d) -> bool {
    auto obs = d.advance(p); // obstacle position
    if (obs == end())
      return false;
    std::set<std::pair<size_t, dir_t>> visited{};
    d = d.rotate();
    while (p != end()) {
      if (peek(d.advance(p)) == '#' || d.advance(p) == obs) {
        d = d.rotate();
      } else {
        std::pair<size_t, dir_t> t = std::make_pair(p._idx, d);
        if (visited.contains(t)) {
          return true;
        } else {
          visited.insert(t);
          p = d.advance(p);
        }
      }
    }
    return false;
  }
  [[nodiscard]] auto peek(iterator it) const -> char {
    if (it != end())
      return *it;
    else
      return '\0';
  }
};
  
auto main() -> int {
  grid_t grd;
  auto pos = grd.parse(std::cin);
  grd.move(pos);
}

#define _GRID_H_IMPL
#include "grid.h"
auto operator<(const std::pair<size_t, grid_t::dir_t> &a, const std::pair<size_t, grid_t::dir_t> &b) -> bool {
  if (a.first < b.first) return true;
  if (a.first > b.first) return false;
  return a.second.dir < b.second.dir;
}
