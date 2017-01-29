#include "ExpressionNumber.hpp"
#include <sstream>

#include "logging.h"

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
	LOG("Load number " << m_number << " to register " << registerNumber);
	compiled << "SET %r" << registerNumber << " " << m_number << " #evaluateToRegister expression number " << m_number << "\n";
	return compiled.str();
}
