#include <iostream>
#include <sstream>
#include <vector>
#include "position.h"

struct robot_t {
  pos_t<short> p, v;

  auto parse(const std::string &s) {
    std::istringstream iss(s);
    auto parse_pos = [&](pos_t<short> &p){
      std::string str;
      std::getline(iss >> std::ws, str, '=');
      iss >> p.c;
      std::getline(iss >> std::ws, str, ',');
      iss >> p.r;
    };
    parse_pos(p);
    parse_pos(v);
  }
};

struct robots_t {
  std::vector<robot_t> robots;
  ushort rmax{}, cmax{};

  auto parse(std::istream &f) {
    while (!f.eof()) {
      std::string line;
      std::getline(std::cin, line);
      if (!line.empty()) {
        robot_t r;
        r.parse(line);
        robots.push_back(r);
        if (r.p.r > rmax) rmax = r.p.r;
        if (r.p.c > cmax) cmax = r.p.c;
      }
    }
    rmax += 1;
    cmax += 1;
  }
  auto advance(ushort seconds) {
    for (auto &r: robots) {
      r.p.r = (r.p.r + r.v.r * seconds) % rmax;
      if (r.p.r < 0) r.p.r += rmax;
      r.p.c = (r.p.c + r.v.c * seconds) % cmax;
      if (r.p.c < 0) r.p.c += cmax;
    }
  }
  auto print() {
    std::vector<bool> map(rmax * cmax, false);
    for (auto r: robots) {
      map[r.p.r*cmax + r.p.c] = true;
    }
    // check we have 'seq' continuous robots on a line
    auto check = [=, this](ushort seq) -> bool {
      for (ushort i=0; i<rmax; i++) {
        ushort seq_count{};
        for (ushort j=0; j<cmax; j++) {
          if (map[i*cmax + j])
            seq_count++;
          else
            seq_count = 0;
          if (seq_count == seq)
            return true;
        }
      }
      return false;
    };
    if (check(10)) {
      for (ushort i=0; i<map.size(); ++i) {
        if (i%cmax == 0) std::cout << std::endl;
        std::cout << (map[i] ? '*':'.');
      }
      std::cout << std::endl;
      return true;
    }
    return false;
  }
  auto count() {
    ushort c1{}, c2{}, c3{}, c4{};
    ushort rmax = this->rmax / 2;
    ushort cmax = this->cmax / 2;
    for (auto r: robots) {
      if (r.p.r < rmax) {
        if (r.p.c < cmax)
          c1 += 1;
        else if (r.p.c > cmax)
          c2 += 1;
      } else if (r.p.r > rmax) {
        if (r.p.c < cmax)
          c3 += 1;
        else if (r.p.c > cmax)
          c4 += 1;
      }
    }
    return c1*c2*c3*c4;
  }
};

auto main() -> int {
  robots_t r;
  r.parse(std::cin);
  r.advance(100);
  std::cout << r.count();
  if (r.cmax > 100) {
    for (unsigned i=0; i<1'000'000; ++i) {
      r.advance(1);
      if (r.print()) {
        std::cout << i+101;
        break;
      }
    }
  }
  std::cout << std::endl;
}
