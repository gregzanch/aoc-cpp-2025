#pragma once

#include <cstdint>
#include <print>
#include <string>
#include "../solution.h"
#include "../util.h"

class Day02 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day02/input.txt");
    const auto read_result = read_input_file(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    ranges = parse_ranges(read_result.value());
    return std::tuple(part1(), part2());
  }

 private:
  struct IdRange {
    uint64_t first;
    uint64_t last;
  };

  std::vector<IdRange> parse_ranges(const std::string& input) {
    return split_by(input, ",", [](const auto range) {
      const auto sections = split_by(
          range, "-", [](const auto id) { return static_cast<uint64_t>(std::stoull(id)); });
      return IdRange{
          sections[0],
          sections[1],
      };
    });
  }

  PartResult part1() {
    std::regex re(R"(^(\d+)\1$)");
    uint64_t count = 0;
    for (const auto& range : ranges) {
      for (auto i = range.first; i <= range.last; i++) {
        const auto number_string = std::to_string(i);
        const auto m = basic_match(number_string, re);
        if (m.has_value()) {
          count += i;
        }
      }
    }
    return std::to_string(count);
  }
  PartResult part2() {
    std::regex re(R"(^(\d+)\1+$)");
    uint64_t count = 0;
    for (const auto& range : ranges) {
      for (auto i = range.first; i <= range.last; i++) {
        const auto number_string = std::to_string(i);
        const auto m = basic_match(number_string, re);
        if (m.has_value()) {
          count += i;
        }
      }
    }
    return std::to_string(count);
  }

  std::vector<IdRange> ranges;
};