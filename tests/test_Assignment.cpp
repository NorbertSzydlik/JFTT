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

TEST_CASE("AssignmentTest", "simple")
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
