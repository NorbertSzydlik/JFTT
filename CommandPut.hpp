#pragma once
#include <string>
#include "driver.hpp"
#include "Command.hpp"
#include "Identifier.hpp"
#include "Number.hpp"

class CommandPut : public Command
{
public:
	CommandPut(IdentifierPtr const & identifier);
	CommandPut(Number number);

	virtual ~CommandPut();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	IdentifierPtr m_identifier;
	Number m_number;
	bool m_isConst;
};

