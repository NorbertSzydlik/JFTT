#pragma once
#include "Expression.hpp"
#include "Identifier.hpp"
#include "Number.hpp"

class ExpressionOperation : public Expression
{
public:
	enum class Type {
		OP_ADD,
		OP_SUB,
		OP_MUL,
		OP_DIV,
		OP_MOD
	};

	ExpressionOperation(Type type, IdentifierPtr leftIdentifier, IdentifierPtr rightIdentifier);
	ExpressionOperation(Type type, IdentifierPtr leftIdentifier, Number rightNumber);
	ExpressionOperation(Type type, Number leftNumber, IdentifierPtr rightIdentifier);
	ExpressionOperation(Type type, Number leftNumber, Number rightNumber);

	~ExpressionOperation();
	std::string evaluateToRegister(Calculator::Driver& driver, unsigned int registerNumber) override;

private:
	Number evaluateConstNumbers();
	enum class Operands {
		IDENTIFIER_IDENTIFIER,
		IDENTIFIER_NUMBER,
		NUMBER_IDENTIFIER,
		NUMBER_NUMBER
	};
	Type m_type;
	Operands m_operands;
	IdentifierPtr m_leftIdentifier;
	IdentifierPtr m_rightIdentifier;
	Number m_leftNumber;
	Number m_rightNumber;
};
