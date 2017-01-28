#include <sstream>
#include "driver.hpp"
#include "Condition.hpp"

Condition::Condition(Type type, IdentifierPtr leftIdentifier, IdentifierPtr rightIdentifier)
{
	m_type = type;
	m_operands = Operands::IDENTIFIER_IDENTIFIER;
	m_leftIdentifier = leftIdentifier;
	m_rightIdentifier = rightIdentifier;
}

Condition::Condition(Type type, Number leftNumber, IdentifierPtr rightIdentifier)
{
	m_type = type;
	m_operands = Operands::NUMBER_IDENTIFIER;
	m_leftNumber = leftNumber;
	m_rightIdentifier = rightIdentifier;
}

Condition::Condition(Type type, IdentifierPtr leftIdentifier, Number rightNumber)
{
	m_type = type;
	m_operands = Operands::IDENTIFIER_NUMBER;
	m_leftIdentifier = leftIdentifier;
	m_rightNumber = rightNumber;
}

Condition::Condition(Type type, Number leftNumber, Number rightNumber)
{
	m_type = type;
	m_operands = Operands::NUMBER_NUMBER;
	m_leftNumber = leftNumber;
	m_rightNumber = rightNumber;
}

Condition::~Condition()
{
}

std::string Condition::evaluate(Calculator::Driver& driver, unsigned int registerNumber, unsigned int tmpRegister)
{
	std::ostringstream compiled;

	std::string command;
  switch (m_type) {
    case Type::OP_EQ: command = "EQ"; break;
    case Type::OP_NEQ: command = "NEQ"; break;
    case Type::OP_GE: command = "GE"; break;
    case Type::OP_GT: command = "GT"; break;
  }


	compiled << "#begin of condition " << command << "\n";

	if(m_operands == Operands::NUMBER_NUMBER)
	{
		bool result;
		switch (m_type) {
			case Type::OP_EQ: result = m_leftNumber == m_rightNumber; break;
			case Type::OP_NEQ: result = m_leftNumber != m_rightNumber; break;
			case Type::OP_GE: result = m_leftNumber >= m_rightNumber; break;
			case Type::OP_GT: result = m_leftNumber > m_rightNumber; break;
		}
		compiled << "ZERO %r" << registerNumber << "#reset result registern\n";
		if(!result)
		{
			compiled << "INC %r" << registerNumber << "#set 1 to result register to indicate false\n";
		}
	}
  else
	{
		IdentifierPtr left;
		IdentifierPtr right;

		auto memTmpLocation = driver.getTmpMemoryPosition();

		if(m_operands == Operands::IDENTIFIER_IDENTIFIER)
		{
			left = m_leftIdentifier;
			right = m_rightIdentifier;
		}
		else if(m_operands == Operands::IDENTIFIER_NUMBER)
		{
			left = m_leftIdentifier;
			right = std::make_shared<Identifier>("_tmpRight_", memTmpLocation);

			compiled << right->loadPositionToRegister(driver, 2);
			compiled << "COPY %r2\n";
			compiled << "SET %r" << registerNumber << " " << m_rightNumber << " #set constant number\n";
			compiled << "STORE %r" << registerNumber << "\n";
		}
		else
		{
			left = std::make_shared<Identifier>("_tmpLeft_", memTmpLocation);
			right = m_rightIdentifier;

			compiled << left->loadPositionToRegister(driver, 2);
			compiled << "COPY %r2\n";
			compiled << "SET %r" << registerNumber << " " << m_leftNumber << " #set constant number\n";
			compiled << "STORE %r" << registerNumber << "\n";
		}
		compiled << right->loadToRegister(driver, registerNumber);
		compiled << left->loadPositionToRegister(driver, 2);
		compiled << "COPY %r2\n";

		auto labelEnd = driver.getNextLabelFor("op_eq_end");
		auto labelOk = driver.getNextLabelFor("op_eq_ok");
		auto labelNeqNOk = driver.getNextLabelFor("op_neq_nok");

		switch(m_type)
		{
			case Type::OP_GT:
			  compiled << "INC %r" << registerNumber << "\n";
				compiled << "SUB %r" << registerNumber << "\n";
			  break;
			case Type::OP_GE:
			  compiled << "SUB %r" << registerNumber << "\n";
			  break;
			case Type::OP_EQ:
			  compiled << "SUB %r" << registerNumber << "\n";

				compiled << "JZERO %r" << registerNumber << " @" << labelOk << " #if b - a = 0 - is ok\n";
				compiled << "JUMP @" << labelEnd << " #previous condition failed, a!=b \n";
				compiled << labelOk << ":\n";

				compiled << left->loadToRegister(driver, registerNumber);
			  compiled << right->loadPositionToRegister(driver, 2);
			  compiled << "COPY %r2\n";

				compiled << "SUB %r" << registerNumber << " #check if a - b = 0\n";
				compiled << labelEnd << ":\n";
			  break;
			case Type::OP_NEQ:
				compiled << "SUB %r" << registerNumber << "\n";

				compiled << "JZERO %r" << registerNumber << " @" << labelOk << " #We know that a >= b. Check if a < b still needed\n";
				compiled << "ZERO %r" << registerNumber << " #At this moment it's certain that a < b, so condition passed, need to reset register to 0 to indicate true\n";
				compiled << "JUMP @" << labelEnd << "\n";
				compiled << labelOk << ":\n";

				compiled << left->loadToRegister(driver, registerNumber);
				compiled << right->loadPositionToRegister(driver, 2);
				compiled << "COPY %r2\n";

				compiled << "SUB %r" << registerNumber << " #check if a - b = 0\n";

				compiled << "JZERO %r" << registerNumber << " @" << labelNeqNOk << " #jump if a == b \n";
				compiled << "ZERO %r" << registerNumber << " #a != b, so we need to reset register to indicate true\n";
				compiled << "JUMP @" << labelEnd << "#make sure to ommit NOK scenario\n";

				compiled << labelNeqNOk << ":\n";
			  compiled << "INC %r" << registerNumber << " #need to set not zero value to register to indicate false\n";

				compiled << labelEnd << ":\n";
		}
	}

	return compiled.str();
}
