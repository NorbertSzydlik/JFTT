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
    int outputNumber;

    for(auto& expected : expectedOutputs )
    {
      programOutputStream >> outputNumber;
      CHECK(outputNumber == expected);
    }
  }
};
