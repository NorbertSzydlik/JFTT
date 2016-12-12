#pragma once
#include <string>

class GebalaCompiler
{
public:
	GebalaCompiler();
	~GebalaCompiler();
	static std::string compile(std::string input);
protected:
	static std::string removeComments(std::string input);
	static std::string removeEmptyLines(std::string input);
	static std::string trimLines(std::string input);
	static std::string compileCommands(std::string input);
	static std::string compileCommand(std::string input);
	static std::string getRegexNumber(std::string input);
};
