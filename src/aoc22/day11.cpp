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
  std::getline(*input, line);
  trim(&line);
  auto parts = split(line, ' ')[3];
  auto divisor = std::stoi(split(line, ' ')[3]);

  std::getline(*input, line);
  trim(&line);
  std::size_t true_monkey = std::stoul(split(line, ' ')[5]);

  std::getline(*input, line);
  trim(&line);
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
  std::getline(*input, line);
  trim(&line);
  monkey.items = parse_items(line);
  // Operation
  std::getline(*input, line);
  trim(&line);
  monkey.operation = parse_operation(line);
  // Test
  auto [tester, divisor] = parse_test(input);
  monkey.test = tester;
  monkey.divisor = divisor;
  return monkey;
}

std::vector<Monkey> read_monkeys(std::istream *input) {
  std::vector<Monkey> monkeys;
  std::string _ignore;
  while (!input->eof()) {
    monkeys.emplace_back(read_monkey(input));
    std::getline(*input, _ignore);
  }
  return monkeys;
}

} // namespace

u_int64_t day11_1(std::istream *input_file) {
  constexpr std::size_t num_rounds{20};

  auto monkeys = read_monkeys(input_file);
  std::vector<std::size_t> inspection_count(monkeys.size());
  for (auto i = 0U; i < num_rounds; ++i) {
    for (auto m_idx = 0U; m_idx < monkeys.size(); ++m_idx) {
      while (monkeys[m_idx].items.size() > 0) {
        auto item_wl =
            monkeys[m_idx].operation(monkeys[m_idx].items.front()) / 3;
        inspection_count[m_idx]++;
        monkeys[m_idx].items.pop_front();
        auto throw_to_monkey = monkeys[m_idx].test(item_wl);
        monkeys[throw_to_monkey].items.push_back(item_wl);
      }
    }
  }
  std::partial_sort(inspection_count.begin(),
                    std::next(inspection_count.begin(), 2),
                    inspection_count.end(), std::greater<>());
  return std::accumulate(inspection_count.begin(),
                         std::next(inspection_count.begin(), 2), 1,
                         std::multiplies<>());
}

u_int64_t day11_2(std::istream *input_file) {
  constexpr std::size_t num_rounds{10000};

  auto monkeys = read_monkeys(input_file);
  std::vector<u_int64_t> inspection_count(monkeys.size());
  u_int64_t divisor_prod = std::accumulate(
      monkeys.begin(), monkeys.end(), 1U,
      [](const auto &acc, const auto &m) { return acc * m.divisor; });
  for (auto i = 0U; i < num_rounds; ++i) {
    for (auto m_idx = 0U; m_idx < monkeys.size(); ++m_idx) {
      while (monkeys[m_idx].items.size() > 0) {
        auto item_wl = monkeys[m_idx].operation(monkeys[m_idx].items.front());
        inspection_count[m_idx]++;
        monkeys[m_idx].items.pop_front();

        // Use the fact that every divisor is a prime number. We can take
        // the modulo of the product of all divisors to get a smaller number
        // with the same divisors.
        auto new_wl = item_wl % divisor_prod;
        auto throw_to_monkey = monkeys[m_idx].test(new_wl);
        monkeys[throw_to_monkey].items.push_back(new_wl);
      }
    }
  }
  std::partial_sort(inspection_count.begin(),
                    std::next(inspection_count.begin(), 2),
                    inspection_count.end(), std::greater<>());
  return std::accumulate(
      inspection_count.begin(), std::next(inspection_count.begin(), 2),
      static_cast<u_int64_t>(1), std::multiplies<u_int64_t>());
}
