#include <iostream>
#include <sys/types.h>
#include <set>
#include <algorithm>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  auto parse(std::istream &f) {
    for (std::string line; std::getline(f, line);) {
      _data.emplace_back(line.begin(), line.end());
    }
  }
  auto path(std::set<size_t> &reach, unsigned &npath, grid_t::iterator p, char level) -> void {
    if (p != end() && *p == level) {
      if (level == '9') {
        reach.insert(p._idx);
        npath++;
      } else {
        path(reach, npath, p.N(), level+1);
        path(reach, npath, p.E(), level+1);
        path(reach, npath, p.S(), level+1);
        path(reach, npath, p.W(), level+1);
      }
    }
  }
};

auto main() -> int {
  unsigned reachable{}, nbpath{};
  grid_t grid;
  grid.parse(std::cin);
  auto it = grid.begin();
  for (auto it = grid.begin(); (it=std::find(it, grid.end(), '0')) != grid.end(); ++it) {
    std::set<size_t> trails;
    grid.path(trails, nbpath, it, '0');
    reachable += trails.size();
  }
  std::cout << reachable << ' ' << nbpath << std::endl;
}
