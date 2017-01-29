#include <algorithm>
#include <cassert>
#include "driver.hpp"

#include "Command.hpp"
#include "ExpressionOperation.hpp"
#include "CommandWhile.hpp"
#include "Condition.hpp"
#include "CommandAssign.hpp"
#include "ExpressionNumber.hpp"
#include "ExpressionIdentifier.hpp"
#include "CommandIfElse.hpp"
#include "CommandPut.hpp"

#include "logging.h"

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

	compiled << "#begin of expression operation" << "\n";

  if(m_operands == Operands::NUMBER_NUMBER)
	{
		compiled << "SET %r" << registerNumber << " " << evaluateConstNumbers() << "\n";
	}
	else
	{
		if(m_type == Type::OP_ADD || m_type == Type::OP_SUB) compiled << evaluateSubAdd(driver, registerNumber);
		else if(m_type == Type::OP_DIV || m_type == Type::OP_MOD) compiled << evaluateDivMod(driver, registerNumber);
		else if(m_type == Type::OP_MUL) compiled << evaluateMul(driver, registerNumber);
	}

	compiled << "#end of expression operation" << "\n";

	return compiled.str();
}

Number ExpressionOperation::evaluateConstNumbers()
{
	switch (m_type) {
	case Type::OP_ADD: return m_leftNumber + m_rightNumber;
	case Type::OP_SUB: return std::max(m_leftNumber - m_rightNumber, Number(0));
	case Type::OP_MUL: return m_leftNumber * m_rightNumber;
	case Type::OP_DIV: return m_rightNumber != 0 ? cln::truncate1(m_leftNumber / m_rightNumber) : 0;
	case Type::OP_MOD: return m_rightNumber != 0 ? cln::mod(m_leftNumber, m_rightNumber) : 0;
	}
}

std::string ExpressionOperation::evaluateSubAdd(Calculator::Driver& driver, unsigned int registerNumber)
{
	std::ostringstream compiled;
	std::string command;

  compiled << "# operation sub/add" << "\n";

	switch (m_type) {
	  case Type::OP_ADD: command = "ADD"; break;
	  case Type::OP_SUB: command = "SUB"; break;
		default: assert(false && "wrong operation");
	}
	if(m_operands == Operands::IDENTIFIER_IDENTIFIER)
	{
		compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
		compiled << m_rightIdentifier->loadPositionToRegister(driver, 2);
		compiled << "COPY %r2\n";

		compiled << command << " %r" << registerNumber << " #execute operation in expression\n";
	}
	else if(m_operands == Operands::NUMBER_IDENTIFIER)
	{
		compiled << m_rightIdentifier->loadPositionToRegister(driver, 2);
		compiled << "COPY %r2\n";

		compiled << "SET %r" << registerNumber << " " << m_leftNumber << "\n";

		compiled << command << " %r" << registerNumber << " #execute operation in expression\n";
	}
	else if(m_operands == Operands::IDENTIFIER_NUMBER)
	{
		if(m_rightNumber <= 30)
		{
			compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
			for(auto i = m_rightNumber; i > 0; --i)
			{
				compiled << (m_type == Type::OP_ADD ? "INC" : "DEC") << " %r" << registerNumber << "\n";
			}
		}
		else
		{
			auto tmpPosition = driver.getTmpMemoryPosition();
			compiled << "#prepare temporary memory allocation for const number on right side of evaluation\n";
			compiled << "SET %r0 " << tmpPosition << " #temporary memory position\n";
			compiled << "SET %r" << registerNumber << " " << m_rightNumber << " #right hand value " << m_rightNumber << "\n";
			compiled << "STORE " << registerNumber << " #store right hand value to temporary position\n";

			compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
			compiled << "SET %r0 " << tmpPosition << " #restore temporary position to address register\n";

			compiled << command << " %r" << registerNumber << " #execute operation in expression\n";
		}
	}
	return compiled.str();
}

