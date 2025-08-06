#include <iostream>
#include <sstream>
#include <vector>

enum class Tendency: int {
  Wrong, // either flat or varying too fast
  Growing, Lowering
};
auto tendency(unsigned a, unsigned b) {
  int tend = a - b;
  if (tend < 0 && tend > -4) return Tendency::Growing;
  if (tend > 0 && tend < 4) return Tendency::Lowering;
  return Tendency::Wrong;
}

auto process_level(std::string line) {
    std::istringstream iss(line);
    std::string elt;
    // store levels in a vector list
    std::vector<unsigned> levels;
    while (std::getline(iss, elt, ' ')) {
      levels.push_back(std::stoi(elt));
    }
  
    if (levels.size() < 2)
      return 0;

    unsigned prev = levels[0];
    // set tendency with the first 2 levels & finish if already wrong
    auto first_tend = tendency(levels[0], levels[1]);
    if (first_tend == Tendency::Wrong)
      return 0;
    unsigned it{};
    for (it=1 ; it != levels.size() ; ++it) {
      // finish if tendency is changing
      if (tendency(prev, levels[it]) != first_tend)
        break;
      prev = levels[it];
    }
    // all good if we got till the end of the levels list
    if (it == levels.size())
      return 1;
    return 0;
}

auto main() -> int {
  unsigned nb_safe{};

  for (std::string line; std::getline(std::cin, line);) {
    nb_safe += process_level(line);
  }
  std::cout << nb_safe << std::endl;
  return 0;
}
