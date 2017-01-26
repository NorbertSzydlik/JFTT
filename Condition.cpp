#include "Condition.hpp"
#include <sstream>

Condition::Condition(Type type, IdentifierPtr leftIdentifier, IdentifierPtr rightIdentifier)
{
	m_type = type;
	m_operands = Operands::IDENTIFIER_IDENTIFIER;
	m_leftIdentifier = leftIdentifier;
	m_rightIdentifier = rightIdentifier;
}

Condition::Condition(Type type, Number leftNumber, IdentifierPtr rightIdentifier)
{
	m_type = type;
	m_operands = Operands::NUMBER_IDENTIFIER;
	m_leftNumber = leftNumber;
	m_rightIdentifier = rightIdentifier;
}

Condition::Condition(Type type, IdentifierPtr leftIdentifier, Number rightNumber)
{
	m_type = type;
	m_operands = Operands::IDENTIFIER_NUMBER;
	m_leftIdentifier = leftIdentifier;
	m_rightNumber = rightNumber;
}

Condition::Condition(Type type, Number leftNumber, Number rightNumber)
{
	m_type = type;
	m_operands = Operands::NUMBER_NUMBER;
	m_leftNumber = leftNumber;
	m_rightNumber = rightNumber;
}

Condition::~Condition()
{
}

std::string Condition::evaluate(Calculator::Driver& driver, unsigned int registerNumber, unsigned int tmpRegister)
{
	std::ostringstream compiled;

	std::string command;
	switch (m_type) {
	case Type::OP_EQ: command = "EQ"; break;
	case Type::OP_NEQ: command = "NEQ"; break;
	case Type::OP_GE: command = "GE"; break;
	case Type::OP_GT: command = "GT"; break;
	}

	compiled << "#begin of condition " << command << "\n";

	if (m_operands == Operands::IDENTIFIER_IDENTIFIER || m_operands == Operands::IDENTIFIER_NUMBER) {
		compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
	}
	else {
		compiled << "SET %r" << registerNumber << " " << m_leftNumber << " #left number is constant\n";
	}

	if (m_operands == Operands::IDENTIFIER_IDENTIFIER || m_operands == Operands::NUMBER_IDENTIFIER) {
		compiled << m_rightIdentifier->loadToRegister(driver, tmpRegister);
	}
	else {
		compiled << "SET %r" << tmpRegister << " " << m_rightNumber << " #right number is constant\n";
	}

	compiled << command << " %r" << registerNumber << " %r" << tmpRegister << " #execute operation in condition\n";

	return compiled.str();
}
