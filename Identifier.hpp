#pragma once
#include <string>
#include <memory>
#include "Number.hpp"

namespace Calculator {
	class Driver;
}

class Identifier
{
public:
	Identifier(std::string name, Number offset = 0);
	Identifier(std::string name, std::string offsetIdentifierName);

	const std::string getName();

	const std::string loadToRegister(Calculator::Driver & driver, unsigned int registerNumber);
	const std::string loadPositionToRegister(Calculator::Driver & driver, unsigned int registerNumber);

	using IdentifierName = std::string;

	~Identifier();
protected:
	inline bool isConstOffset() {
		return m_offsetIdentifierName == "";
	}
private:
	IdentifierName m_name;
	Number m_offset;
	IdentifierName m_offsetIdentifierName;
};

using IdentifierPtr = std::shared_ptr<Identifier>;
