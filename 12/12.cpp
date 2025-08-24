#include <iostream>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include "common.h"

struct plot_t { char plot; ushort region; }; // region 0 is undefined region
struct region_t { ushort fences; ushort surface; };
auto operator<<(std::ostream &out, region_t const& r) -> std::ostream& {
  out << '[' << r.surface << ',' << r.fences << ']';
  return out;
}

struct garden_t {
  std::vector< std::vector<plot_t>> plots;

  auto parse(std::istream &f) {
    for (std::string line; std::getline(f, line);) {
      std::vector<plot_t> l;
      for (auto c: line) {
        l.push_back({.plot=c, .region=0});
      }
      plots.push_back(l);
    }
  }
  [[nodiscard]] auto valid(const pos_t &p) const -> bool {
    return p.r < plots.size() && p.c < plots[0].size();
  }
  [[nodiscard]] auto plot(const pos_t &p) const -> const plot_t& { return plots[p.r][p.c]; }
  [[nodiscard]] auto plot(const pos_t &p) -> plot_t& { return plots[p.r][p.c]; }
  [[nodiscard]] auto fences(const pos_t &p) const -> ushort {
    ushort res{};
    const plot_t &plot = this->plot(p);
    auto fct = [&,this](const pos_t &p) -> ushort {
      return (valid(p) && this->plot(p).plot == plot.plot) ? 0 : 1;
    };
    res += fct(p.N());
    res += fct(p.E());
    res += fct(p.S());
    res += fct(p.W());
    return res;
  }
  [[nodiscard]] auto begin() const { return grid_iterator_t(plots[0].size());};
  [[nodiscard]] auto end() const { return grid_iterator_t(plots[0].size(), pos_t(plots.size(), 0)); };
};

struct map_t {
  garden_t garden;
  std::vector<region_t> reg;
  std::vector<ushort> mr2r; // map region to region idx;

  // attributes a region id for the plot or 0 if new region
  auto region(pos_t p) -> ushort {
    ushort r1{}, r2{};
    if (garden.valid(p.N()) && garden.plot(p.N()).plot == garden.plot(p).plot && garden.plot(p.N()).region)
      r1 = mr2r[ garden.plot(p.N()).region ];
    if (garden.valid(p.W()) && garden.plot(p.W()).plot == garden.plot(p).plot && garden.plot(p.W()).region)
      r2 = mr2r[ garden.plot(p.W()).region ];
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
    reg[r2] = {.fences=0, .surface=0};
  }
  auto fencing() {
    // add the undefined region (0 should not be used)
    reg.push_back({});
    mr2r.push_back({});    
    for (auto pit = garden.begin(); pit != garden.end(); ++pit) {
      // region & surface
      auto r = region(*pit);
      if (r) {
        garden.plot(*pit).region = r;
        reg[r].surface++;
      } else {
        r = reg.size();
        reg.push_back({.fences=0, .surface=1});
        garden.plot(*pit).region = mr2r.size();
        mr2r.push_back(r);
      }
      // fences
      reg[r].fences += garden.fences(*pit);
    }
  }
  auto cost() {
    unsigned res{};
    for (auto r: reg) {
      if (r.surface) {
        res += r.surface * r.fences;
      }
    }
    return res;
  }
};

auto main() -> int {
  map_t map;
  map.garden.parse(std::cin);
  map.fencing();
  std::cout << map.cost() << std::endl;
}
