// #include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <algorithm>
// #include <string>
#include <cstdio>
#include <vector>
#include <set>

using page_t = unsigned;
using update_t = std::vector<page_t>;
using updatel_t = std::vector<update_t>;

struct rule_t {
  page_t first, second;
};
using rules_t = std::vector<rule_t>;
struct rule_list_t {
  rules_t rules;

  // get all pages that must be before p
  auto get_before(page_t p) -> std::set<page_t> {
    std::set<page_t> res;
    for (auto r: rules) {
      if (r.first == p)
        res.insert(r.second);
    }
    return res;
  }
};

auto parse_rules(rules_t &rules) {
  for (std::string line; std::getline(std::cin, line);) {
    if (line.size() == 0) {
      return;
    }
    unsigned a, b;
    std::istringstream iss(line);
    std::string rule;
    std::getline(iss, rule, '|');
    a = std::stoul(rule);
    std::getline(iss, rule);
    b = std::stoul(rule);
    rules.push_back(rule_t(a, b));
  }
}

auto parse_updates(updatel_t &updates) {
  for (std::string line; std::getline(std::cin, line);) {
    std::istringstream iss(line);
    std::string spage;
    update_t update;
    while (std::getline(iss, spage, ',')) {
      page_t page = std::stoul(spage);
      update.push_back(page);
    }
    updates.push_back(update);
  }
}

auto get_middle(update_t &update) -> page_t {
  if (update.size() % 2 == 0) {
    std::cerr << "update with an even number of pages!" << std::endl;
    std::exit(1);
  }
  return update[update.size()/2];
}

auto main() -> int {
  rule_list_t rules;
  updatel_t updates;
  parse_rules(rules.rules);
  parse_updates(updates);
  unsigned res1{};

  // check updates
  for (update_t &upd: updates) {
    if (upd.size() < 2)
      continue;
    std::set<page_t> prev;
    prev.insert(upd[0]);
    bool error{false};
    for (auto it = std::next(std::begin(upd)) ; it!=std::end(upd) ; ++it) {
      // get all pages that must be after current page
      std::set<page_t> after = rules.get_before(*it);
      std::set<page_t> intersect;
      std::ranges::set_intersection(prev, after, std::inserter(intersect, std::end(intersect)));
      if (intersect.size() != 0) {
        error = true;
        break;
      }
      prev.insert(*it);
    }
    if (!error)
      res1 += get_middle(upd);
  }
  std::cout << res1 << std::endl;
}
