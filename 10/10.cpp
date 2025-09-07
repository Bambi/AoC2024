#include <iostream>
#include <sys/types.h>
#include <set>
#include <algorithm>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  auto parse(std::istream &f) {
    for (std::string line; std::getline(f, line);) {
      add_data(line);
    }
  }
  auto path(std::set<size_t> &reach, unsigned &npath, size_t p, char level) -> void {
    if (p != aoc::npos && (*this)(p) == level) {
      if (level == '9') {
        reach.insert(p);
        npath++;
      } else {
        path(reach, npath, peek(p, aoc::dir::N), level+1);
        path(reach, npath, peek(p, aoc::dir::E), level+1);
        path(reach, npath, peek(p, aoc::dir::S), level+1);
        path(reach, npath, peek(p, aoc::dir::W), level+1);
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
    grid.path(trails, nbpath, it.idx_, '0');
    reachable += trails.size();
  }
  std::cout << reachable << ' ' << nbpath << std::endl;
}
