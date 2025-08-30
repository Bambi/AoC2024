#include <iostream>
#include <sys/types.h>
#include <string>
#include <functional>
#include <cassert>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  grid<char>::iterator p;
  grid_t() : p(_data) {};

  auto parse(std::istream &f) {
    ushort nb{};
    while (!f.eof()) {
      std::string line;
      std::getline(f, line);
      if (line.empty()) {
        break;
      } else {
        auto pos = line.find('@');
        if (pos != std::string::npos) {
          p.set(nb, pos);
          line[pos] = '.';
        }
        row_type l(line.begin(), line.end());
        _data.push_back(l);
      }
      nb++;
    }
  }
  auto print() {
    for (auto l: _data)
      std::cout << l << std::endl;
    std::cout << p << std::endl;
  }
  static auto fctdir(char dir) -> iterator (iterator::*)()const {
    switch(dir) {
      case '^': return &iterator::N;
      case 'v': return &iterator::S;
      case '<': return &iterator::W;
      case '>': return &iterator::E;
    }
    assert(false);
  }
  auto find_free(char d, iterator &p) {
    iterator np = std::invoke(fctdir(d), p);
    switch (*np) {
      case '.': return np;
      case '#': return end();
      case 'O': return find_free(d, np);
    }
    assert(false);
  }
  auto move(std::istream &f) {
    std::string l; 
    while (std::getline(f, l) && !f.eof()) {
      if (l.empty())
        continue;
      for (auto dir: l) {
        auto fpos = find_free(dir, p);
        if (fpos != end()) {
          auto npos = std::invoke(fctdir(dir), p);
          if (fpos != npos)
            std::swap(*npos, *fpos);
          p = npos;
        }
      }
    }
  }
  auto gps_sum() {
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
  grid.parse(std::cin);
  grid.move(std::cin);
  std::cout << grid.gps_sum() << std::endl;
}

#define _DGRID_H_IMPL
#include "grid.h"
