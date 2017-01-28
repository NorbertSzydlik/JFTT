#include "CommandIfElse.hpp"
#include "driver.hpp"

const unsigned int WORK_REGISTER = 1;
const unsigned int TMP_REGISTER = 2;


CommandIfElse::CommandIfElse(ConditionPtr condition, CommandPtrs const & ifCommands, CommandPtrs const & elseCommands)
{
	m_condition = condition;
	m_ifCommands = ifCommands;
	m_elseCommands = elseCommands;
}

CommandIfElse::CommandIfElse(ConditionPtr condition, CommandPtrs const & ifCommands)
{
	m_condition = condition;
	m_ifCommands = ifCommands;
	m_elseCommands = CommandPtrs();
}

CommandIfElse::~CommandIfElse()
{
}

std::string CommandIfElse::compile(Calculator::Driver & driver) {
	std::ostringstream compiled;

  std::string conditionOkLabel = driver.getNextLabelFor("if_ok");
	std::string elseLabel = driver.getNextLabelFor("else");
	std::string endLabel = driver.getNextLabelFor("if_end");

	compiled << "#if-else block\n";

	compiled << m_condition->evaluate(driver, WORK_REGISTER, TMP_REGISTER);

	compiled << "JZERO %r" << WORK_REGISTER << " @" << conditionOkLabel << " #jump to condition ok label when condition is false\n";
	compiled << "JUMP @" << elseLabel << " #false - jump to else\n";
	compiled << conditionOkLabel << ": #if part\n";
	for (auto& cmd : m_ifCommands) {
		std::cout << "test: if-else(if): before: " << cmd->getCommandName() << std::endl;
		compiled << cmd->compile(driver);
		std::cout << "test: if-else(if): after: " << cmd->getCommandName() << std::endl;
	}
	std::cout << "after if commands" << std::endl;
	compiled << "JUMP @" << endLabel << " #jump to end label, as we need to execute only if part\n";
	compiled << "#else part\n";
	compiled << elseLabel << ": #else label\n";

	std::cout << "before elseCommands: " << m_elseCommands[0]->getCommandName() << std::endl;
	for (auto& cmd : m_elseCommands) {\
		std::cout << "test: if-else(else): before: " << cmd->getCommandName() << std::endl;
		compiled << cmd->compile(driver);
		std::cout << "test: if-else(else): after: " << cmd->getCommandName() << std::endl;
	}
	std::cout << "after elseCommands" << std::endl;

	compiled << endLabel << ": #if-else end label\n";
	compiled << "#end of if-else block\n";

	std::cout << "if else end" << std::endl;
	return compiled.str();
}

std::string CommandIfElse::getCommandName()
{
	return "Command if-else";
}

CodeBlockPtr CommandIfElse::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();

	codeBlock->setSource(compile(driver));

	return codeBlock;
}
