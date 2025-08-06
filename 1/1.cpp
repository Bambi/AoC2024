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

int main() {
  auto left = loc_c();
  auto right = loc_c();

  parsing(left, right);  

  sort(left.begin(), left.end());
  sort(right.begin(), right.end());

  unsigned distance{};
  unsigned similitary{};

  for (unsigned i=0 ; i<left.size() ; i++) {
    unsigned nb_elements{};
    distance += std::abs(static_cast<int>(left[i]) - static_cast<int>(right[i]));
    std::for_each(right.begin(), right.end(), [&](unsigned n) { if (n == left[i]) nb_elements += 1; });
    similitary += left[i] * nb_elements;
  }

  std::cout << "Distance: " << distance << std::endl;
  std::cout << "Similitary score: " << similitary << std::endl;
  return 0;
}
