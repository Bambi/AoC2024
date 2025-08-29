#include <iostream>
#include <sys/types.h>
#include <vector>
#include <string>
#include <functional>
#include <cassert>
#include "position.h"

using rpos_t = pos_t<ushort>;

struct grid_t {
  std::vector<std::string> g;
  rpos_t p{};

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
          p.c = pos;
          p.r = nb;
          line[pos] = '.';
        }
        g.push_back(line);
      }
      nb++;
    }
  }
  auto print() {
    for (auto l: g)
      std::cout << l << std::endl;
    std::cout << p << std::endl;
  }
  auto box(const rpos_t &p) { return g[p.r][p.c]; }
  auto begin() { return grid_iterator_t(g[0].size()); }
  auto end() { return grid_iterator_t(g[0].size(), rpos_t(g.size(),0)); }
  static auto fctdir(char dir) {
    switch(dir) {
      case '^': return &rpos_t::N;
      case 'v': return &rpos_t::S;
      case '<': return &rpos_t::W;
      case '>': return &rpos_t::E;
    }
    assert(false);
  }
  auto find_free(char d, rpos_t p) {
    rpos_t np = std::invoke(fctdir(d), p);
    switch (box(np)) {
      case '.': return np;
      case '#': return rpos_t(0,0);
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
        if (fpos != rpos_t(0,0)) {
          auto npos = std::invoke(fctdir(dir), p);
          if (fpos != npos)
            std::swap(g[npos.r][npos.c], g[fpos.r][fpos.c]);
          p = npos;
        }
      }
    }
  }
  auto gps_sum() {
    unsigned res{};
    for (auto it=begin(); it!=end(); ++it) {
      if (box(*it) == 'O')
        res += (*it).r*100 + (*it).c;
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
