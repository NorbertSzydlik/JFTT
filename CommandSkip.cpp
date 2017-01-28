#include "CommandSkip.hpp"
#include "driver.hpp"

CommandSkip::CommandSkip()
{
}


CommandSkip::~CommandSkip()
{
}

std::string CommandSkip::compile(Calculator::Driver & driver) {
	return getCodeBlock(driver)->getRaw();
}

std::string CommandSkip::getCommandName()
{
	return "Command Skip";
}

CodeBlockPtr CommandSkip::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();
	codeBlock->setSource("#skip\n");
	return codeBlock;
}
