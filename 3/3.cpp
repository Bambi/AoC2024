#include <cstddef>
#include <iostream>
#include <sstream>

void process_memory(const std::string &mem) {
  unsigned res{};
  size_t mul, pos{};
  while ((mul = mem.find("mul(", pos)) != std::string::npos) {
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
    res += a*b;
  }
  std::cout << res << std::endl;
}

auto main() -> int {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  process_memory(buffer.str());
}
