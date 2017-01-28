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
