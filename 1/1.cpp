#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using loc_c = std::vector<unsigned>;

void parsing(loc_c &left, loc_c &right) {
  for (std::string line; std::getline(std::cin, line);) {
    std::istringstream iss(line);
    unsigned l, r;
    iss >> l >> r;
    left.push_back(l);
    right.push_back(r);
  }
}

auto main() -> int {
  auto left = loc_c();
  auto right = loc_c();

  parsing(left, right);  

  std::ranges::sort(left);
  std::ranges::sort(right);

  unsigned distance{};
  unsigned similarity{};

  for (unsigned i=0 ; i<left.size() ; i++) {
    unsigned nb_elements{};
    distance += std::abs(static_cast<int>(left[i]) - static_cast<int>(right[i]));
    std::ranges::for_each(right, [&](unsigned n) { if (n == left[i]) nb_elements += 1; });
    similarity += left[i] * nb_elements;
  }

  std::cout << distance << " " << similarity << std::endl;
  return 0;
}
