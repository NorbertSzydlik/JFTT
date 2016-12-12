#include "ExpressionOperation.hpp"

ExpressionOperation::ExpressionOperation(Type type, IdentifierPtr leftIdentifier, IdentifierPtr rightIdentifier) :
	m_type(type),
	m_operands(Operands::IDENTIFIER_IDENTIFIER),
	m_leftIdentifier(leftIdentifier),
	m_rightIdentifier(rightIdentifier)
{
}

ExpressionOperation::ExpressionOperation(Type type, Number leftNumber, IdentifierPtr rightIdentifier) :
	m_type(type),
	m_operands(Operands::NUMBER_IDENTIFIER),
	m_rightIdentifier(rightIdentifier),
	m_leftNumber(leftNumber)
{
}

ExpressionOperation::ExpressionOperation(Type type, IdentifierPtr leftIdentifier, Number rightNumber) :
	m_type(type),
	m_operands(Operands::IDENTIFIER_NUMBER),
	m_leftIdentifier(leftIdentifier),
	m_rightNumber(rightNumber)
{
}

ExpressionOperation::ExpressionOperation(Type type, Number leftNumber, Number rightNumber) :
	m_type(type),
	m_operands(Operands::NUMBER_NUMBER),
	m_leftNumber(leftNumber),
	m_rightNumber(rightNumber)
{
}

ExpressionOperation::~ExpressionOperation()
{
}

std::string ExpressionOperation::evaluateToRegister(Calculator::Driver & driver, unsigned int registerNumber)
{
	std::ostringstream compiled;

	std::string command;
	switch (m_type) {
	case Type::OP_ADD: command = "ADD"; break;
	case Type::OP_SUB: command = "SUB"; break;
	case Type::OP_MUL: command = "MUL"; break;
	case Type::OP_DIV: command = "DIV"; break;
	case Type::OP_MOD: command = "MOD"; break;
	}

	compiled << "#begin of expression " << command << "\n";

	unsigned int tmpRegister = registerNumber == 1 ? 0 : 1;

	if (m_operands == Operands::IDENTIFIER_IDENTIFIER || m_operands == Operands::IDENTIFIER_NUMBER) {
		compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
	}
	else {
		compiled << "LOAD %r" << registerNumber << " " << m_leftNumber << "\n";
	}

	if (m_operands == Operands::IDENTIFIER_IDENTIFIER || m_operands == Operands::NUMBER_IDENTIFIER) {
		compiled << m_rightIdentifier->loadToRegister(driver, tmpRegister);
	}
	else {
		compiled << "LOAD %r" << tmpRegister << " " << m_rightNumber << "\n";
	}

	compiled << command << " %r" << registerNumber << " %r" << tmpRegister << " #execute operation in expression\n";

	return compiled.str();
}
