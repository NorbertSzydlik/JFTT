#include <iostream>

#include "Fixture.hpp"

#include "catch.hpp"
#include "interpreter.hpp"

TEST_CASE("add constants", "OperatorAdd")
{
  std::string program = R"DELIM(
VAR
  a
BEGIN
  a := 25 + 74;
  WRITE a;
  END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  REQUIRE(result != 0);
  Fixture().checkOutput(programOutput, {25 + 74});
}

TEST_CASE("add identifiers", "OperatorAdd")
{
  std::string program = R"DELIM(
VAR
  a b c
BEGIN
  a := 1;
  WRITE a;
  b := a + 1;
  WRITE a;
  WRITE b;
  c := a + b;
  WRITE c;
  c:= 1 + c;
  WRITE c;
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  REQUIRE(result != 0);

  Fixture().checkOutput(programOutput, {1, 1, 2, 3, 4});
}
