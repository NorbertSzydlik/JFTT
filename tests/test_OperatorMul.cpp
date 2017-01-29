#include <iostream>

#include "Fixture.hpp"

#include "catch.hpp"
#include "interpreter.hpp"

#include "logging.h"

TEST_CASE("mul constants", "OperatorMul")
{
  std::string program = R"DELIM(
VAR
  a
BEGIN
  a := 10 * 3;
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
  Fixture().checkOutput(programOutput, {30});
}

TEST_CASE("mul both identifiers", "OperatorMul")
{
  std::string program = R"DELIM(
VAR
  a b c
BEGIN
  a := 51;
  b := 12;
  c := a * b;
  WRITE c;
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
    51 * 12
  });
}

TEST_CASE("mul left identifiers", "OperatorMul")
{
  std::string program = R"DELIM(
VAR
  a b c
BEGIN
  a := 51;
  b := 12;
  c := a * 12;
  WRITE c;
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
    51 * 12
  });
}

TEST_CASE("mul right identifiers", "OperatorMul")
{
  std::string program = R"DELIM(
VAR
  a b c
BEGIN
  a := 51;
  b := 12;
  c := 51 * b;
  WRITE c;
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
    51 * 12
  });
}
