#include <iostream>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <string>

#include "driver.hpp"

int main(const int argc, const char **argv)
{
	Calculator::Driver driver;

	std::ifstream source(argv[1]);
	std::ofstream compiled(argv[2]);
	std::string sourceString((std::istreambuf_iterator<char>(source)),
		std::istreambuf_iterator<char>());
	std::cout << sourceString << std::endl;

	driver.parse(sourceString);
	driver.compile(compiled) << "\n";

   return( EXIT_SUCCESS );
}
