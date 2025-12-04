#pragma once

#include "../solution.h"

class Day03 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day03/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = std::move(read_result.value());

    return std::tuple(part1(), part2());
  }

 private:
  PartResult part1() {
    // naive
    int sum = 0;
    for(const auto& line : input) {
      int max = 0;
      for(int i = 0; i<line.length()-1; i++) {
        for(int j = i+1; j<line.length(); j++) {
          const char first_digit = line[i];
          const char second_digit = line[j];
          std::string concat;
          concat.append(1, first_digit);
          concat.append(1, second_digit);
          const auto val = std::stoi(concat);
          if(val >= max) {
            max = val;
          }
        }
      }
      sum += max;
    }
    return std::to_string(sum);
  }
  PartResult part2() { return std::to_string(0); }

  std::vector<std::string> input;
};