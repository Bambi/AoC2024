#include <iostream>
#include <forward_list>
#include <iterator>
#include <bits/stdc++.h>
#include <string>

using mark_t = ulong;
using stones_t = std::forward_list<mark_t>;
inline auto digits(mark_t n) -> unsigned { return floor(std::log10(n) + 1); }

auto parse(std::istream &f, stones_t &s) {
  std::front_insert_iterator<stones_t> it(s);
  mark_t stone{};
  while (f >> stone) {
    *it = stone;
  }
  s.reverse();
}

auto print(const stones_t &l) {
  std::ostream_iterator<mark_t> out(std::cout, " ");
  std::ranges::copy(l, out);
  std::cout << std::endl;
}

auto blink(stones_t &stones) {
  for (auto s = stones.begin(); s != stones.end(); ++s) {
    if (*s == 0)
      *s = 1;
    else if (digits(*s)%2 == 0) {
      std::string str = std::to_string(*s);
      std::string s1 = str.substr(0, str.size()/2);
      std::string s2 = str.substr(str.size()/2, str.size()/2);
      *s = std::stoi(s1);
      s = stones.insert_after(s, std::stoi(s2));
    } else
      *s = *s * 2024;
  }
}

auto main() -> int {
  stones_t stones;
  parse(std::cin, stones);
  for (const uint i : std::views::iota(0, 25))
    blink(stones);
  std::cout << std::distance(stones.cbegin(), stones.cend()) << std::endl;
}
