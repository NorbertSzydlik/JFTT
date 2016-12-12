#include "CommandPut.hpp"

CommandPut::CommandPut(IdentifierPtr const & identifier) : m_isConst(false)
{
	m_identifier = identifier;
}

CommandPut::CommandPut(Number number) : m_isConst(true)
{
	m_number = number;
}

CommandPut::~CommandPut()
{
}

std::string CommandPut::compile(Calculator::Driver & driver) {
	std::ostringstream compiled;
	std::cout << "test: put: " << m_isConst << " " << m_number << std::endl;
	if (m_isConst) {
		compiled << "LOAD %r0 " << m_number << "\n";
		compiled << "WRITE %r0\n";
	}
	else {
		compiled << m_identifier->loadToRegister(driver, 0);
		compiled << "WRITE %r0\n";
	}

	return compiled.str();
}

std::string CommandPut::getCommandName()
{
	return "Command Put";
}

CodeBlockPtr CommandPut::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();

	codeBlock->setSource(compile(driver));

	return codeBlock;
}
