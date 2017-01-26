#include "CommandGet.hpp"

CommandGet::CommandGet(IdentifierPtr const & identifier)
{
	m_identifier = identifier;
}

CommandGet::~CommandGet()
{
}

std::string CommandGet::compile(Calculator::Driver & driver) {
	std::ostringstream compiled;

	compiled << "GET 1\n";
	compiled << m_identifier->loadPositionToRegister(driver, 0);
	compiled << "STORE 1\n";

	return compiled.str();
}

std::string CommandGet::getCommandName()
{
	return "Command Get";
}

CodeBlockPtr CommandGet::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();

	codeBlock->setSource(compile(driver));

	return codeBlock;
}
