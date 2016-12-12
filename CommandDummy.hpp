#pragma once
#include "Command.hpp"
#include "driver.hpp"

class CommandDummy :
	public Command
{
public:
	CommandDummy(const std::string dummy);
	virtual ~CommandDummy();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	const std::string m_dummy;
};

