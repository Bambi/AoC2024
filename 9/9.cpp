#include <cstddef>
#include <iostream>
#include <vector>

using block_t = std::vector<unsigned>;

auto expand(unsigned id, ushort nb, block_t &out) {
  while (nb > 0) {
    out.push_back(id);
    nb--;
  }
}

auto parse(block_t &blk) {
  unsigned id{}, idx{};
  char c;
  while (std::cin.get(c)) {
    if (c < '0' || c > '9')
      continue;
    if (idx%2)
      // free spaces are stored with id=0
      expand(0, c-'0', blk);
    else {
      // id are stored 'id+1'
      expand(id+1, c-'0', blk);
      id++;
    }
    idx++;
  }
}

// find the first free block, return blk.size() if not found
auto ffb(unsigned last, const block_t &blk) -> unsigned {
  for ( ; last<blk.size() ; last++) {
    if (blk[last] == 0)
      return last;
  }
  return blk.size();
}

auto defrag(block_t &blk) {
  unsigned eb{}; // empty block idx
  for (unsigned b = blk.size(); b-- > 0; ) {
    if (blk[b] != 0) {
      unsigned x = ffb(eb, blk);
      if (x > b || x == blk.size())
        return; // no more free blks
      std::swap(blk[b], blk[x]);
      eb = x;
    }
  }
}

auto checksum(const block_t &blk) -> size_t {
  size_t res{};
  for (unsigned i=0 ; i<blk.size() ; i++) {
    if (blk[i] == 0)
      break;
    res += (blk[i]-1) * i;
  }
  return res;
}

auto print(const block_t &blk) {
  for (auto b: blk) {
    if (b == 0)
      std::cout << '.';
    else if (b < 11)
      std::cout << b-1;
    else
      std::cout << '?';
  }
  std::cout << std::endl;
}

auto main() -> int {
  block_t blocks{};
  parse(blocks);
  defrag(blocks);
  std::cout << checksum(blocks) << std::endl;
}
