#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <sys/types.h>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <utility>
#include <optional>
#include "grid.h"

struct grid_t : public aoc::grid<char> {
  using swapq = std::queue<std::pair<size_t, size_t>>;
  auto parse(std::istream &f) -> iterator {
    ushort nb{}, r{}, c{};
    while (!f.eof()) {
      std::string line;
      std::getline(f, line);
      if (line.empty()) {
        break;
      } else {
        auto p = line.find('@');
        if (p != std::string::npos) {
          cols_ = line.size() * 2;
          line[p] = '.';
          c = p*2;
          r = nb;
        }
        std::ranges::for_each(line, [&](char x) {
          switch (x) {
            case '@': add_data('.'); add_data('.'); return;
            case 'O': add_data('['); add_data(']'); return;
            case '.':
            case '#':
              add_data(x); add_data(x); return;
          }
        });
      }
      nb++;
    }
    return {*this, r, c};
  }
  static auto fctdir(char dir) -> enum dir {
    switch(dir) {
      case '^': return dir::N;
      case 'v': return dir::S;
      case '<': return dir::W;
      case '>': return dir::E;
    }
    std::abort();
  }
  [[nodiscard]]
  auto check_new_branch(dir_iterator it) const -> std::optional<dir_iterator> {
    if (*it == '[') {
      auto it2 = it.move_card(dir::E);
      if (*std::prev(it2) == ']')
        return std::nullopt;
      else
        return it2;
    } else if (*it == ']') {
      auto it2 = it.move_card(dir::W);
      if (*std::prev(it2) == '[')
        return std::nullopt;
      else
        return it2;
    }
    return std::nullopt;        
  }
  auto find_free(dir_iterator it, swapq& swaps) -> size_t {
    if (it == std::default_sentinel_t() || *it == '#') {
      return npos;
    } else if (*it == '.') {
      return it.idx_;
    } else {
      if (it.d_ == dir::E || it.d_ == dir::W) {
        auto res = find_free(std::next(it), swaps);
        if (res != npos) {
          swaps.emplace(it.idx_, res);
          return it.idx_;
        } else
          return npos;
      } else {
        auto res1 = find_free(std::next(it), swaps);
        if (res1 == npos)
          return npos;
        auto it2 = check_new_branch(it);
        if (it2) {
          auto res2 = find_free(std::next(*it2), swaps);
          if (res2 != npos) {
            swaps.emplace(it.idx_, res1);
            swaps.emplace((*it2).idx_, res2);
            return it.idx_;
          } else
            return npos;
        } else {
          swaps.emplace(it.idx_, res1);
          return it.idx_;
        }
      }
    }
  }
  auto find_free(size_t pos, char d) -> size_t {
    auto it = dir_iterator(*this, fctdir(d), pos);
    swapq swaps;
    // some operations might be multiple times in the queue
    // we use a set to ensure that each operation is treated only once
    std::set<std::pair<size_t,size_t>> opdone;
    if (find_free(std::next(it), swaps) != npos) {
      while (!swaps.empty()) {
        auto pair = swaps.front();
        if (!opdone.contains(pair)) {
          std::swap((*this)(pair.first), (*this)(pair.second));
          opdone.insert(pair);
        }
        swaps.pop();
      }
      return std::next(it).idx_; // advance
    }
    return it.idx_; // keep position
  }
  auto move(std::istream &f, size_t p) {
    std::string l; 
    while (std::getline(f, l) && !f.eof()) {
      if (l.empty())
        continue;
      for (auto dir: l) {
        auto nextp = find_free(p, dir);
        p = nextp;
      }
    }
  }
  auto gps_sum() -> unsigned {
    unsigned res{};
    for (auto it=begin(); it!=end(); ++it) {
      if (*it == '[')
        res += it.row()*100 + it.col();
    }
    return res;
  }
};

auto main() -> int {
  grid_t grid;
  auto p = grid.parse(std::cin);
  grid.move(std::cin, p.idx_);
  std::cout << grid.gps_sum() << std::endl;
}

#define _GRID_H_IMPL
#include "grid.h"
