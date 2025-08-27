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

  auto parse(std::istream &f) {
    std::pair<ushort,ushort> max;
    while (!f.eof()) {
      std::string line;
      std::getline(std::cin, line);
      if (!line.empty()) {
        robot_t r;
        r.parse(line);
        robots.push_back(r);
        if (r.p.r > max.first) max.first = r.p.r;
        if (r.p.c > max.second) max.second = r.p.c;
      }
    }
    max.first += 1;
    max.second += 1;
    return max;
  }
  auto advance(ushort seconds, ushort rmax, ushort cmax) {
    for (auto &r: robots) {
      r.p.r = (r.p.r + r.v.r * seconds) % rmax;
      if (r.p.r < 0) r.p.r += rmax;
      r.p.c = (r.p.c + r.v.c * seconds) % cmax;
      if (r.p.c < 0) r.p.c += cmax;
    }
  }
  auto print() {
    for (auto r: robots) {
      std::cout << r.p << ' ';
    }
    std::cout << std::endl;
  }
  auto count(ushort mr, ushort mc) {
    ushort c1{}, c2{}, c3{}, c4{};
    for (auto r: robots) {
      if (r.p.r < mr) {
        if (r.p.c < mc)
          c1 += 1;
        else if (r.p.c > mc)
          c2 += 1;
      } else if (r.p.r > mr) {
        if (r.p.c < mc)
          c3 += 1;
        else if (r.p.c > mc)
          c4 += 1;
      }
    }
    return c1*c2*c3*c4;
  }
};

auto main() -> int {
  robots_t r;
  auto max = r.parse(std::cin);
  r.advance(100, max.first, max.second);
  std::cout << r.count(max.first/2, max.second/2) << std::endl;
}
