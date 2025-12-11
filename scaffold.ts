#!/usr/bin/env -S deno run --allow-read --allow-write

const dayNum = Deno.args[0];

if (!dayNum) {
  console.error("Usage: scaffold.ts <day_number>");
  Deno.exit(1);
}

const paddedDay = dayNum.padStart(2, "0");
const folderName = `day${paddedDay}`;
const className = `Day${paddedDay}`;

// Check if folder already exists
try {
  await Deno.stat(`src/${folderName}`);
  console.error(`Error: Folder '${folderName}' already exists`);
  Deno.exit(1);
} catch (error) {
  if (!(error instanceof Deno.errors.NotFound)) {
    throw error;
  }
}

// Create folder
await Deno.mkdir(`src/${folderName}`);

// Create day<xx>.h
const headerContent = `#pragma once

#include "../solution.h"

class ${className} : public Solution {
 public:
  DayResult solve() override {
    // parse the lines
    const auto path = std::filesystem::path("src/${folderName}/input.txt");
    const auto read_result = read_input_lines(path);
    if (!read_result.has_value()) {
      return std::unexpected(read_result.error());
    }
    input = std::move(read_result.value());
    return std::tuple(part1(), part2());
  }
 private:
  PartResult part1() { return std::to_string(0); }
  PartResult part2() { return std::to_string(0); }
  std::vector<std::string> input;
};
`;

await Deno.writeTextFile(`src/${folderName}/${folderName}.h`, headerContent);

// Create empty input files
await Deno.writeTextFile(`src/${folderName}/example.txt`, "");
await Deno.writeTextFile(`src/${folderName}/input.txt`, "");

// Update days.h
const daysPath = "src/days.h";
const daysContent = await Deno.readTextFile(daysPath);

// Add include
const includePattern = /#include "day\d{2}\/day\d{2}\.h"/g;
const includes = daysContent.match(includePattern) || [] as string[];
const newInclude = `#include "${folderName}/${folderName}.h"`;
includes.push(newInclude);
includes.sort();

// Replace includes section
const includeStart = daysContent.indexOf('#include "day');
const includeEnd = daysContent.lastIndexOf('.h"') + 3;
const beforeIncludes = daysContent.substring(0, includeStart);
const afterIncludes = daysContent.substring(includeEnd);

// Add emplace_back
const emplacePattern = /v\.emplace_back\(std::make_unique<Day\d{2}>\(\)\);/g;
const emplaces = daysContent.match(emplacePattern) || [] as string[];
const newEmplace = `v.emplace_back(std::make_unique<${className}>());`;
emplaces.push(newEmplace);
emplaces.sort();

// Reconstruct days.h
const newDaysContent = `${beforeIncludes}${includes.join("\n")}

inline std::vector<std::unique_ptr<Solution>> get_days() {
  std::vector<std::unique_ptr<Solution>> v;
  ${emplaces.join("\n  ")}
  return v;
};
`;

await Deno.writeTextFile(daysPath, newDaysContent);

console.log(`Successfully scaffolded ${folderName}`);
console.log(`  Created: ${folderName}/${folderName}.h`);
console.log(`  Created: ${folderName}/example.txt`);
console.log(`  Created: ${folderName}/input.txt`);
console.log(`  Updated: days.h`);