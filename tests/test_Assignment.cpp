#include <iostream>
#include <sstream>

#include <gtest/gtest.h>
#include <driver.hpp>

#include "interpreter.hpp"

class AssignmentTest : public ::testing::Test
{
protected:
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

TEST_F(AssignmentTest, simple)
{
  std::string program = R"DELIM(
VAR
  a
BEGIN
  a := 1;
  WRITE a;
END
)DELIM";

  auto compiled = compile(program);

  std::istringstream compiledFile(compiled);
  std::istringstream stdIn;
//std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut
  std::ostringstream programOutput;
  auto result = interpret(compiledFile, std::cout, stdIn, programOutput);

  ASSERT_GT(result, 0);
  ASSERT_EQ(programOutput.str(), "1\n");
}
