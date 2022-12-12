#include "aoc22/aoc.h"

#include <functional>
#include <list>
#include <numeric>

namespace {
struct Monkey {
  std::list<u_int64_t> items;
  std::function<u_int64_t(u_int64_t)> operation;
  std::function<std::size_t(u_int64_t)> test;
  u_int64_t divisor;
};

void getline_with_trim(std::istream *input, std::string *line) {
  std::getline(*input, *line);
  trim(line);
}

std::list<u_int64_t> parse_items(const std::string &line) {
  auto str_items = split(split(line, ':')[1], ',');
  std::list<u_int64_t> items(str_items.size());
  std::transform(str_items.begin(), str_items.end(), items.begin(), [](auto x) {
    trim(&x);
    return std::stoi(x);
  });
  return items;
}

std::function<u_int64_t(u_int64_t)> parse_operation(const std::string &line) {
  auto eqn_parts = split(split(line, ':')[1], ' ');
  std::function<u_int64_t(u_int64_t, u_int64_t)> op;
  switch (eqn_parts[4][0]) {
  case '*':
    op = std::multiplies<u_int64_t>();
    break;
  case '+':
    op = std::plus<u_int64_t>();
    break;
  default:
    throw std::runtime_error("Unrecognised operation");
  }
  if (eqn_parts[5] == "old") {
    return [op](const u_int64_t old) { return op(old, old); };
  }
  auto operand = std::stoi(eqn_parts[5]);
  return [op, operand](const u_int64_t old) { return op(old, operand); };
}

std::pair<std::function<std::size_t(u_int64_t)>, u_int64_t>
parse_test(std::istream *input) {
  std::string line;
  getline_with_trim(input, &line);
  auto parts = split(line, ' ')[3];
  auto divisor = std::stoi(split(line, ' ')[3]);
  getline_with_trim(input, &line);
  std::size_t true_monkey = std::stoul(split(line, ' ')[5]);
  getline_with_trim(input, &line);
  std::size_t false_monkey = std::stoul(split(line, ' ')[5]);
  auto test_func = [divisor, true_monkey, false_monkey](const u_int64_t x) {
    if (x % divisor == 0) {
      return true_monkey;
    } else {
      return false_monkey;
    }
  };
  return {test_func, divisor};
}

Monkey read_monkey(std::istream *input) {
  std::string line;
  Monkey monkey;
  // Ignore first line, giving monkey no.
  std::getline(*input, line);
  // Starting items
  getline_with_trim(input, &line);
  monkey.items = parse_items(line);
  // Operation
  getline_with_trim(input, &line);
  monkey.operation = parse_operation(line);
  // Test
  auto [tester, divisor] = parse_test(input);
  monkey.test = tester;
  monkey.divisor = divisor;
  return monkey;
}

std::vector<Monkey> read_monkeys(std::istream *input) {
  std::vector<Monkey> monkeys;
  std::string _empty_line_ignore;
  while (!input->eof()) {
    monkeys.emplace_back(read_monkey(input));
    std::getline(*input, _empty_line_ignore);
  }
  return monkeys;
}

u_int64_t simulate(std::vector<Monkey> &monkeys, std::size_t num_rounds,
                   std::function<u_int64_t(u_int64_t)> wl_manager) {
  std::vector<u_int64_t> inspection_count(monkeys.size());
  for (auto i = 0U; i < num_rounds; ++i) {
    for (auto m_idx = 0U; m_idx < monkeys.size(); ++m_idx) {
      while (monkeys[m_idx].items.size() > 0) {
        auto item_wl =
            wl_manager(monkeys[m_idx].operation(monkeys[m_idx].items.front()));
        inspection_count[m_idx]++;
        monkeys[m_idx].items.pop_front();
        auto throw_to = monkeys[m_idx].test(item_wl);
        monkeys[throw_to].items.push_back(item_wl);
      }
    }
  }
  std::partial_sort(inspection_count.begin(),
                    std::next(inspection_count.begin(), 2),
                    inspection_count.end(), std::greater<>());
  return inspection_count[0] * inspection_count[1];
}
} // namespace

u_int64_t day11_1(std::istream *input_file) {
  auto monkeys = read_monkeys(input_file);
  return simulate(monkeys, 20, [](auto wl) { return wl / 3; });
}

u_int64_t day11_2(std::istream *input_file) {
  auto monkeys = read_monkeys(input_file);
  // We must guard against 'worry level' integer overflows in this part.
  // Use the fact that every divisor is a prime number. We can take the
  // 'worry level' modulo the product of all the divisors, to get a smaller
  // number that still has the same set of prime divisors.
  u_int64_t divisor_prod = std::accumulate(
      monkeys.begin(), monkeys.end(), 1U,
      [](const auto &acc, const auto &m) { return acc * m.divisor; });
  auto wl_manager = [divisor_prod](auto wl) { return wl % divisor_prod; };
  return simulate(monkeys, 10000, wl_manager);
}
