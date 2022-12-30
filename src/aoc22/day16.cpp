#include "aoc22/aoc.h"

#include <map>
#include <numeric>
#include <queue>
#include <unordered_set>

namespace {
using ul_bitset = std::bitset<sizeof(std::size_t)>;

std::pair<std::vector<std::size_t>, std::vector<std::vector<std::size_t>>>
read_valves(std::istream *input) {
  std::map<std::string, std::size_t> flow_rates;
  std::map<std::string, std::vector<std::string>> leads_to;
  for (std::string line; std::getline(*input, line);) {
    auto parts = split(line, ' ');
    auto valve_name = parts[1];
    auto flow_rate_str = split(parts[4], '=')[1];
    flow_rate_str.erase(flow_rate_str.end()); // remove semicolon
    std::string leads_to_str =
        std::accumulate(std::next(parts.begin(), 9), parts.end(), std::string());
    flow_rates[valve_name] = std::stoul(flow_rate_str);
    leads_to[valve_name] = split(leads_to_str, ',');
  }

  std::map<std::string, std::size_t> ids;
  std::size_t id{0};
  for (const auto &[valve_name, _] : flow_rates) {
    ids[valve_name] = id;
    ++id;
  }

  std::vector<std::size_t> flow_rate;
  std::vector<std::vector<std::size_t>> adj_list;
  for (const auto &[valve_name, id] : ids) {
    flow_rate.emplace_back(flow_rates[valve_name]);
    std::vector<std::size_t> adj;
    std::transform(leads_to[valve_name].begin(), leads_to[valve_name].end(),
                   std::back_inserter(adj),
                   [&ids](const auto v_name) { return ids[v_name]; });
    adj_list.emplace_back(adj);
  }
  return {flow_rate, adj_list};
}

// https://stackoverflow.com/questions/42701688/using-an-unordered-map-with-arrays-as-keys
template <std::size_t N> struct ArrayHasher {
  std::size_t operator()(const std::array<std::size_t, N> &arr) const {
    std::size_t h{0};
    for (auto e : arr) {
      h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
  }
};
using StateSet = std::unordered_set<std::array<std::size_t, 4>, ArrayHasher<4>>;

Vec2d<std::size_t>
floyd_warshall(const std::vector<std::vector<std::size_t>> &adj_list) {
  Vec2d<std::size_t> dist(adj_list.size(), adj_list.size(),
                          std::numeric_limits<std::size_t>::max() / 4);
  for (auto i = 0U; i < adj_list.size(); ++i) {
    dist.get(i, i) = 0;
    for (auto neighbour : adj_list[i]) {
      dist.get(i, neighbour) = 1;
    }
  }
  for (auto k = 0U; k < adj_list.size(); ++k) {
    for (auto i = 0U; i < adj_list.size(); ++i) {
      for (auto j = 0U; j < adj_list.size(); ++j) {
        dist.get(i, j) = std::min(dist.get(i, j), dist.get(i, k) + dist.get(k, j));
      }
    }
  }
  return dist;
}

struct State {
  ul_bitset opened;
  std::size_t position;
  std::size_t time_spent;
  std::size_t pressure_relieved;
  std::array<std::size_t, 4> to_array() const {
    return {opened.to_ulong(), position, time_spent, pressure_relieved};
  }
};

std::size_t sum_flow_rate(const State &state, const std::vector<std::size_t> flow_rate) {
  std::size_t flow_rate_sum{0};
  for (auto i = 0U; i < flow_rate.size(); ++i) {
    if (state.opened[i]) {
      flow_rate_sum += flow_rate[i];
    }
  }
  return flow_rate_sum;
}

std::size_t sim_to_end(const State &state, const std::vector<std::size_t> flow_rate,
                       const std::size_t time_budget) {
  auto relieved_per_min = sum_flow_rate(state, flow_rate);
  auto time_remaining = time_budget - state.time_spent;
  return state.pressure_relieved + relieved_per_min * time_remaining;
}

std::vector<std::size_t> unopened_flowing(const std::vector<std::size_t> &flow_rate,
                                          const ul_bitset opened) {
  std::vector<std::size_t> uof;
  for (auto i = 0U; i < flow_rate.size(); ++i) {
    if (flow_rate[i] > 0 && !opened[i]) {
      uof.emplace_back(i);
    }
  }
  return uof;
}

bool all_valves_open(const std::vector<std::size_t> &flow_rate, const ul_bitset opened) {
  for (auto i = 0U; i < flow_rate.size(); ++i) {
    if (flow_rate[i] > 0 && !opened[i]) {
      return false;
    }
  }
  return true;
}
} // namespace

int day16_1(std::istream *input_file) {
  const auto [flow_rate, adjacency_list] = read_valves(input_file);
  const auto distance = floyd_warshall(adjacency_list);
  const std::size_t time_budget{30};

  State initial_state({0, 0, 0, 0});
  std::queue<State> states;
  states.push(initial_state);
  StateSet visited_states{initial_state.to_array()};
  std::size_t max_pressure_relieved{0};
  while (states.size() > 0) {
    auto state = states.front();
    states.pop();

    if (all_valves_open(flow_rate, state.opened)) {
      auto relieved = sim_to_end(state, flow_rate, time_budget);
      max_pressure_relieved = std::max(relieved, max_pressure_relieved);
      continue;
    }

    for (const auto dest : unopened_flowing(flow_rate, state.opened)) {
      const auto cost = distance.at(state.position, dest) + 1;
      const auto time_spent = state.time_spent + cost;
      if (time_spent > time_budget) {
        auto relieved = sim_to_end(state, flow_rate, time_budget);
        max_pressure_relieved = std::max(relieved, max_pressure_relieved);
        continue;
      }
      // Build the new state of things
      auto new_state = state;
      new_state.opened[dest] = true;
      new_state.position = dest;
      new_state.pressure_relieved += sum_flow_rate(state, flow_rate) * cost;
      new_state.time_spent += cost;

      if (visited_states.find(new_state.to_array()) == visited_states.end()) {
        visited_states.insert(state.to_array());
        states.push(new_state);
      }
    }
  }
  return max_pressure_relieved;
}

int day16_2(std::istream *input_file) { return 1; }
