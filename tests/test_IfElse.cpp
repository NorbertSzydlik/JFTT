#include <iostream>

#include "Fixture.hpp"

#include "catch.hpp"
#include "interpreter.hpp"

#include "logging.h"

TEST_CASE("simple const if", "IfElse")
{
  std::string program = R"DELIM(
VAR
BEGIN
  IF 1 < 2 THEN
    WRITE 1;
  ELSE
    SKIP;
  ENDIF
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  LOG("program out:\n" << programOutput.str() << "=====");

  REQUIRE(result > 0);
  Fixture().checkOutput(programOutput, {1});
}

TEST_CASE("test operators < >", "IfElse")
{
  std::string program = R"DELIM(
VAR
  a b
BEGIN
  a := 1;
  b := 2;

  IF a < 2 THEN
    WRITE 1;
  ELSE
    WRITE 2;
  ENDIF

  IF a > 2 THEN
    WRITE 3;
  ELSE
   WRITE 4;
  ENDIF

  IF 2 > a THEN
    WRITE 5;
  ELSE
   WRITE 6;
  ENDIF

  IF 2 < a THEN
    WRITE 7;
  ELSE
   WRITE 8;
  ENDIF

  IF a < b THEN
    WRITE 9;
  ELSE
    WRITE 10;
  ENDIF

  IF a > b THEN
    WRITE 11;
  ELSE
    WRITE 12;
  ENDIF
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  LOG("program out:\n" << programOutput.str() << "=====");

  REQUIRE(result > 0);
  Fixture().checkOutput(programOutput, {1, 4, 5, 8, 9, 12});
}

TEST_CASE("test operators <= >= when elements not equal", "IfElse")
{
  std::string program = R"DELIM(
VAR
  a b
BEGIN
  a := 1;
  b := 2;

  IF a <= 2 THEN
    WRITE 1;
  ELSE
    WRITE 2;
  ENDIF

  IF a >= 2 THEN
    WRITE 3;
  ELSE
   WRITE 4;
  ENDIF

  IF 2 >= a THEN
    WRITE 5;
  ELSE
   WRITE 6;
  ENDIF

  IF 2 <= a THEN
    WRITE 7;
  ELSE
   WRITE 8;
  ENDIF

  IF a < b THEN
    WRITE 9;
  ELSE
    WRITE 10;
  ENDIF

  IF a > b THEN
    WRITE 11;
  ELSE
    WRITE 12;
  ENDIF
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  LOG("program out:\n" << programOutput.str() << "=====");

  REQUIRE(result > 0);
  Fixture().checkOutput(programOutput, {1, 4, 5, 8, 9, 12});
}


TEST_CASE("test operators <= >= when elements are equal", "IfElse")
{
  std::string program = R"DELIM(
VAR
  a b
BEGIN
  a := 1;
  b := 1;

  IF a <= 1 THEN
    WRITE 1;
  ELSE
    WRITE 2;
  ENDIF

  IF a >= 1 THEN
    WRITE 3;
  ELSE
   WRITE 4;
  ENDIF

  IF 1 >= a THEN
    WRITE 5;
  ELSE
   WRITE 6;
  ENDIF

  IF 1 <= a THEN
    WRITE 7;
  ELSE
   WRITE 8;
  ENDIF

  IF a <= b THEN
    WRITE 9;
  ELSE
    WRITE 10;
  ENDIF

  IF a >= b THEN
    WRITE 11;
  ELSE
    WRITE 12;
  ENDIF
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  LOG("program out:\n" << programOutput.str() << "=====");

  REQUIRE(result > 0);
  Fixture().checkOutput(programOutput, {1, 3, 5, 7, 9, 11});
}

TEST_CASE("test operator =", "IfElse")
{
  std::string program = R"DELIM(
VAR
  a b c
BEGIN
  a := 1;
  b := 1;
  c := 2;

  IF a = 1 THEN
    WRITE 1;
  ELSE
    WRITE 2;
  ENDIF

  IF a = 2 THEN
    WRITE 3;
  ELSE
   WRITE 4;
  ENDIF

  IF 1 = a THEN
    WRITE 5;
  ELSE
   WRITE 6;
  ENDIF

  IF 2 = a THEN
    WRITE 7;
  ELSE
   WRITE 8;
  ENDIF

  IF a = b THEN
    WRITE 9;
  ELSE
    WRITE 10;
  ENDIF

  IF a = c THEN
    WRITE 11;
  ELSE
    WRITE 12;
  ENDIF
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  LOG("program out:\n" << programOutput.str() << "=====");

  REQUIRE(result > 0);
  Fixture().checkOutput(programOutput, {1, 4, 5, 8, 9, 12});
}

TEST_CASE("test operator <>", "IfElse")
{
  std::string program = R"DELIM(
VAR
  a b c
BEGIN
  a := 1;
  b := 1;
  c := 2;

  IF a <> 1 THEN
    WRITE 1;
  ELSE
    WRITE 2;
  ENDIF

  IF a <> 2 THEN
    WRITE 3;
  ELSE
   WRITE 4;
  ENDIF

  IF 1 <> a THEN
    WRITE 5;
  ELSE
   WRITE 6;
  ENDIF

  IF 2 <> a THEN
    WRITE 7;
  ELSE
   WRITE 8;
  ENDIF

  IF a <> b THEN
    WRITE 9;
  ELSE
    WRITE 10;
  ENDIF

  IF a <> c THEN
    WRITE 11;
  ELSE
    WRITE 12;
  ENDIF
END
)DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn;
  //std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);

  LOG("program out:\n" << programOutput.str() << "=====");

  REQUIRE(result > 0);
  Fixture().checkOutput(programOutput, {2, 3, 6, 7, 10, 11});
}
