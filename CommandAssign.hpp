#pragma once
#include "Command.hpp"
#include "Identifier.hpp"
#include "Expression.hpp"

class CommandAssign :
	public Command
{
public:
	CommandAssign(IdentifierPtr const & identifier, ExpressionPtr const & expression);

	~CommandAssign();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
private:
	IdentifierPtr m_identifier;
	ExpressionPtr m_expression;
};

