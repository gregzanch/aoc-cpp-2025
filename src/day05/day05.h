#pragma once

#include <algorithm>
#include <cstdint>
#include <print>
#include "../solution.h"
#include "../util.h"

class Day05 : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/day05/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    bool parsing_ranges = true;
    for(const auto& line : read_result.value()) {
      if(line.size() == 0) {
        parsing_ranges = false;
        continue;
      }
      if(parsing_ranges) {
        const auto bounds = split_by(line, "-", [](const std::string& item){
          return std::stoull(item);
        });
        if(bounds.size() != 2) {
          return std::unexpected("Failed to parse bounds");
        }
        fresh_ranges.push_back(std::tuple(bounds[0], bounds[1]));
      } else {
        available_ids.push_back(std::stoull(line));
      }
    }

    return std::tuple(part1(), part2());
  }
 private:
  PartResult part1() {
    int count = 0;
    for(const auto& id : available_ids) {
      for(const auto& range : fresh_ranges) {
        const auto& [a, b] = range;
        if(within(id, a, b, BOTH_INCLUSIVE)) {
          count++;
          break;
        }
      }
    }
    return std::to_string(count);
  }
  PartResult part2() {
    // we want to somehow merge the ranges
    // i.e.
    // ------
    //    --------
    //   ---
    //               ---
    // becomes
    // -----------   ---


    // probably sort by lower bound
    std::sort(available_ids.begin(), available_ids.end(), [](auto a, auto b) {
      return false;
    });
    return std::to_string(0);
  }
  std::vector<std::tuple<uint64_t, uint64_t>> fresh_ranges;
  std::vector<uint64_t> available_ids;
};
