#pragma once
#include "Command.hpp"
#include "Condition.hpp"

class CommandIfElse :
	public Command
{
public:
	CommandIfElse(ConditionPtr condition, CommandPtrs const & ifCommands, CommandPtrs const & elseCommands);
	CommandIfElse(ConditionPtr condition, CommandPtrs const & ifCommands);

	~CommandIfElse();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	ConditionPtr m_condition;
	CommandPtrs m_ifCommands;
	CommandPtrs m_elseCommands;

};