std::string ExpressionOperation::evaluateDivMod(Calculator::Driver& driver, unsigned int registerNumber)
{
	LOG(__FUNCTION__  << " " << m_leftIdentifier->getName() << " " << (int)m_operands << " " << m_rightNumber);
	std::ostringstream compiled;
	compiled << "# operation div/mod\n";
	if(isRightOperandNumber())
	{
		LOG("RIGHT operand number");
		if(m_rightNumber == 0)
		{
			LOG("RIGHT operand == 0");
			compiled << "SET %r" << registerNumber << " 0\n";
			return compiled.str();
		}
		if(m_rightNumber == 1)
		{
			LOG("RIGHT operand == 1");
			compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
			return compiled.str();
		}
		if(m_rightNumber == 2)
		{
			LOG("RIGHT operand == 2");
			compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
			compiled << "SHR %r" << registerNumber << "\n";
			return compiled.str();
		}
	}

	IdentifierPtr left;
	IdentifierPtr right;

  driver.declare("_tmp_");

	if(isLeftOperandNumber())
	{
		right = m_rightIdentifier;
		left = std::make_shared<Identifier>("_tmp_");

		compiled << left->loadPositionToRegister(driver, 2);
		compiled << "COPY %r2\n";
		compiled << "SET %r" << registerNumber << " " << m_leftNumber << " #set constant number\n";
		compiled << "STORE %r" << registerNumber << "\n";
	}
	else if(isRightOperandNumber())
	{
		compiled << "# " << m_leftIdentifier->getName() << " / " << m_rightNumber << "\n";
		left = m_leftIdentifier;
		right = std::make_shared<Identifier>("_tmp_");

		compiled << right->loadPositionToRegister(driver, 2);
		compiled << "COPY %r2\n";
		compiled << "SET %r" << registerNumber << " " << m_rightNumber << " #set constant number\n";
		compiled << "STORE %r" << registerNumber << "\n";
	}
	else
	{
		left = m_leftIdentifier;
		right = m_rightIdentifier;
	}

	compiled << "# divQ := 0;\n";
	compiled << "# divR := left;\n";
	compiled << "# divD := right;\n";
	compiled << "# divN := left;\n";
	compiled << "# WHILE divN >= divD DO\n";
	compiled << "#   divD := divD * 2;\n";
	compiled << "# ENDWHILE\n";
	compiled << "# divD := divD / 2;\n";

	driver.declare("div_Q");
	auto divQ = std::make_shared<Identifier>("div_Q");
	auto cmdAssignQ = std::make_shared<CommandAssign>(divQ, std::make_shared<ExpressionNumber>(0) );
	compiled << cmdAssignQ->compile(driver);

	driver.declare("div_R");
	auto divR = std::make_shared<Identifier>("div_R");
	auto cmdAssignR = std::make_shared<CommandAssign>(divR, std::make_shared<ExpressionIdentifier>(left) );
	compiled << cmdAssignR->compile(driver);

	driver.declare("div_D");
	auto divD = std::make_shared<Identifier>("div_D");
	auto cmdAssignD = std::make_shared<CommandAssign>(divD, std::make_shared<ExpressionIdentifier>(right) );
	compiled << cmdAssignD->compile(driver);

	//auto writeD = std::make_shared<CommandPut>(divD);
	//compiled << writeD->compile(driver);

	driver.declare("div_N");
	auto divN = std::make_shared<Identifier>("div_N");
	auto cmdAssignN = std::make_shared<CommandAssign>(divN, std::make_shared<ExpressionIdentifier>(left) );
	compiled << cmdAssignN->compile(driver);

	ConditionPtr condition;
	condition = std::make_shared<Condition>(Condition::Type::OP_GE, divN, divD);

	CommandPtrs commands;
	commands.push_back( std::make_shared<CommandAssign>(divD, std::make_shared<ExpressionOperation>(Type::OP_MUL, divD, 2)) );
	//commands.push_back( writeD );

	auto whileCmd = std::make_shared<CommandWhile>(condition, commands);

	compiled << whileCmd->compile(driver);
  //compiled << writeD->compile(driver);

	auto divDBy2 = std::make_shared<CommandAssign>(divD, std::make_shared<ExpressionOperation>(Type::OP_DIV, divD, 2));
	compiled << divDBy2->compile(driver);
	//compiled << writeD->compile(driver);

	compiled << "# WHILE divD >= right\n";
	compiled << "#  IF divN >= divD THEN\n";
	compiled << "#    divQ := divQ + 1;\n";
	compiled << "#    divN := divN - divD;\n";
	compiled << "#  ELSE\n";
	compiled << "#  ENDIF\n";
	compiled << "#  divQ = divQ * 2;\n";
	compiled << "#  divD = divD / 2;\n";
	compiled << "# ENDWHILE\n";

	//auto writeQ = std::make_shared<CommandPut>(divQ);

	auto mulQBy2 = std::make_shared<CommandAssign>(divQ, std::make_shared<ExpressionOperation>(Type::OP_MUL, divQ, 2));
	auto incrementQ = std::make_shared<CommandAssign>(divQ, std::make_shared<ExpressionOperation>(Type::OP_ADD, divQ, 1));

  condition = std::make_shared<Condition>(Condition::Type::OP_GE, divD, right);
	auto conditionInIf = std::make_shared<Condition>(Condition::Type::OP_GE, divN, divD);
	commands.clear();

  auto ifElse = std::make_shared<CommandIfElse>(conditionInIf, CommandPtrs(
	{
	  incrementQ,
		//writeQ,
		std::make_shared<CommandAssign>(divN, std::make_shared<ExpressionOperation>(Type::OP_SUB, divN, divD))
	}));
  commands.push_back(ifElse);
  commands.push_back(mulQBy2);
	commands.push_back(divDBy2);
	whileCmd = std::make_shared<CommandWhile>(condition, commands);

  compiled << whileCmd->compile(driver);
	auto divQBy2 = std::make_shared<CommandAssign>(divQ, std::make_shared<ExpressionOperation>(Type::OP_DIV, divQ, 2));
	compiled << divQBy2->compile(driver);

	switch (m_type) {
	  case Type::OP_DIV: compiled << divQ->loadToRegister(driver, registerNumber); break;
	  case Type::OP_MOD: compiled << divN->loadToRegister(driver, registerNumber); break;
		default: LOG("wrong operation"); assert(false);
	}

  driver.undeclare("_tmp_");
  driver.undeclare("div_Q");
	driver.undeclare("div_R");
	driver.undeclare("div_D");
	driver.undeclare("div_N");

	return compiled.str();
}

std::string ExpressionOperation::evaluateMul(Calculator::Driver& driver, unsigned int registerNumber)
{
	std::ostringstream compiled;
	compiled << "# operation mul" << "\n";

	if(isRightOperandNumber() && m_rightNumber == 2)
	{
		compiled << m_leftIdentifier->loadToRegister(driver, registerNumber);
		compiled << "SHL %r" << registerNumber << "\n";
	}
	else
	{
		compiled << "# MUL NOT IMPLEMENTED YET";
	}

	return compiled.str();
}
