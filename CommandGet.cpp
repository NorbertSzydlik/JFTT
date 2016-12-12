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
	compiled << "READ 1\n";
	compiled << "#TODO STORE to identifier: " << m_identifier << "\n";
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
