#include <algorithm>
#include "driver.hpp"
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

std::string ExpressionOperation::evaluateToRegister(Calculator::Driver& driver, unsigned int registerNumber)
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

	if(m_operands == Operands::IDENTIFIER_IDENTIFIER)
	{
		compiled << m_rightIdentifier->loadPositionToRegister(driver, 0);
		compiled << m_leftIdentifier->loadPositionToRegister(driver, registerNumber);

		compiled << command << " %r" << registerNumber << " #execute operation in expression\n";
	}
	else if(m_operands == Operands::NUMBER_IDENTIFIER)
	{
		compiled << m_rightIdentifier->loadPositionToRegister(driver, 0);
		compiled << "SET %r" << registerNumber << " " << m_leftNumber << "\n";

		compiled << command << " %r" << registerNumber << " #execute operation in expression\n";
	}
	else if(m_operands == Operands::IDENTIFIER_NUMBER)
	{
		compiled << "#prepare temporary memory allocation for const number on right side of evaluation\n";
		compiled << "SET %r0 " << driver.getTmpMemoryPosition() << " #temporary memory position\n";
		compiled << "SET %r" << registerNumber << " " << m_rightNumber << " #divide by " << m_rightNumber << "\n";
		compiled << "STORE " << registerNumber << "\n";

		compiled << m_leftIdentifier->loadPositionToRegister(driver, registerNumber);

		compiled << command << " %r" << registerNumber << " #execute operation in expression\n";
	}
	else if(m_operands == Operands::NUMBER_NUMBER)
	{
		compiled << "SET %r" << registerNumber << " " << evaluateConstNumbers() << "\n";
	}

	return compiled.str();
}

Number ExpressionOperation::evaluateConstNumbers()
{
	switch (m_type) {
	case Type::OP_ADD: return m_leftNumber + m_rightNumber;
	case Type::OP_SUB: return std::max(m_leftNumber - m_rightNumber, Number(0));
	case Type::OP_MUL: return m_leftNumber * m_rightNumber;
	case Type::OP_DIV: return m_rightNumber != 0 ? m_leftNumber / m_rightNumber : 0;
	case Type::OP_MOD: return m_rightNumber != 0 ? m_leftNumber % m_rightNumber : 0;
	}
}
