#include <iostream>
#include <sstream>

#include "catch.hpp"

#include <driver.hpp>

#include "interpreter.hpp"

class AssignmentTest
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
};

TEST_CASE("simple", "AssignmentTest")
{
  std::string program = R"DELIM(
VAR
  a
BEGIN
  a := 1;
  WRITE a;
  END
)DELIM";

  auto compiled = AssignmentTest().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  REQUIRE(result != 0);
  REQUIRE(programOutput.str() == "1\n");
}

TEST_CASE("array", "AssignmentTest")
{
  std::string program = R"DELIM(
VAR
  a[100]
BEGIN
  a[0] := 1;
  WRITE a[0];
  a[5] := 15;
  WRITE a[5];
  END
)DELIM";

  auto compiled = AssignmentTest().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  REQUIRE(result != 0);

  std::istringstream programOutputStream(programOutput.str());

  int outputNumber;

  programOutputStream >> outputNumber;
  REQUIRE(outputNumber == 1);

  programOutputStream >> outputNumber;
  REQUIRE(outputNumber == 15);
}
