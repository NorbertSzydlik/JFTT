#include <iostream>

#include "Fixture.hpp"

#include "catch.hpp"
#include "interpreter.hpp"

#include "logging.h"

TEST_CASE("div constants", "OperatorDiv")
{
  std::string program = R"DELIM(
VAR
  a
BEGIN
  a := 10 / 3;
  WRITE a;
  a := 10 % 3;
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
  Fixture().checkOutput(programOutput, {
    3,
    1
  });
}

TEST_CASE("div both identifiers", "OperatorDiv")
{
  std::string program = R"DELIM(
VAR
  a b c d
BEGIN
  a := 51;
  b := 12;
  c := a / b;
  WRITE c;
  d := a % b;
  WRITE d;
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  std::ostringstream programOutput;

  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
  LOG(programOutput.str());

  REQUIRE(result > 0);

  Fixture().checkOutput(programOutput, {
    51 / 12,
    51 % 12
  });
}

TEST_CASE("div left identifiers", "OperatorDiv")
{
  std::string program = R"DELIM(
VAR
  a b c d
BEGIN
  a := 51;
  b := 12;
  c := a / 12;
  WRITE c;
  d := a % 12;
  WRITE d;
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  std::ostringstream programOutput;

  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
  LOG(programOutput.str());

  REQUIRE(result > 0);

  Fixture().checkOutput(programOutput, {
    51 / 12,
    51 % 12
  });
}

TEST_CASE("div right identifiers", "OperatorDiv")
{
  std::string program = R"DELIM(
VAR
  a b c d
BEGIN
  a := 51;
  b := 12;
  c := 51 / b;
  WRITE c;
  d := 51 % b;
  WRITE d;
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  std::ostringstream programOutput;

  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
  LOG(programOutput.str());

  REQUIRE(result > 0);

  Fixture().checkOutput(programOutput, {
    51 / 12,
    51 % 12
  });
}
