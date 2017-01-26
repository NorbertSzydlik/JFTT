#include <sstream>
#include "Identifier.hpp"
#include "driver.hpp"

const unsigned int ADDRESS_REGISTER = 0;

Identifier::Identifier(std::string name, Number offset)
{
	m_name = name;
	m_offset = offset;
	m_offsetIdentifierName = "";
}

Identifier::Identifier(std::string name, std::string offsetIdentifierName)
{
	m_name = name;
	m_offset = 0;
	m_offsetIdentifierName = offsetIdentifierName;
}

Identifier::~Identifier()
{
}

const std::string Identifier::getName() {
	return m_name;
}

const std::string Identifier::loadToRegister(Calculator::Driver & driver, unsigned int registerNumber)
{
	std::ostringstream compiled;
	compiled << loadPositionToRegister(driver, ADDRESS_REGISTER);
	compiled << "LOAD %r" << registerNumber << " #load value from memory pointed by address register\n";

	return compiled.str();
}

const std::string Identifier::loadPositionToRegister(Calculator::Driver & driver, unsigned int registerNumber)
{
	std::ostringstream compiled;
	Number position;
	if (isConstOffset()) {
		position = driver.getDeclarationPosition(m_name);
		position += m_offset;
		compiled << "SET %r" << registerNumber << " " << position << " #memory position of identifier " << m_name << "\n";
	}
	else {
		position = driver.getDeclarationPosition(m_name);
		auto offsetPosition = driver.getDeclarationPosition(m_offsetIdentifierName);

		compiled << "SET %r0 " << offsetPosition << "\n";
		compiled << "SET %r" << registerNumber << " " << position << "\n";
		compiled << "ADD %r" << registerNumber << "\n";

	}
	return compiled.str();
}
