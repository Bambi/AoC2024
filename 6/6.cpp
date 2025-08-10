#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

struct pos_t {
  size_t l{}, c{}; // indexes: from 0 to size-1
};
auto operator<<(std::ostream &out, pos_t const& p) -> std::ostream& {
  out << '[' << p.l << ',' << p.c << ']';
  return out;
}

struct dir_t {
  static const std::array<std::function<pos_t (pos_t)>, 4> advance;
  enum Dir { N, E, S, W } dir{Dir::N};

  auto rotate() {
    dir = static_cast<Dir>((dir + 1) % 4);
  }
};
const std::array<std::function<pos_t (pos_t p)>, 4> dir_t::advance = {
  [](pos_t p){ p.l -= 1; return p; }, // N
  [](pos_t p){ p.c += 1; return p; }, // E
  [](pos_t p){ p.l += 1; return p; }, // S
  [](pos_t p){ p.c -= 1; return p; }, // W
};

struct grid_t {
  std::vector<std::string> grid;
  pos_t pos;
  dir_t dir;

  auto parse() {
    for (std::string line; std::getline(std::cin, line);) {
      grid.push_back(line);
      size_t start = line.find('^');
      if (start != std::string::npos) {
        pos = pos_t(grid.size()-1, start);
      }
    }
  }

  auto valid(pos_t &p) -> bool {
    return p.l < grid.size() && p.c < grid[0].size(); 
  }

  auto set_visited(pos_t &p) {
    pos = p;
    grid[pos.l][pos.c] = 'X';
  }

  auto get_visited() -> size_t {
    size_t res{};
    for (auto ln: grid) {
      res += std::ranges::count(ln, 'X');
    }
    return res;
  }
  
  auto move() -> size_t {
    set_visited(pos); // mark starting point
    while(valid(pos)) {
      auto newpos = dir_t::advance[dir.dir](pos);
      if (!valid(newpos)) {
        // exit!
        break;
      } else {
        if (grid[newpos.l][newpos.c] == '#') {
          dir.rotate();
        } else {
          // move one step
          set_visited(newpos);
        }
      }
    }
    return get_visited();
  }
};

auto main() -> int {
  grid_t grd;
  grd.parse();
  // std::cout << "starting pos " << grd.pos << std::endl;
  std::cout << grd.move() << std::endl;
}
