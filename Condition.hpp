#pragma once
#include <memory>
#include <vector>
#include "Identifier.hpp"
#include "Number.hpp"

namespace Calculator {
	class Driver;
}

class Condition
{
public:
	enum class Type {
		OP_GT,
		OP_GE,
		OP_EQ,
		OP_NEQ
	};

	Condition(Type type, IdentifierPtr leftIdentifier, IdentifierPtr rightIdentifier);
	Condition(Type type, IdentifierPtr leftIdentifier, Number rightNumber);
	Condition(Type type, Number leftNumber, IdentifierPtr rightIdentifier);
	Condition(Type type, Number leftNumber, Number rightNumber);

	~Condition();
	std::string evaluate(Calculator::Driver& driver, unsigned int  registerNumber);
private:
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

using ConditionPtr = std::shared_ptr<Condition>;
using ConditionPtrs = std::vector<ConditionPtr>;