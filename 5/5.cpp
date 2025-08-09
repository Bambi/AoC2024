#include <climits>
#include <cstddef>
#include <iostream>
#include <sstream>
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
  // return 0 if not found, order index+1 if found
  auto check_before(size_t idx, update_t &order) {
    int res = INT_MAX;
    // get all pages that must be after current page
    std::set<page_t> to_check = get_before(order[idx]);
    for (auto i: to_check) {
      for (size_t x=0 ; x<idx ; x++) {
        if (order[x] == i && x < res)
          res = x;
      }
    }
    return res == INT_MAX ? 0 : res+1;
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
  unsigned res1{}, res2{};

  // check updates
  for (update_t &upd: updates) {
    if (upd.size() < 2)
      continue;
    size_t i=1;
    bool error{false};
    while (i < upd.size()) {
      int err = rules.check_before(i, upd);
      if (err) {
        error = true;
        // change order list
        std::swap(upd[err-1], upd[i]); 
        i = err; // start again where we swap
      } else {
        i++;
      }
    }
    if (error)
      res2 += get_middle(upd);
    else
      res1 += get_middle(upd);
  }
  std::cout << res1 << " " << res2 << std::endl;
}
