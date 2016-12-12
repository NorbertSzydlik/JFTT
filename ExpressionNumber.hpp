#pragma once
#include "Expression.hpp"
#include "Number.hpp"

class ExpressionNumber : public Expression
{
public:
	ExpressionNumber(Number number);
	~ExpressionNumber();
	std::string evaluateToRegister(Calculator::Driver& driver, unsigned int  registerNumber) override;
private:
	Number m_number;
};

