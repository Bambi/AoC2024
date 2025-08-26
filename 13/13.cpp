#include <iostream>
#include <sstream>

const ulong SHIFT = 10'000'000'000'000u;

using param_t = std::pair<long,long>;
auto operator<<(std::ostream &out, param_t const& p) -> std::ostream& {
  out << '[' << p.first << ',' << p.second << ']';
  return out;
}

auto parse_pair(param_t &res, char tok, const std::string &s) {
  std::string str;
  std::istringstream iss(s);
  std::getline(iss >> std::ws, str, tok);
  if (iss.eof()) return false;
  iss >> res.first;
  std::getline(iss >> std::ws, str, tok);
  if (iss.eof()) return false;
  iss >> res.second;
  return true;
}

auto parse(std::istream &f, param_t &A, param_t &B, param_t &P) -> bool {
  std::string line;
  while (std::getline(f, line) && line.empty() && !f.eof()) ; // read empty lines
  if (!parse_pair(A, '+', line)) return false;
  std::getline(f, line);
  if (!parse_pair(B, '+', line)) return false;
  std::getline(f, line);
  if (!parse_pair(P, '=', line)) return false;
  return true;
}

auto main() -> int {
  param_t A, B, P;
  unsigned cost1{};
  ulong cost2{};
  std::cout.setf(std::ios::fixed);
  while (parse(std::cin, A, B, P)) {
    for (ulong a=1; a<=std::min(P.first/A.first, P.second/A.second); ++a) {
      for (ulong b=1; b<=std::min(P.first/B.first, P.second/B.second); ++b) {
        if (a*A.first + b*B.first == P.first && a*A.second + b*B.second == P.second)
          cost1 += 3*a + b;
      }
    }
    param_t SP = { P.first + SHIFT, P.second + SHIFT };
    double a = double((SP.first*B.second)-(B.first*SP.second)) / double((A.first*B.second)-(A.second*B.first));
    double b = double(SP.second-A.second*a) / B.second;
    if (ulong(a) == a && ulong(b) == b)
      cost2 += 3*a + b;
  }
  std::cout << cost1 << ' ' << cost2 << std::endl;
}
