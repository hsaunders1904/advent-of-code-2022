#include "aoc22/aoc.h"

#include <bitset>
#include <map>
#include <numeric>
#include <queue>
#include <unordered_map>

namespace {
std::pair<std::vector<std::size_t>, std::vector<std::vector<std::size_t>>>
read_valves(std::istream *input) {
  // Important that we use an ordered map here, so AA is always first
  std::map<std::string, std::size_t> flow_rates;
  std::map<std::string, std::vector<std::string>> leads_to;
  for (std::string line; std::getline(*input, line);) {
    auto parts = split(line, ' ');
    auto valve_name = parts[1];
    auto flow_rate_str = split(parts[4], '=')[1];
    flow_rate_str.pop_back(); // remove semicolon
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
  std::size_t position;
  std::size_t time_remaining;
  std::size_t pressure_relieved;
  std::bitset<64> opened;

  bool operator==(const State &other) const {
    return (opened == other.opened) && (position == other.position) &&
           (time_remaining == other.time_remaining) &&
           (pressure_relieved == other.pressure_relieved);
  }

  State open_valve(std::size_t valve, std::size_t cost,
                   std::size_t total_flow_rate) const {
    auto new_state = *this;
    new_state.position = valve;
    new_state.opened.set(valve);
    cost += 1; // time to open the valve
    new_state.pressure_relieved = pressure_relieved + cost * total_flow_rate;
    new_state.time_remaining = time_remaining < cost ? 0 : time_remaining - cost;
    return new_state;
  }
};

std::size_t sum_flow_rate(const State &state,
                          const std::vector<std::size_t> &flow_rate) {
  std::size_t flow_rate_sum{0};
  for (auto i = 0U; i < flow_rate.size(); ++i) {
    if (state.opened[i]) {
      flow_rate_sum += flow_rate[i];
    }
  }
  return flow_rate_sum;
}

std::size_t sim_to_end(const State &state, const std::vector<std::size_t> &flow_rate) {
  auto relieved_per_min = sum_flow_rate(state, flow_rate);
  return state.pressure_relieved + relieved_per_min * state.time_remaining;
}

std::vector<std::size_t> unopened_flowing(const std::vector<std::size_t> &flow_rate,
                                          const std::bitset<64> opened) {
  std::vector<std::size_t> uof;
  uof.reserve(flow_rate.size());
  for (auto i = 0U; i < flow_rate.size(); ++i) {
    if (flow_rate[i] > 0 && !opened[i]) {
      uof.emplace_back(i);
    }
  }
  return uof;
}

std::bitset<64> make_flowing_valve_bitmask(const std::vector<std::size_t> &flow_rate) {
  std::bitset<64> all_valves_open_bitmask{0};
  for (auto i = 0U; i < flow_rate.size(); ++i) {
    if (flow_rate[i] > 0) {
      all_valves_open_bitmask.set(i);
    }
  }
  return all_valves_open_bitmask;
}

std::unordered_map<std::bitset<64>, std::size_t>
bfs_on_states(std::istream *input_file, const std::size_t time_budget) {
  const auto [flow_rate, adjacency_list] = read_valves(input_file);
  const auto distance = floyd_warshall(adjacency_list);
  const std::bitset<64> all_valves_open_bitmask = make_flowing_valve_bitmask(flow_rate);

  std::queue<State> state_queue;
  state_queue.push({0, time_budget, 0, 0});
  std::unordered_map<std::bitset<64>, std::size_t> state_to_max_relief;
  while (state_queue.size() > 0) {
    const auto state = state_queue.front();
    state_queue.pop();

    for (auto other_valve : unopened_flowing(flow_rate, state.opened)) {
      if (other_valve == state.position) {
        continue;
      }

      auto time_cost = distance.at(state.position, other_valve);
      if (time_cost + 1 >= state.time_remaining) {
        // + 1 & >= here, as for the journey to be useful, we need time to open the valve
        // and for some pressure to be released.
        continue;
      }

      auto current_flow_rate = sum_flow_rate(state, flow_rate);
      auto new_state = state.open_valve(other_valve, time_cost, current_flow_rate);
      if (new_state.opened != all_valves_open_bitmask) {
        state_queue.push(new_state);
      }
      auto relief = sim_to_end(new_state, flow_rate);
      state_to_max_relief[new_state.opened] =
          std::max(relief, state_to_max_relief[new_state.opened]);
    }
  }
  return state_to_max_relief;
}

} // namespace

int day16_1(std::istream *input_file) {
  const auto states = bfs_on_states(input_file, 30);
  auto max_relief = std::max_element(states.begin(), states.end(),
                                     [](auto a, auto b) { return a.second < b.second; });
  return max_relief->second;
}

int day16_2(std::istream *input_file) {
  const auto states = bfs_on_states(input_file, 26);
  std::size_t max_relief{0};
  for (const auto [my_valves, my_relief] : states) {
    for (const auto [elephant_valves, elephant_relief] : states) {
      if ((my_valves & elephant_valves) == 0) {
        max_relief = std::max(max_relief, my_relief + elephant_relief);
      }
    }
  }
  return max_relief;
}
