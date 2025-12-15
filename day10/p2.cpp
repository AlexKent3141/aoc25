#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include <cstdint>
#include <bit>

struct machine {
  std::vector<std::vector<int>> actions;
  std::vector<int> targets;
};

template <std::size_t T>
std::ostream& operator<<(std::ostream& os, const std::array<int, T>& digits) {
  for (int d : digits) os << d << " ";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& digits) {
  for (int d : digits) os << d << " ";
  return os;
}

std::ostream& operator<<(std::ostream& os, const machine& m) {
  os << "Actions:" << std::endl;
  for (const auto& actions : m.actions) os << actions << std::endl;
  os << "Targets:" << std::endl;
  os << m.targets;
  return os;
}

std::vector<int> parse_values(std::string s) {
  std::vector<int> values;

  std::replace(s.begin(), s.end(), '(', ' ');
  std::replace(s.begin(), s.end(), '{', ' ');
  std::replace(s.begin(), s.end(), ',', ' ');

  std::stringstream ss(s);
  std::string value_str;

  while (ss >> value_str) {
    values.push_back(std::stoi(value_str));
  }

  return values;
}

struct search_state {
  std::array<int, 20> remaining_targets;
  std::array<int, 20> current_multipliers;
  int considered_flags;
  int mul_total;
};

std::vector<int> find_action_indices(
  const std::vector<std::vector<int>>& actions,
  std::size_t index) {

  std::vector<int> action_indices;
  for (std::size_t i = 0; i < actions.size(); ++i) {
    const auto& action = actions[i];
    if (std::find(action.cbegin(), action.cend(), index) != std::cend(action)) {
      action_indices.push_back(i);
    }
  }

  return action_indices;
}

std::vector<int> make_target_action_flags(
  const std::vector<std::vector<int>>& actions,
  const std::vector<int>& targets) {

  std::vector<int> target_action_flags;
  for (std::size_t t = 0; t < targets.size(); ++t) {
    int flags = 0;
    for (std::size_t a = 0; a < actions.size(); ++a) {
      const auto& action = actions[a];
      if (std::find(action.cbegin(), action.cend(), t) != action.cend()) {
        flags |= (1 << a);
      }
    }

    target_action_flags.push_back(flags);
  }

  return target_action_flags;
}

int best_score;

bool check_solution(
  const auto& remaining,
  const auto& multipliers) {

  if (std::all_of(
    remaining.cbegin(),
    remaining.cend(),
    [] (auto target) { return target == 0; })) {

    int sum = std::accumulate(
      multipliers.cbegin(),
      multipliers.cend(),
      0);

    if (sum < best_score) {
      best_score = sum;
      std::cout << "Best so far: " << multipliers << ", " << best_score << std::endl;
    }

    return true;
  }

  return false;
}

std::uint64_t skip_total = 0;

