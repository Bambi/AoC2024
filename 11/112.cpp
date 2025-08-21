#include <iostream>
#include <unordered_map>
#include <bits/stdc++.h>
#include <string>
#include <utility>

using mark_t = ulong;
using stones_t = std::unordered_map<mark_t, ulong>;
inline auto digits(ulong n) -> unsigned { return floor(std::log10(n) + 1); }


auto parse(std::istream &f, stones_t &s) {
  mark_t stone{};
  while (f >> stone) {
    s[stone] = 1;
  }
}

auto print(const stones_t &l) {
  for (const auto &s: l)
    std::cout << s.first << '(' << s.second << ") ";
  std::cout << std::endl;
}

inline auto insert(stones_t &s, ulong mark, ulong fact) {
  s[mark] = s[mark] + fact;
}

auto blink(stones_t *stones) -> stones_t* {
  auto *newstones = new stones_t;
  for (auto &s: *stones) {
    if (s.first == 0) {
      insert(*newstones, 1, s.second);
    }
    else if (digits(s.first)%2 == 0) {
      std::string str = std::to_string(s.first);
      std::string s1 = str.substr(0, str.size()/2);
      std::string s2 = str.substr(str.size()/2, str.size()/2);
      insert(*newstones, std::stoi(s1), s.second);
      insert(*newstones, std::stoi(s2), s.second);
    } else
      insert(*newstones, s.first*2024, s.second);
  }
  delete stones;
  return newstones;
}

auto main() -> int {
  auto *stones = new stones_t;
  ulong res{};
  parse(std::cin, *stones);
  for (const uint i : std::views::iota(0, 75))
    stones = blink(stones);
  for (auto &stone: *stones)
    res += stone.second;
  std::cout << res << std::endl;
}
