#pragma once
#include "Command.hpp"
#include "Condition.hpp"

class CommandWhile :
	public Command
{
public:
	CommandWhile(ConditionPtr condition, CommandPtrs const & commands);
	virtual ~CommandWhile();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	ConditionPtr m_condition;
	CommandPtrs m_commands;
};

