#include "CommandDummy.hpp"

CommandDummy::CommandDummy(const std::string dummy) : m_dummy(dummy)
{
}

CommandDummy::~CommandDummy()
{
}

std::string CommandDummy::compile(Calculator::Driver & driver) {
	return m_dummy + "\n";
}

std::string CommandDummy::getCommandName()
{
	return "Command Dummy";
}

CodeBlockPtr CommandDummy::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();

	codeBlock->setSource(compile(driver));

	return codeBlock;
}