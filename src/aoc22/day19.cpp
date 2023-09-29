#include <array>
#include <iostream>
#include <istream>
#include <numeric>
#include <queue>
#include <regex>
#include <string>

namespace {
constexpr auto BP_RE_PATTERN = R"(Blueprint \d+: )"
                               R"(.* ([0-9]+) ore. )"
                               R"(.* ([0-9]+) ore. )"
                               R"(.* ([0-9]+) ore and ([0-9]+) clay. )"
                               R"(.* ([0-9]+) ore and ([0-9]+) obsidian.)";
constexpr int ORE{0};
constexpr int CLAY{1};
constexpr int OBSIDIAN{2};
constexpr int GEODE{3};

struct Blueprint {
  Blueprint(const std::string &s) {
    std::regex pattern(BP_RE_PATTERN);
    std::smatch match_groups;
    std::regex_match(s, match_groups, pattern);
    ore_cost = std::stoi(match_groups[1].str());
    clay_cost = std::stoi(match_groups[2].str());
    obsidian_cost = std::make_pair(std::stoi(match_groups[3].str()),
                                   std::stoi(match_groups[4].str()));
    geode_cost = std::make_pair(std::stoi(match_groups[5].str()),
                                std::stoi(match_groups[6].str()));
  }

  int ore_cost{0};
  int clay_cost{0};
  std::pair<int, int> obsidian_cost = {0, 0};
  std::pair<int, int> geode_cost = {0, 0};
};

std::vector<Blueprint>
read_blueprints(std::istream *input_file,
                const std::size_t max = std::numeric_limits<std::size_t>::max()) {
  std::vector<Blueprint> blueprints;
  for (std::string line; std::getline(*input_file, line);) {
    if (blueprints.size() >= max) {
      break;
    }
    if (!line.empty()) {
      blueprints.emplace_back(line);
    }
  }
  return blueprints;
}

struct BlueprintState {
  BlueprintState(Blueprint blueprint, int max_time)
      : blueprint(std::move(blueprint)), time_remaining(max_time) {}

  bool can_build_robot(const int robot_type) {
    switch (robot_type) {
    case ORE:
      return materials[ORE] >= blueprint.ore_cost;
    case CLAY:
      return materials[ORE] >= blueprint.clay_cost;
    case OBSIDIAN:
      return materials[ORE] >= blueprint.obsidian_cost.first &&
             materials[CLAY] >= blueprint.obsidian_cost.second;
    case GEODE:
      return materials[ORE] >= blueprint.geode_cost.first &&
             materials[OBSIDIAN] >= blueprint.geode_cost.second;
    default:
      std::runtime_error("invalid material");
    }
    return false;
  }

  BlueprintState build_robot(int robot_type) const {
    BlueprintState new_state = *this;
    new_state.advance_time();
    new_state.robots[robot_type] += 1;
    switch (robot_type) {
    case ORE:
      new_state.materials[ORE] -= blueprint.ore_cost;
      break;
    case CLAY:
      new_state.materials[ORE] -= blueprint.clay_cost;
      break;
    case OBSIDIAN:
      new_state.materials[ORE] -= blueprint.obsidian_cost.first;
      new_state.materials[CLAY] -= blueprint.obsidian_cost.second;
      break;
    case GEODE:
      new_state.materials[ORE] -= blueprint.geode_cost.first;
      new_state.materials[OBSIDIAN] -= blueprint.geode_cost.second;
      break;
    default:
      std::runtime_error("invalid material");
    }
    return new_state;
  }

  void advance_time() {
    for (auto robot_type = 0U; robot_type < robots.size(); ++robot_type) {
      materials[robot_type] += robots[robot_type];
    }
    time_remaining -= 1;
  }

  Blueprint blueprint;
  std::array<int, 4> robots = {1, 0, 0, 0};
  std::array<int, 4> materials = {0};
  int time_remaining{0};
};

std::array<int, 4> get_max_spends(const Blueprint &blueprint) {
  int max_ore_spend{0};
  for (auto ore_cost : {blueprint.ore_cost, blueprint.clay_cost,
                        blueprint.obsidian_cost.first, blueprint.geode_cost.first}) {
    max_ore_spend = std::max(max_ore_spend, ore_cost);
  }
  int max_clay_spend = blueprint.obsidian_cost.second;
  int max_obsidian_spend = blueprint.geode_cost.second;
  return {max_ore_spend, max_clay_spend, max_obsidian_spend,
          std::numeric_limits<int>::max()};
}

int max_n_geodes_for_blueprint(const Blueprint &blueprint, const int max_time) {
  auto max_spends = get_max_spends(blueprint);

  std::queue<BlueprintState> states;
  states.push(BlueprintState(blueprint, max_time));
  int max_n_geodes{0};
  while (states.size() > 0) {
    auto state = states.front();
    states.pop();

    auto geodes_possible =
        state.materials[GEODE] + state.robots[GEODE] * state.time_remaining;
    max_n_geodes = std::max(max_n_geodes, geodes_possible);

    for (auto robot_type = 0; robot_type < 4; ++robot_type) {
      auto new_state = state;
      if (robot_type == OBSIDIAN && new_state.robots[CLAY] == 0) {
        // We can't make obsidian if we have no clay robots
        continue;
      }
      if (robot_type == GEODE && new_state.robots[OBSIDIAN] == 0) {
        // We can't make geode if we have no obsidian robots
        continue;
      }
      if (new_state.robots[robot_type] >= max_spends[robot_type]) {
        // There's no point making new robots if we can't, per minute, spend what they
        // make
        continue;
      }
      while (!new_state.can_build_robot(robot_type)) {
        new_state.advance_time();
      }
      new_state = new_state.build_robot(robot_type);

      // If we build build a geode robot and mine a geode on every step from now,
      // can we beat the current best?
      // n_geodes + n_robots*t + t*(t - 1)/2
      auto n_geodes = new_state.materials[GEODE];
      auto n_geode_robots = new_state.robots[GEODE];
      if (n_geodes + n_geode_robots * state.time_remaining +
              state.time_remaining * (state.time_remaining - 1) / 2 <
          max_n_geodes) {
        continue;
      }

      if (new_state.time_remaining > 0) {
        states.push(new_state);
      }
    }
  }
  return max_n_geodes;
}
} // namespace

int day19_1(std::istream *input_file) {
  const auto blueprints = read_blueprints(input_file);
  constexpr int max_time{24};

  int score_sum{0};
  for (auto id = 0U; id < blueprints.size(); ++id) {
    auto n_geodes = max_n_geodes_for_blueprint(blueprints[id], max_time);
    score_sum += n_geodes * (id + 1);
  }
  return score_sum;
}

int day19_2(std::istream *input_file) {
  constexpr std::size_t num_blueprints_to_process{3};
  const auto blueprints = read_blueprints(input_file, num_blueprints_to_process);
  constexpr int max_time{32};
  return std::accumulate(blueprints.begin(), blueprints.end(), 1,
                         [max_time](int acc, const auto &bp) {
                           return acc * max_n_geodes_for_blueprint(bp, max_time);
                         });
}
