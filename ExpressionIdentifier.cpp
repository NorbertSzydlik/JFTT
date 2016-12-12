#include "ExpressionIdentifier.hpp"
#include <sstream>

ExpressionIdentifier::ExpressionIdentifier(IdentifierPtr const & identifier) : m_identifier(identifier)
{
}

ExpressionIdentifier::~ExpressionIdentifier()
{
}

std::string ExpressionIdentifier::evaluateToRegister(Calculator::Driver& driver, unsigned int registerNumber)
{
	return m_identifier->loadToRegister(driver, registerNumber);
}