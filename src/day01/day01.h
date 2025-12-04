#pragma once

#include <cmath>
#include <ranges>
#include "../solution.h"
#include "../util.h"
class Day01 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day01/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = std::move(read_result.value());
    commands = parse_commands();
    return std::tuple(part1(), part2());
  }

 private:
  enum Direction : char {
    LEFT = 'L',
    RIGHT = 'R',
  };

  struct Command {
    Direction direction;
    int amount;
  };

  static Command parse_command(const std::string& line) {
    auto dir = line.substr(0, 1) == "L" ? Direction::LEFT : Direction::RIGHT;
    auto amount = std::stoi(line.substr(1));
    return Command{dir, amount};
  }

  std::vector<Command> parse_commands() {
    return input | std::views::transform(parse_command) | std::ranges::to<std::vector>();
  }

  PartResult part1() {
    int pos = start_pos;
    int zero_count = 0;
    for(const auto& command : commands) {
      switch (command.direction) {
        case Direction::LEFT: {
          pos = circular_mod(pos - command.amount, 100);
        } break;
        case Direction::RIGHT: {
          pos = circular_mod(pos + command.amount, 100);
        } break;
        default: break;
      }
      if (pos == 0) {
        zero_count++;
      }
    }
    return std::to_string(zero_count);
  }
  PartResult part2() { 
    int pos = start_pos;
    int zero_count = 0;
    for(const auto& command : commands) {
      switch (command.direction) {
        case Direction::LEFT: {
          // flip to be positive
          const auto flipped_pos = 100 - pos;
          const auto rem = std::floor((flipped_pos + command.amount) / 100);
          zero_count += pos == 0 ? 0 : rem;
          pos = circular_mod(pos - command.amount, 100);
        } break;
        case Direction::RIGHT: {
          const auto rem = std::floor((pos + command.amount) / 100);
          zero_count += pos == 0 ? 0 : rem;
          pos = circular_mod(pos + command.amount, 100);
        } break;
        default: break;
      }
    }
    return std::to_string(zero_count);
  }

  int start_pos = 50;
  std::vector<std::string> input;
  std::vector<Command> commands;
};