#pragma once

#include "catch.hpp"
#include <sstream>
#include <vector>
#include <driver.hpp>

class Fixture
{
public:
  std::string compile(std::string& input)
  {
    Calculator::Driver driver;

    std::istringstream source(input);
    std::ostringstream compiled{};
    std::string sourceString((std::istreambuf_iterator<char>(source)),
    std::istreambuf_iterator<char>());

    driver.parse(sourceString);
    driver.compile(compiled) << "\n";

    return compiled.str();
  }
  bool checkOutput(std::ostringstream& programOutput, std::vector<int> expectedOutputs)
  {
    std::istringstream programOutputStream(programOutput.str());

    std::string line;
    std::vector<std::string> outputLines;

    while (std::getline(programOutputStream, line)) {
      outputLines.push_back(line);
    }

    REQUIRE(outputLines.size() == expectedOutputs.size());

    auto currentOutputLine = std::begin(outputLines);
    for(auto& expected : expectedOutputs )
    {
      int outputNumber = std::stoi(*currentOutputLine);
      CHECK(outputNumber == expected);
      ++currentOutputLine;
    }

    return true;
  }
};
