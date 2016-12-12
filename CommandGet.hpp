#pragma once
#include <string>
#include "driver.hpp"
#include "Command.hpp"
#include "Identifier.hpp"

class CommandGet : public Command
{
public:
	CommandGet(IdentifierPtr const & identifier);
	virtual ~CommandGet();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	IdentifierPtr m_identifier;
};

