#pragma once

#include <print>
#include "../matrix.h"
#include "../solution.h"

class Day04 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day04/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }

    std::vector<std::vector<char>> grid_data;
    grid_data.reserve(read_result.value().size());
    for (const auto& line : read_result.value()) {
      const auto items = std::vector<char>(line.begin(), line.end());
      grid_data.emplace_back(std::move(items));
    }

    grid.set_data(std::move(grid_data));

    // grid.print();

    return std::tuple(part1(), part2());
  }

 private:
  int remove_rolls(bool mutate) {
    std::vector<std::tuple<int, int>> positions;
    std::array<std::tuple<int, int>, 8> delta_positions{
        std::tuple<int, int>(-1, 0),   // L
        std::tuple<int, int>(-1, 1),   // LU
        std::tuple<int, int>(-1, -1),  // LD
        std::tuple<int, int>(0, 1),    // U
        std::tuple<int, int>(0, -1),   // D
        std::tuple<int, int>(1, 0),    // R
        std::tuple<int, int>(1, 1),    // RU
        std::tuple<int, int>(1, -1),   // RD
    };
    for (const auto& [x, y, value] : grid.iter()) {
      // std::println("{},{},{}", x, y, value);
      if (value != '@') {
        continue;
      }
      int neighbor_count = 0;
      for (const auto& [dx, dy] : delta_positions) {
        auto new_x = x + dx;
        auto new_y = y + dy;
        auto val = grid.get(new_x, new_y);
        if (val.has_value() && val.value() == '@') {
          neighbor_count++;
        }
      }
      if (neighbor_count < 4) {
        // std::println("{},{}", x, y);
        positions.emplace_back(std::tuple<int, int>(x, y));
      }
    }

    if (mutate) {
      for (const auto& [x, y] : positions) {
        const auto ref = grid.get_ref_mut(x, y);
        if (ref.has_value()) {
          ref.value().get() = '.';
        }
      }
    }

    return positions.size();
  }

  PartResult part1() { return std::to_string(remove_rolls(false)); }
  PartResult part2() {
    int count = 0;
    bool done = false;
    while(!done) {
      int removed_count = remove_rolls(true);
      if(removed_count == 0) {
        break;
      }
      count += removed_count;
    }
    return std::to_string(count);
  }
  Matrix<char> grid;
};