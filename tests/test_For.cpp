#include <iostream>

#include "Fixture.hpp"

#include "catch.hpp"
#include "interpreter.hpp"

#include "logging.h"


TEST_CASE("simple increasing", "FOR")
{
  std::string program = R"DELIM(
VAR
BEGIN
  FOR i FROM 1 TO 10 DO
    WRITE i;
  ENDFOR
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
  Fixture().checkOutput(programOutput, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

TEST_CASE("simple decreasing", "FOR")
{
  std::string program = R"DELIM(
VAR
BEGIN
  FOR i FROM 10 DOWNTO 0 DO
    WRITE i;
  ENDFOR
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
  Fixture().checkOutput(programOutput, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
}

TEST_CASE("For loop in for loop", "FOR")
{
  std::string program = R"DELIM(
VAR
BEGIN
    FOR i FROM 10 DOWNTO 1 DO
        FOR j FROM 0 TO i DO
            WRITE j;
        ENDFOR
    ENDFOR
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
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6,
    0, 1, 2, 3, 4, 5,
    0, 1, 2, 3, 4,
    0, 1, 2, 3,
    0, 1, 2,
    0, 1
  });
}

TEST_CASE("For loop in for loop in for loop", "FOR")
{
  std::string program = R"DELIM(
VAR
    a
BEGIN
    a := 0;
    FOR i FROM 10 DOWNTO 1 DO
        FOR j FROM 0 TO i DO
            FOR k FROM 0 TO j DO
                a := a + 1;
            ENDFOR
        ENDFOR
    ENDFOR
    WRITE a;
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
    285
  });
}

TEST_CASE("For loop over table", "FOR")
{
  std::string program = R"DELIM(
VAR
    a[10]
BEGIN
    FOR i FROM 0 TO 9 DO
      a[i] := i;
    ENDFOR
    FOR i FROM 9 DOWNTO 0 DO
      WRITE a[i];
    ENDFOR
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
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0
  });
}
