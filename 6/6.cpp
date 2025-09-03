#include <cstddef>
#include <iostream>
#include <string>
#include <set>
#include <sys/types.h>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  static auto rotate(dir d) -> dir {
    switch (d) {
      case dir::N: return dir::E;
      case dir::E: return dir::S;
      case dir::S: return dir::W;
      case dir::W: return dir::N;
      default: std::abort();
    }
  }
  auto parse(std::istream &f) -> iterator {
    size_t r{}, c{}, lg{};
    for (std::string line; std::getline(f, line);) {
      size_t start = line.find('^');
      if (start != std::string::npos) {
        // record starting point & replace ^ by .
        r = lg; c= start;
        line[start] = '.';
      }
      add_line(line);
      lg++;
    }
    return {*this, r, c};
  }
  auto move(size_t pos) {
    size_t paradox{};
    std::set<size_t> visited{};
    dir cdir(dir::N);
    while(pos != npos) {
      if ((*this)(peek(pos, cdir)) == '#') {
        cdir = rotate(cdir);
      } else {
        auto nextpos = peek(pos, cdir);
        if (!visited.contains(nextpos) && simulate(pos, cdir)) {
          paradox +=1;
        }
        visited.insert(pos);
        pos = nextpos;
      }
    }
    std::cout << visited.size() << ' ' << paradox << std::endl;
  }
  auto simulate(size_t p, dir d) -> bool {
    auto obs = peek(p, d); // obstacle position
    if (obs == npos)
      return false;
    std::set<std::pair<size_t, dir>> visited{};
    d = rotate(d);
    while (p != npos) {
      auto next = peek(p, d);
      if (next != npos && (next == obs || (*this)(next) == '#')) {
        d = rotate(d);
      } else {
        std::pair<size_t, dir> t = std::make_pair(p, d);
        if (visited.contains(t)) {
          return true;
        } else {
          visited.insert(t);
          p = next;
        }
      }
    }
    return false;
  }
};
  
auto main() -> int {
  grid_t grd;
  auto pos = grd.parse(std::cin);
  grd.move(pos.idx_);
}

#define _GRID_H_IMPL
#include "grid.h"
