#pragma once
#include "Command.hpp"
#include "Condition.hpp"

class CommandSkip :
	public Command
{
public:
	CommandSkip();
	virtual ~CommandSkip();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	ConditionPtr m_condition;
	CommandPtrs m_commands;
};
