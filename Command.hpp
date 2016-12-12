#pragma once

#include <string>
#include <memory>
#include <vector>
#include "CodeBlock.hpp"

namespace Calculator {
	class Driver;
}

class Command
{
public:
	Command();
	virtual ~Command();
	virtual std::string compile(Calculator::Driver & driver) = 0;
	virtual CodeBlockPtr getCodeBlock(Calculator::Driver & driver) = 0;
	virtual std::string getCommandName() = 0;
};

using CommandPtr = std::shared_ptr<Command>;
using CommandPtrs = std::vector<CommandPtr>;

