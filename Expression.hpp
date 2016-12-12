#pragma once
#include <string>
#include <memory>

namespace Calculator {
	class Driver;
}
class Expression
{
public:
	Expression();
	virtual ~Expression();
	virtual std::string evaluateToRegister(Calculator::Driver& driver, unsigned int  registerNumber) = 0;
};

using ExpressionPtr = std::shared_ptr<Expression>;