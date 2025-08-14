#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

auto parse_line(const std::string l, std::vector<size_t> &v) -> size_t {
  size_t res{};
  std::string str;
  std::istringstream iss(l);
  std::getline(iss, str, ':');
  res = std::stoul(str);
  iss.ignore(1); // ignore the space after the ':'
  while (std::getline(iss, str, ' ')) {
    std::istringstream i(str);
    size_t n;
    i >> n;
    v.push_back(n);
  }
  return res;
}

auto compute(size_t v, std::vector<size_t> &pr, std::vector<size_t> &nr) {
  for (auto val: pr) {
    auto val1 = v + val;
    auto val2 = v * val;
    nr.push_back(val1);
    nr.push_back(val2);
  }
}

auto main() -> int {
  size_t res1{};
  for (std::string line; std::getline(std::cin, line);) {
    std::vector<size_t> eqvals;
    auto goal = parse_line(line, eqvals);
    if (eqvals.size() < 2)
      continue;
    std::vector<size_t> computedv;
    computedv.push_back(eqvals[0]);
    for (size_t idx=1 ; idx<eqvals.size() ; idx++) {
      std::vector<size_t> newv;
      compute(eqvals[idx], computedv, newv);
      computedv = newv;
    }
    if (std::ranges::find(computedv, goal) != computedv.end()) {
      res1 += goal;
    }
  }
  std::cout << res1 << std::endl;
}
