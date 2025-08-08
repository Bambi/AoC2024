#include <cstddef>
#include <iostream>
#include <vector>
#include <functional>

using datal_t = std::vector<char>;
using data_t = std::vector<datal_t>;
const char* PATTERN = "XMAS";
const size_t PATSZ = 4;
enum Direction {
  N, S, E, W, NE, SE, SW, NW, PLOUF
};

struct Position {
private:
  size_t l, c;
  const data_t &data;
public:
  std::string str{};
  static const std::array<std::function<void (struct Position&)>, 8> directions;
  Position(size_t line, size_t col, const data_t &d) : l{line}, c{col}, data{d} {
    if (valid())
      str.push_back(data[l][c]);
  }
  void move(enum Direction dir) {
    Position::directions[static_cast<unsigned>(dir)](*this);
    if (valid())
      str.push_back(data[l][c]);
  }
  void move(enum Direction dir, size_t nb) {
    for (size_t i=0 ; i<nb; i++) {
      move(dir);
    }
  }
  auto pick(enum Direction dir) -> char {
    Position::directions[static_cast<unsigned>(dir)](*this);
    if (valid())
      return data[l][c];
    return 'X';
  }
  auto get_cross() -> std::string {
    std::string res;
    for (auto i: { NE, SE, SW, NW }) {
      Position p(*this);
      res.push_back(p.pick(i));
    }
    return res;
  }
  auto valid() -> bool {
    return l >= 0 && c >= 0 && l < data.size() && c < data[0].size();
  }
};
const std::array<std::function<void (struct Position&)>, 8> Position::directions = {
  [](struct Position &p){ p.l -=1; },
  [](struct Position &p){ p.l +=1; },
  [](struct Position &p){ p.c +=1; },
  [](struct Position &p){ p.c -=1; },
  [](struct Position &p){ Position::directions[Direction::N](p); Position::directions[Direction::E](p); },
  [](struct Position &p){ Position::directions[Direction::S](p); Position::directions[Direction::E](p); },
  [](struct Position &p){ Position::directions[Direction::S](p); Position::directions[Direction::W](p); },
  [](struct Position &p){ Position::directions[Direction::N](p); Position::directions[Direction::W](p); }
};

auto read_data(std::vector<datal_t> &data) -> size_t {
  size_t linelen{};
  for (std::string line; std::getline(std::cin, line);) {
    datal_t ln;
    for (char i: line) {
      ln.push_back(i);
    }
    data.push_back(ln);
    if (linelen == 0)
      linelen = ln.size();
    else if (linelen != ln.size()) {
      std::cerr << "input line of different sizes!" << std::endl;
      std:exit(-1);
    }
  }
  return linelen;
}

auto main() -> int {
  std::vector<datal_t> data;
  const auto cols = read_data(data);
  const auto lines = data.size();

  size_t res1{}, res2{};
  size_t l, c;
  for (l=0 ; l<lines ; l++) {
    for (c=0 ; c<cols ; c++) {
      for(unsigned d=Direction::N ; d<Direction::PLOUF ; d++) {
        Position pos(l, c, data);
        pos.move(static_cast<enum Direction>(d), PATSZ-1);
        if (pos.valid()) {
          if (pos.str == PATTERN) {
            res1 += 1;
          }
        }
      }
      if (data[l][c] == 'A') {
        Position pos(l, c, data);
        auto cross = pos.get_cross();
        if (cross == "MMSS" ||
            cross == "MSSM" ||
            cross == "SSMM" ||
            cross == "SMMS")
          res2 += 1;
      }
    }
  }
  std::cout << res1 << " " << res2 << std::endl;
}
