#include "GebalaCompiler.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <map>

#include "Number.hpp"
#include "logging.h"

GebalaCompiler::GebalaCompiler()
{
}

GebalaCompiler::~GebalaCompiler()
{
}

std::string GebalaCompiler::compile(std::string input) {
	LOG("raw:\n" << input << "\nend raw" );
	input = removeComments(input);
	input = removeEmptyLines(input);
	input = trimLines(input);
	input = compileCommands(input);
	input = replaceLabelsWithLines(input);
	input = fixRegisterNames(input);
	input = addHalt(input);
	LOG("compiled:\n" << input << "\nend compiled");
	return input;
}

std::string GebalaCompiler::removeComments(std::string input) {
	bool comment = false;
	for (int i = 0; i < input.size(); ++i) {
		if (comment && input[i] == '\n') {
				comment = false;
		}
		else if(comment) {
			input[i] = ' ';
		}
		else if (input[i] == '#') {
			comment = true;
			input[i] = ' ';
		}
	}
	return input;
}

std::string GebalaCompiler::removeEmptyLines(std::string input) {
	std::istringstream istream(input);
	std::ostringstream compiled;
	std::string line;


	std::regex emptyLine("\\s*");
	while (std::getline(istream, line)) {
		if (!std::regex_match(line, emptyLine)) {
			compiled << line << "\n";
		}
	}

	return compiled.str();
}

std::string GebalaCompiler::trimLines(std::string input)
{
	std::istringstream istream(input);
	std::ostringstream compiled;
	std::string line;


	std::regex content("^\\s*(.*?)\\s*$");
	while (std::getline(istream, line)) {
		compiled << std::regex_replace(line, content, "$1") << "\n";
	}

	return compiled.str();
}

std::string GebalaCompiler::compileCommands(std::string input)
{
	std::istringstream istream(input);
	std::ostringstream compiled;
	std::string line;

	std::regex isLabel("\\S+\\s*:");

	while (std::getline(istream, line)) {
		if (std::regex_match(line, isLabel)) {
			compiled << line << "\n";
		}
		else {
			compiled << compileCommand(line);
		}
	}

	return compiled.str();
}

std::string GebalaCompiler::compileCommand(std::string input)
{
	std::string command;
	std::string operandFirst;
	std::string operandSecond;

	std::regex cmdRegex("^(\\S+)\\s+(\\S+)\\s*(\\S*)");
	std::smatch match;

	std::ostringstream compiled;

	std::regex_match(input, match, cmdRegex);

	const std::string cmd = match[1];

	if (cmd == "GE") {
		compiled << "INC " << getRegexNumber(match[2]) << "\n";
		compiled << "SUB " << getRegexNumber(match[2]) << " " << getRegexNumber(match[3]) << "\n";
	}
	else if (cmd == "GT") {
		compiled << "SUB " << getRegexNumber(match[2]) << " " << getRegexNumber(match[3]) << "\n";
	}
	else if (cmd == "INC") {
		compiled << "INC " << getRegexNumber(match[2]) << "\n";
	}
	else if (cmd == "SET")
	{
		auto registerNumber = getRegexNumber(match[2]);
		std::string constToSetStr = match[3].str();
		const auto constToSet = Number(constToSetStr.c_str());

		compiled << "ZERO " << registerNumber << "\n";

		if(constToSet != 0)
		{
			auto currentBit = Number(1) << (sizeof(Number) * 8 - 1);
			while((currentBit & constToSet) == 0) currentBit >>= 1;
			compiled << "INC " << registerNumber << "\n";
			if(currentBit > 1) compiled << "SHL " << registerNumber << "\n";
			currentBit >>= 1;

			while(currentBit > 0)
			{
				if((currentBit & constToSet) != 0) compiled << "INC " << registerNumber << "\n";
				if(currentBit > 1) compiled << "SHL " << registerNumber << "\n";
				currentBit >>= 1;
			}
		}
	}
	else {
		compiled << input << "\n";
	}
	return compiled.str();
}

std::string GebalaCompiler::getRegexNumber(std::string input)
{
	std::regex num("%r(\\d+)");
	std::smatch match;

	if (!std::regex_match(input, match, num)) {
		return "999";
	}

	return match[1];
}

std::string GebalaCompiler::replaceLabelsWithLines(std::string input)
{
	std::istringstream istream(input);
	std::ostringstream compiled;
	std::string line;

	std::regex isLabel("(\\S+)\\s*:");
	unsigned int lineNum = 0;

  std::map<std::string, unsigned int> labelToLineNum;

	while (std::getline(istream, line)) {
		std::smatch m;
		if (std::regex_match(line, m, isLabel)) {
			labelToLineNum.insert(std::make_pair(m[1], lineNum));
		}
		else {
			compiled << line << "\n";
			++lineNum;
		}
	}

	istream = std::istringstream(compiled.str());
	compiled = std::ostringstream();
	std::regex isLabelInCommand("(.*) [@](\\S+)");

	while (std::getline(istream, line)) {
		std::smatch m;
		if (std::regex_match(line, m, isLabelInCommand)) {
			auto labelLine = labelToLineNum[m[2]];
			line = std::regex_replace(line, isLabelInCommand, std::string("$1 ") + std::to_string(labelLine) );
		}
		compiled << line << "\n";
	}

	return compiled.str();
}

std::string GebalaCompiler::fixRegisterNames(std::string input)
{
	std::regex e("%r");   // matches words beginning by "sub"
  // using string/c-string (3) version:
  return std::regex_replace (input, e, "");
}

std::string GebalaCompiler::addHalt(std::string input)
{
	return input + "HALT";
}