void solve(
  const std::vector<std::vector<int>>& actions,
  const std::vector<int>& targets,
  const std::vector<std::vector<int>>& action_indices,
  const std::vector<int>& target_action_flags,
  search_state& state) {

  if (state.mul_total >= best_score) {
    ++skip_total;
    if ((skip_total & 0xFFFFFF) == 0xFFFFFF)
      std::cout << "Skipped: " << skip_total << "\n";
    return;
  }

  // Check the target indices for which all actions have been completed are correct.
  for (std::size_t i = 0; i < targets.size(); ++i) {
    if ((state.considered_flags & target_action_flags[i]) == target_action_flags[i]) {
      if (state.remaining_targets[i] != 0) {
        return;
      }
    }
  }

  // Pick the next action to try.
  // First find the lowest (non-zero) remaining target.
  int current_index = -1;
  int lowest_so_far = 100000;
  for (std::size_t i = 0; i < state.remaining_targets.size(); ++i) {
    if (state.remaining_targets[i] > 0 && state.remaining_targets[i] < lowest_so_far) {
      current_index = i;
      lowest_so_far = state.remaining_targets[i];
    }
  }

  assert(current_index != -1);

  // Next action for this target.
  int action_index = -1;
  for (int ai : action_indices[current_index]) {
    if ((state.considered_flags & (1 << ai)) == 0) {
      action_index = ai;
    }
  }

  assert(action_index != -1);

  // If this is the last action which affects this target.
  if (std::has_single_bit(static_cast<std::uint32_t>(target_action_flags[current_index] & ~state.considered_flags))) {

    int mul = state.remaining_targets[current_index];

    // Will this invalidate any constraints?
    auto next_remaining_targets = state.remaining_targets;
    const auto& action = actions[action_index];
    for (int i : action) {
      next_remaining_targets[i] -= mul;
      if (next_remaining_targets[i] < 0) {
        // Impossible.
        return;
      }
    }

    auto next_multipliers = state.current_multipliers;
    next_multipliers[action_index] += mul;

    // Is this a solution?
    if (check_solution(next_remaining_targets, next_multipliers)) {
      return;
    }

    auto next_considered_flags = state.considered_flags;
    next_considered_flags |= (1 << action_index);

    search_state next = {
      next_remaining_targets,
      next_multipliers,
      next_considered_flags,
      state.mul_total + mul
    };

    return solve(actions, targets, action_indices, target_action_flags, next);
  }

  assert((state.considered_flags & (1 << action_index)) == 0);

  // Consider voluntarily skipping this action with no multiplier.
  if (state.current_multipliers[action_index] == 0) {
    auto next_considered_flags = state.considered_flags;
    next_considered_flags |= (1 << action_index);

    search_state next = {
      state.remaining_targets,
      state.current_multipliers,
      next_considered_flags,
      state.mul_total
    };

    solve(actions, targets, action_indices, target_action_flags, next);
  }

  // Will this action invalidate any constraints.
  auto next_remaining_targets = state.remaining_targets;
  const auto& action = actions[action_index];
  for (int i : action) {
    --next_remaining_targets[i];
    if (next_remaining_targets[i] < 0) {
      // Impossible.
      return;
    }
  }

  auto next_multipliers = state.current_multipliers;
  ++next_multipliers[action_index];

  auto next_mul_total = state.mul_total + 1;

  // Is this a solution?
  if (check_solution(next_remaining_targets, next_multipliers)) {
    return;
  }

  // Try incrementing this one again OR move to the next action.
  search_state next = {
    next_remaining_targets,
    next_multipliers,
    state.considered_flags,
    next_mul_total
  };

  // Solve again based on the same action.
  solve(actions, targets, action_indices, target_action_flags, next);

  // Move to the next action in the pool.
  auto next_considered_flags = state.considered_flags;
  next_considered_flags |= (1 << action_index);

  next = {
    next_remaining_targets,
    next_multipliers,
    next_considered_flags,
    next_mul_total
  };

  solve(actions, targets, action_indices, target_action_flags, next);
}

int main() {

  std::ifstream ifs("input.txt");
  std::string token;

  int index = 0;
  std::vector<int> bests;
  machine m;
  while (ifs >> token) {
    if (token[0] == '(') {
      m.actions.push_back(parse_values(token));
    }
    else if (token[0] == '{') {
      // This is the end of the current machine.
      m.targets = parse_values(token);

      std::cout << "Solving index: " << index++ << std::endl;
      std::cout << m << std::endl;

      // Setup the action lists for each index.
      std::vector<std::vector<int>> action_indices;
      for (std::size_t i = 0; i < m.targets.size(); ++i) {
        action_indices.push_back(find_action_indices(m.actions, i));
      }

      const auto target_action_flags = make_target_action_flags(m.actions, m.targets);

      std::array<int, 20> remaining{0};
      std::memcpy(remaining.data(), m.targets.data(), m.targets.size() * sizeof(int));

      search_state next = { remaining, 0, 0, 0 };

      best_score = 100000;
      solve(
        m.actions,
        m.targets,
        action_indices,
        target_action_flags,
        next);

      assert(best_score < 100000);
      bests.push_back(best_score);

      // Prep for next machine.
      m.actions.clear();
      m.targets.clear();
    }
  }

  std::cout << "Bests: " << bests << std::endl;
  std::cout << "Total: " << std::accumulate(bests.cbegin(), bests.cend(), 0) << std::endl;

  return 0;
}
