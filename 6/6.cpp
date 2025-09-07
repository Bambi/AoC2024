#include <cstddef>
#include <iostream>
#include <string>
#include <set>
#include <sys/types.h>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  static auto rotate(aoc::dir d) -> aoc::dir {
    switch (d) {
      case aoc::dir::N: return aoc::dir::E;
      case aoc::dir::E: return aoc::dir::S;
      case aoc::dir::S: return aoc::dir::W;
      case aoc::dir::W: return aoc::dir::N;
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
      add_data(line);
      lg++;
    }
    return {*this, r, c};
  }
  auto move(size_t pos) {
    size_t paradox{};
    std::set<size_t> visited{};
    aoc::dir cdir(aoc::dir::N);
    while(pos != aoc::npos) {
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
  auto simulate(size_t p, aoc::dir d) -> bool {
    auto obs = peek(p, d); // obstacle position
    if (obs == aoc::npos)
      return false;
    std::set<std::pair<size_t, aoc::dir>> visited{};
    d = rotate(d);
    while (p != aoc::npos) {
      auto next = peek(p, d);
      if (next != aoc::npos && (next == obs || (*this)(next) == '#')) {
        d = rotate(d);
      } else {
        std::pair<size_t, aoc::dir> t = std::make_pair(p, d);
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
