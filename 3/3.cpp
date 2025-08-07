#include <cstddef>
#include <iostream>
#include <sstream>
#include <cstring>
#include <array>

std::array<const char*, 2> DODONT{{ "do()", "don't()" }};

auto dostatus(const std::string &mem, bool status) -> bool {
  size_t dodont, pos{};
  while ((dodont = mem.find(DODONT[status], pos)) != std::string::npos) {
    pos = dodont+strlen(DODONT[status]);
    status = status ? false : true;
  }
  return status;
}

void process_memory(const std::string &mem) {
  unsigned res1{}, res2{};
  bool enabled{true};
  size_t mul, pos{};
  while ((mul = mem.find("mul(", pos)) != std::string::npos) {
    // find if we have do/dont before the mul()
    enabled = dostatus(mem.substr(pos, mul-pos), enabled);
    pos = mul+4;
    size_t digits;
    unsigned long a = std::stoul(mem.substr(pos, 3), &digits);
    if (a == 0 || digits == 0)
      continue;
    pos += digits;
    if (mem[pos] != ',')
      continue;
    pos += 1;
    unsigned long b = std::stoul(mem.substr(pos, 3), &digits);
    if (b == 0 || digits == 0)
      continue;
    pos += digits;
    if (mem[pos] != ')')
      continue;
    pos += 1;
    if (enabled)
      res2 += a*b;
    res1 += a*b;
  }
  std::cout << res1 << " " << res2 << std::endl;
}

auto main() -> int {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  process_memory(buffer.str());
}
