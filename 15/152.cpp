#include <iostream>
#include <sys/types.h>
#include <string>
#include <functional>
#include <algorithm>
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
        row_type r;
        r.resize(line.size() * 2);
        std::ranges::for_each(line, [&](char x) {
          switch (x) {
            case '@': r.push_back('.'); r.push_back('.'); return;
            case 'O': r.push_back('['); r.push_back(']'); return;
            case '.':
            case '#':
              r.push_back(x); r.push_back(x); return;
          }
        });
        _data.push_back(r);
        if (auto pos = line.find('@'); pos != std::string::npos) {
          p.set(nb, pos*2);
        }
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
  template<typename iterator>
  auto find_hdir(iterator it) -> bool {
    auto next = ++it;
    if (next == std::default_sentinel)
      return false;
    if (*next == '.') {
      std::swap(*it, *next);
      return true;
    } else if (*next == '#') {
      return false;
    } else if (find_hdir(next)) {
      std::swap(*it, *next);
      return true;
    }
    return false;
  }
  auto find_free(iterator &p, char d) {
    switch (d) {
      case '>': find_hdir(p.hiterator()); return;
      case '<': find_hdir(p.rhiterator()); return;
    }
    return;
  }
  auto move(std::istream &f) {
    std::string l; 
    while (std::getline(f, l) && !f.eof()) {
      if (l.empty())
        continue;
      for (auto dir: l) {
        find_free(p, dir);
        // auto fpos = find_free(dir, p);
        // if (fpos != end()) {
        //   auto npos = std::invoke(fctdir(dir), p);
        //   if (fpos != npos)
        //     std::swap(*npos, *fpos);
        //   p = npos;
        // }
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
  grid.print();
  // grid.move(std::cin);
  // std::cout << grid.gps_sum() << std::endl;
}

#define _GRID_H_IMPL
#include "grid.h"
