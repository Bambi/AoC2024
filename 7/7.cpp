#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

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

auto compute1(size_t v, const std::vector<size_t> &pr, std::vector<size_t> &nr) {
  for (auto val: pr) {
    nr.push_back(v + val);
    nr.push_back(v * val);
  }
}

auto compute2(size_t v, const std::vector<size_t> &pr, std::vector<size_t> &nr) {
  for (auto val: pr) {
    nr.push_back(v + val);
    nr.push_back(v * val);
    nr.push_back(std::stol(std::to_string(val)+std::to_string(v)));
  }
}

auto calib(size_t goal, const std::vector<size_t> &eq,
           std::function<void(size_t, const std::vector<size_t>&, std::vector<size_t>&)> compute) -> size_t
{
  std::vector<size_t> computedv;
  computedv.push_back(eq[0]);
  for (size_t idx=1 ; idx<eq.size() ; idx++) {
    std::vector<size_t> newv;
    compute(eq[idx], computedv, newv);
    computedv = newv;
  }
  if (std::ranges::find(computedv, goal) != computedv.end()) {
    return goal;
  }
  return 0;
}

auto main() -> int {
  size_t res1{}, res2{};
  for (std::string line; std::getline(std::cin, line);) {
    std::vector<size_t> eqvals;
    auto goal = parse_line(line, eqvals);
    if (eqvals.size() < 2)
      continue;
    res1 += calib(goal, eqvals, compute1);
    res2 += calib(goal, eqvals, compute2);
  }
  std::cout << res1 << ' ' << res2 << std::endl;
}
