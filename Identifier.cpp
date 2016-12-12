#include <sstream>
#include "Identifier.hpp"
#include "driver.hpp"

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
	compiled << loadPositionToRegister(driver, registerNumber);
	compiled << "LOAD %r" << registerNumber << " %r" << registerNumber << " #load value from memory pointed by the same register\n";
	
	return compiled.str();
}

const std::string Identifier::loadPositionToRegister(Calculator::Driver & driver, unsigned int registerNumber)
{
	std::ostringstream compiled;
	Number position;
	if (isConstOffset()) {
		position = driver.getDeclarationPosition(m_name);
		position += m_offset;
		compiled << "LOAD %r" << registerNumber << " " << position << " #memory position of identifier " << m_name << "\n";
	}
	else {
		position = driver.getDeclarationPosition(m_name);
		unsigned int tmpRegister = registerNumber == 1 ? 2 : 1;
		auto offsetPosition = driver.getDeclarationPosition(m_offsetIdentifierName);
		compiled << "LOAD %r" << registerNumber << " " << position << " #load start position of identifier " << m_name << "\n";
		compiled << "LOAD %r" << tmpRegister << " $" << offsetPosition << " #load offset from memory pointed by offset identifier " << m_offsetIdentifierName << "\n";
		compiled << "ADD %r" << registerNumber << " %r" << tmpRegister << " #memory position of identifier " << m_name << "(" << m_offsetIdentifierName << ")\n";
	}
	return compiled.str();
}
