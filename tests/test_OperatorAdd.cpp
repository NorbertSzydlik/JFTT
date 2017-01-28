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

  REQUIRE(result > 0);
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
  WRITE a;
  WRITE b;
  WRITE c;
  c:= 1 + c;
  WRITE a;
  WRITE b;
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

  REQUIRE(result > 0);

  Fixture().checkOutput(programOutput, {
    1,
    1, 2,
    1, 2, 3,
    1, 2, 4
  });
}

TEST_CASE("add table identifiers", "OperatorAdd")
{
  std::string program = R"DELIM(
VAR
  a[100]
BEGIN
  a[0] := 1;
  WRITE a[0];

  a[1] := a[0] + 1;
  WRITE a[0];
  WRITE a[1];

  a[2] := 1 + a[1];
  WRITE a[0];
  WRITE a[1];
  WRITE a[2];

  a[3] := a[1] + a[2];
  WRITE a[0];
  WRITE a[1];
  WRITE a[2];
  WRITE a[3];
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  REQUIRE(result > 0);

  Fixture().checkOutput(programOutput, {
    1,
    1, 2,
    1, 2, 3,
    1, 2, 3, 5
  });
}
