#include <cstddef>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <string>
#include <algorithm>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  auto parse(std::istream &f) -> iterator {
    ushort nb{}, r{}, c{};
    while (!f.eof()) {
      std::string line;
      std::getline(f, line);
      if (line.empty()) {
        break;
      } else {
        auto p = line.find('@');
        if (p != std::string::npos) {
          line[p] = '.';
          c = p;
          r = nb;
        }
        add_data(line);
      }
      nb++;
    }
    return {*this, r, c};
  }
  static auto fctdir(char dir) -> enum aoc::dir {
    switch(dir) {
      case '^': return aoc::dir::N;
      case 'v': return aoc::dir::S;
      case '<': return aoc::dir::W;
      case '>': return aoc::dir::E;
    }
    std::abort();
  }
  auto find_free(char d, size_t p) -> size_t {
    size_t np = peek(p, fctdir(d));
    switch ((*this)(np)) {
      case '.': return np;
      case '#': return aoc::npos;
      case 'O': return find_free(d, np);
    }
    assert(false);
  }
  auto move(std::istream &f, size_t p) {
    std::string l; 
    while (std::getline(f, l) && !f.eof()) {
      if (l.empty())
        continue;
      for (auto dir: l) {
        auto fpos = find_free(dir, p);
        if (fpos != aoc::npos) {
          auto nextp = peek(p, fctdir(dir));
          if (nextp != fpos)
            std::swap((*this)(nextp), (*this)(fpos));
          p = nextp;
        }
      }
    }
  }
  auto gps_sum() -> unsigned {
    unsigned res{};
    for (auto it=begin(); it!=end(); ++it) {
      if (*it == 'O')
        res += it.row()*100 + it.col();
    }
    return res;
  }
};

auto main() -> int {
  grid_t grid;
  auto p = grid.parse(std::cin);
  grid.move(std::cin, p.idx_);
  std::cout << grid.gps_sum() << std::endl;
}

#define _GRID_H_IMPL
#include "grid.h"
