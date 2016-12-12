#include "ExpressionNumber.hpp"
#include <sstream>

ExpressionNumber::ExpressionNumber(Number number)
{
	m_number = number;
}


ExpressionNumber::~ExpressionNumber()
{
}

std::string ExpressionNumber::evaluateToRegister(Calculator::Driver& driver, unsigned int  registerNumber)
{
	std::ostringstream compiled;
	compiled << "LOAD %r" << registerNumber << " " << m_number << " #expression number " << m_number << "\n";
	return compiled.str();
}
