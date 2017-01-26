#include "GebalaCompiler.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <map>

GebalaCompiler::GebalaCompiler()
{
}

GebalaCompiler::~GebalaCompiler()
{
}

std::string GebalaCompiler::compile(std::string input) {
	input = removeComments(input);
	input = removeEmptyLines(input);
	input = trimLines(input);
	input = compileCommands(input);
	input = replaceLabelsWithLines(input);
	input = addHalt(input);
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
	else if (cmd == "ADD" || cmd == "SUB") {
		compiled << cmd << " " << getRegexNumber(match[2]) << " " << getRegexNumber(match[3]) << "\n";
	}
	else if (cmd == "SET")
	{
		auto registerNumber = getRegexNumber(match[2]);
		const auto constToSet = Number(match[3]);
		compiled << "ZERO " << registerNumber << "\n";
		sconstToSet.numberOfDigits();
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
	std::regex isLabelInCommand("(.*)[@](\\S+)");

	while (std::getline(istream, line)) {
		std::smatch m;
		if (std::regex_match(line, m, isLabelInCommand)) {
			auto labelLine = labelToLineNum[m[2]];
			line = std::regex_replace (line, isLabelInCommand, std::string("$1 ") + std::to_string(labelLine) );
		}
		compiled << line << "\n";
	}

	return compiled.str();
}

std::string GebalaCompiler::addHalt(std::string input)
{
	return input + "HALT";
}
