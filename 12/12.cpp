#include <cstddef>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include "grid.h"

struct plot_t { char plot; ushort region; }; // region 0 is undefined region
struct region_t { ushort fences; ushort corners; ushort surface; };
auto operator<<(std::ostream &out, region_t const& r) -> std::ostream& {
  out << '[' << r.surface << ',' << r.fences << ',' << r.corners << ']';
  return out;
}

struct garden_t : public aoc::grid<plot_t> {
  auto parse(std::istream &f) {
    for (std::string line; std::getline(f, line);) {
      for (auto c: line) {
        add_data({.plot=c, .region=0});
      }
      add_row();
    }
  }
  [[nodiscard]] auto fences(size_t p) {
    std::pair<ushort,ushort> res{}; // first: fences, second: corners
    auto cplot = (*this)(p).plot;
    auto ffence = [&,this](size_t p) -> bool {
      return (p != aoc::npos && (*this)(p).plot == cplot) ? false : true;
    };
    auto f1 = ffence(peek(p, aoc::dir::N));
    auto f2 = ffence(peek(p, aoc::dir::E));
    auto f3 = ffence(peek(p, aoc::dir::S));
    auto f4 = ffence(peek(p, aoc::dir::W));
    // count a corner between 2 coninuous fences
    if (f1 && f2) res.second++;
    if (f2 && f3) res.second++;
    if (f3 && f4) res.second++;
    if (f4 && f1) res.second++;
    res.first = f1 + f2 + f3 + f4;
    // open corners
    auto ocorner = [p, cplot, this](aoc::dir d1, aoc::dir d2) -> bool {
      auto p1 = peek(p, d1);
      auto p2 = peek(p, d2);
      if (p1 == aoc::npos || p2 == aoc::npos) return false;
      auto p3 = peek(p1, d2);
      return (*this)(p1).plot == cplot && (*this)(p2).plot == cplot && (*this)(p3).plot != cplot;
    };
    if (ocorner(aoc::dir::N, aoc::dir::E))
        res.second++;
    if (ocorner(aoc::dir::E, aoc::dir::S))
        res.second++;
    if (ocorner(aoc::dir::S, aoc::dir::W))
        res.second++;
    if (ocorner(aoc::dir::W, aoc::dir::N))
        res.second++;
    return res;
  }
};

struct map_t {
  garden_t garden;
  std::vector<region_t> reg;
  std::vector<ushort> mr2r; // map region to region idx;

  // attributes a region id for the plot or 0 if new region
  auto region(size_t p) -> ushort {
    ushort r1{}, r2{};
    size_t N_plot = garden.peek(p, aoc::dir::N);
    size_t W_plot = garden.peek(p, aoc::dir::W);
    if (N_plot != aoc::npos && garden(N_plot).plot == garden(p).plot && garden(N_plot).region)
      r1 = mr2r[ garden(N_plot).region ];
    if (W_plot != aoc::npos && garden(W_plot).plot == garden(p).plot && garden(W_plot).region)
      r2 = mr2r[ garden(W_plot).region ];
    if (r1 == r2)
      return r1;
    else if (r1 == 0 || r2 == 0)
      return r1 ? r1 : r2;
    else {
      merge(r1, r2);
      // update all indexes
      std::ranges::for_each(mr2r, [r1,r2](ushort &x){ if (x==r2) x=r1; });
      return r1;
    }
  }
  // merge r2 region into r1; leaving r2 empty
  void merge(ushort r1, ushort r2) {
    reg[r1].fences += reg[r2].fences;
    reg[r1].surface += reg[r2].surface;
    reg[r1].corners += reg[r2].corners;
    reg[r2] = {.fences=0, .corners=0, .surface=0};
  }
  auto fencing() {
    // add the undefined region (0 should not be used)
    reg.push_back({});
    mr2r.push_back({});    
    for (auto pit = garden.begin(); pit != garden.end(); ++pit) {
      // region & surface
      auto r = region(pit.idx());
      if (r) {
        pit->region = r;
        reg[r].surface++;
      } else {
        r = reg.size();
        reg.push_back({.fences=0, .corners=0, .surface=1});
        pit->region = mr2r.size();
        mr2r.push_back(r);
      }
      // fences
      auto counts = garden.fences(pit.idx());
      reg[r].fences += counts.first;
      reg[r].corners += counts.second;
    }
  }
  auto cost(bool bulk) {
    unsigned res{};
    for (auto r: reg) {
      if (r.surface)
        res += r.surface * (bulk ? r.corners : r.fences);
    }
    return res;
  }
};

auto main() -> int {
  map_t map;
  map.garden.parse(std::cin);
  map.fencing();
  std::cout << map.cost(false) << ' ' << map.cost(true) << std::endl;
}
