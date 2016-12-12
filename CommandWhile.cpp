#include "CommandWhile.hpp"
#include "driver.hpp"

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

	compiled << "#while block\n";
	compiled << conditionsLabel << ": #conditions label\n";

	compiled << m_condition->evaluate(driver, 0);

	compiled << "JZERO %r0 @" << endLabel << " #jump to end label when condition is false\n";
	for (auto& cmd : m_commands) {
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
