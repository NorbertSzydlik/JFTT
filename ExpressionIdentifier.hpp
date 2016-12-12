#pragma once
#include "Expression.hpp"
#include "Identifier.hpp"

class ExpressionIdentifier : public Expression
{
public:
	ExpressionIdentifier(IdentifierPtr const & identifier);
	~ExpressionIdentifier();
	std::string evaluateToRegister(Calculator::Driver& driver, unsigned int  registerNumber) override;
private:
	IdentifierPtr m_identifier;
};

