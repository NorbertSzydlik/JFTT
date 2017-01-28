#include "CommandWhile.hpp"
#include "driver.hpp"

const unsigned int WORK_REGISTER = 1;
const unsigned int TMP_REGISTER = 2;

CommandWhile::CommandWhile(ConditionPtr condition, CommandPtrs const & commands)
{
	m_condition = condition;
	m_commands = commands;
}

CommandWhile::~CommandWhile()
{
}

std::string CommandWhile::compile(Calculator::Driver & driver) {
	std::ostringstream compiled;

	std::string conditionsLabel = driver.getNextLabelFor("while_condition_check");
	std::string endLabel = driver.getNextLabelFor("while_end");
	std::string commandBlockLabel = driver.getNextLabelFor("while_commands");

	compiled << "#while block\n";
	compiled << conditionsLabel << ": #conditions label\n";

	compiled << m_condition->evaluate(driver, WORK_REGISTER, TMP_REGISTER);

	compiled << "JZERO %r" << WORK_REGISTER << " @" << commandBlockLabel << " #jump to command block label if condition is true (e.g register == 0)\n";
	compiled << "JUMP @" << endLabel << " #condition failed, jump to end\n";

  compiled << commandBlockLabel << ": #start of command block\n";
	for (auto& cmd : m_commands) {
		compiled << "# while-cmd: " << cmd->getCommandName() << "\n";
		compiled << cmd->compile(driver);
	}

	compiled << "JUMP @" << conditionsLabel << " #jump to check conditions\n";
	compiled << endLabel << ": #end label\n";
	compiled << "#end of while block\n";

	return compiled.str();
}

std::string CommandWhile::getCommandName()
{
	return "Command While";
}

CodeBlockPtr CommandWhile::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();

	codeBlock->setSource(compile(driver));

	return codeBlock;
}
