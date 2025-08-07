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

// true: all pairs in list have good and same tendency
// ignore: index to ignore, set to -1 to not ignore any value
auto good_tendendy(const std::vector<unsigned> &levels, int ignore) -> bool {
  unsigned first, second;
  if (ignore == 0) {
    first = 1; second = 2;
  } else if (ignore == 1) {
    first = 0; second = 2;
  } else {
    first = 0; second = 1;
  }
  auto first_tend = tendency(levels[first], levels[second]);
  if (first_tend == Tendency::Wrong)
    return false;
  unsigned it{};
  unsigned prev = levels[first];
  for (it=second ; it != levels.size() ; ++it) {
    if (it == ignore) continue;
    // finish if tendency is changing
    if (tendency(prev, levels[it]) != first_tend)
      break;
    prev = levels[it];
  }
  // all good if we got till the end of the levels list
  if (it == levels.size())
    return true;
  return false;
}

enum class Status: int {
  Safe, Dampened, Unsafe
};
auto process_level(const std::string line) -> Status {
    std::istringstream iss(line);
    std::string elt;
    // store levels in a vector list
    std::vector<unsigned> levels;
    while (std::getline(iss, elt, ' ')) {
      levels.push_back(std::stoi(elt));
    }
  
    if (levels.size() < 2)
      return Status::Unsafe;

    if (good_tendendy(levels, -1))
      return Status::Safe;
    else {
      for (unsigned i=0 ; i<levels.size() ; ++i)
        if (good_tendendy(levels, i))
          return Status::Dampened;
      return Status::Unsafe;
    }
}

auto main() -> int {
  unsigned nb_safe{}, nb_damp{};

  for (std::string line; std::getline(std::cin, line);) {
    switch (process_level(line)) {
      case Status::Safe:
        nb_safe += 1;
        break;
      case Status::Dampened:
        nb_damp += 1;
        break;
      case Status::Unsafe:
        break;
    }
  }
  std::cout << nb_safe << " " << (nb_safe+nb_damp) << std::endl;
  return 0;
}
