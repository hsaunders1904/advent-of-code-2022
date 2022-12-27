#include "aoc22/aoc.h"

#include <array>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace {

struct Valve {
  std::size_t rate;
  std::vector<std::string> leads_to;
};

std::unordered_map<std::string, Valve> read_valves(std::istream *input) {
  std::unordered_map<std::string, Valve> valves;
  for (std::string line; std::getline(*input, line);) {
    auto parts = split(line, ' ');
    auto valve_name = parts[1];
    auto flow_rate_str = split(parts[4], '=')[1];
    flow_rate_str.erase(flow_rate_str.end()); // remove semicolon
    std::string leads_to_str =
        std::accumulate(std::next(parts.begin(), 9), parts.end(), std::string());
    valves[valve_name].rate = std::stoul(flow_rate_str);
    valves[valve_name].leads_to = split(leads_to_str, ',');
  }
  return valves;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>>
floyd_warshall(const std::unordered_map<std::string, Valve> &adjacency_list) {
  std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>> dist;
  // Set all distances to something large (but not so large that we get overflows!)
  for (const auto &[i, valve_i] : adjacency_list) {
    for (const auto &[j, valve_j] : adjacency_list) {
      dist[i][j] = std::numeric_limits<std::size_t>().max() / 4;
    }
  }
  // Set distances to self to 0, and neighbour distances to 1
  for (const auto &[start, valve] : adjacency_list) {
    dist[start][start] = 0;
    for (const auto &neighbour : valve.leads_to) {
      dist[start][neighbour] = 1;
    }
  }
  // Loop through valves and accumulate distances
  for (const auto &[id_k, valve_k] : adjacency_list) {
    for (const auto &[id_i, valve_i] : adjacency_list) {
      for (const auto &[id_j, valve_j] : adjacency_list) {
        dist[id_i][id_j] =
            std::min(dist[id_i][id_j], dist[id_i][id_k] + dist[id_k][id_j]);
      }
    }
  }
  return dist;
}

struct State {
  std::unordered_set<std::string> opened;
  std::string current;
  std::size_t time_elapsed;
  std::size_t pressure_relieved;
  bool operator==(const State &other) const {
    return opened == other.opened && current == other.current &&
           time_elapsed == other.time_elapsed;
  }
  bool operator<(const State &other) const {
    if (current == other.current) {
      if (time_elapsed == other.time_elapsed) {
        if (pressure_relieved == other.pressure_relieved) {
          return false;
        }
        return pressure_relieved < other.pressure_relieved;
      }
      return time_elapsed < other.time_elapsed;
    }
    return current < other.current;
  }
};

std::size_t wait_until_ending(const std::size_t time_left, const std::size_t relieved,
                              const std::unordered_set<std::string> &opened,
                              const std::unordered_map<std::string, Valve> &valves) {
  std::size_t relieved_per_min{0};
  for (const auto &v_id : opened) {
    relieved_per_min += valves.at(v_id).rate;
  }
  return relieved + (relieved_per_min * time_left);
}

std::vector<std::string>
get_unopened(const std::unordered_map<std::string, Valve> &valves,
             const std::unordered_set<std::string> &opened) {
  std::vector<std::string> unopened;
  for (const auto &v_id : valves) {
    if (opened.find(v_id.first) == opened.end()) {
      unopened.emplace_back(v_id.first);
    }
  }
  return unopened;
}

std::size_t sum_flow_rate(const std::unordered_map<std::string, Valve> &valves,
                          const std::unordered_set<std::string> &opened) {
  std::size_t flow_rate{0};
  for (const auto &valve_id : opened) {
    flow_rate += valves.at(valve_id).rate;
  }
  return flow_rate;
}
} // namespace

int day16_1(std::istream *input_file) {
  const auto valves = read_valves(input_file);
  const auto distances = floyd_warshall(valves);
  std::unordered_map<std::string, Valve> flowing;
  for (const auto &valve : valves) {
    if (valve.second.rate > 0) {
      flowing.insert(valve);
    }
  }

  std::size_t max_time{30};
  std::size_t max_relieved{0};

  State initial_state{{}, "AA", 0, 0};
  std::set<State> seen;
  seen.insert(initial_state);
  std::queue<State> states;
  states.push(initial_state);
  while (states.size() > 0) {
    std::cout << states.size() << std::endl;
    auto state = states.front();
    states.pop();

    if (state.opened.size() == flowing.size() || state.time_elapsed >= 30) {
      auto relieved_at_end = wait_until_ending(
          max_time - state.time_elapsed, state.pressure_relieved, state.opened, valves);
      max_relieved = std::max(max_relieved, relieved_at_end);
      std::cout << "relieved: " << relieved_at_end << std::endl;
      continue;
    }

    for (const auto &dest : get_unopened(flowing, state.opened)) {
      auto cost = distances.at(state.current).at(dest) + 1;
      auto new_elapsed = state.time_elapsed + cost;
      if (new_elapsed >= 30) {
        auto relieved_at_end =
            wait_until_ending(max_time - state.time_elapsed, state.pressure_relieved,
                              state.opened, valves);
        std::cout << "relieved: " << relieved_at_end << std::endl;
        max_relieved = std::max(max_relieved, relieved_at_end);
        continue;
      }
      auto relieved_per_min = sum_flow_rate(flowing, state.opened);
      auto new_relieved = state.pressure_relieved + (relieved_per_min * cost);
      auto new_opened = state.opened;
      new_opened.insert(dest);

      State new_state({new_opened, dest, new_elapsed, new_relieved});
      if (seen.find(new_state) == seen.end()) {
        seen.insert(state);
        states.push(new_state);
      }
    }
  }
  return max_relieved;
}

int day16_2(std::istream *input_file) { return 1; }