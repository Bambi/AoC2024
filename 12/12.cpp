#include <iostream>
#include <sys/types.h>
#include <vector>
#include <algorithm>

struct plot_t { char plot; ushort region; }; // region 0 is undefined region
struct region_t { ushort fences; ushort surface; };
auto operator<<(std::ostream &out, region_t const& r) -> std::ostream& {
  out << '[' << r.surface << ',' << r.fences << ']';
  return out;
}

struct map_t {
  std::vector< std::vector<plot_t>> map;
  std::vector<region_t> reg;
  std::vector<ushort> mr2r; // map region to region idx;

  auto parse(std::istream &f) {
    for (std::string line; std::getline(f, line);) {
      std::vector<plot_t> l;
      for (auto c: line) {
        l.push_back({.plot=c, .region=0});
      }
      map.push_back(l);
    }
  }
  [[nodiscard]] auto valid(ushort l, ushort c) const -> bool {
    return l<map.size() && c<map[0].size();
  }
  [[nodiscard]] auto fences(ushort l, ushort c) const -> ushort {
    ushort res{};
    const plot_t &plot = map[l][c];
    auto fct = [&,this](ushort l, ushort c) -> ushort {
      return (valid(l, c) && map[l][c].plot == plot.plot) ? 0 : 1;
    };
    res += fct(l-1, c);   // N
    res += fct(l,   c+1); // E
    res += fct(l+1, c);   // S
    res += fct(l,   c-1); // W
    return res;
  }
  // attributes a region id for the plot or 0 if new region
  auto region(ushort l, ushort c) -> ushort {
    ushort r1{}, r2{};
    if (valid(l-1, c) && map[l-1][c].plot == map[l][c].plot && map[l-1][c].region)
      r1 = mr2r[ map[l-1][c].region ];
    if (valid(l, c-1) && map[l][c-1].plot == map[l][c].plot && map[l][c-1].region)
      r2 = mr2r[ map[l][c-1].region ];
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
    for (ushort l=0; l<map.size(); ++l) {
      for (ushort c=0; c<map[0].size(); c++) {
        // region & surface
        auto r = region(l, c);
        if (r) {
          map[l][c].region = r;
          reg[r].surface++;
        } else {
          r = reg.size();
          reg.push_back({.fences=0, .surface=1});
          map[l][c].region = mr2r.size();
          mr2r.push_back(r);
        }
        // fences
        reg[r].fences += fences(l, c);
      }
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
  map.parse(std::cin);
  map.fencing();
  std::cout << map.cost() << std::endl;
}
