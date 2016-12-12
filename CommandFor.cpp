#include "CommandFor.hpp"
#include "driver.hpp"

CommandFor::CommandFor(Identifier::IdentifierName identifierName,
	IdentifierPtr fromIdentifier,
	IdentifierPtr toIdentifier,
	CommandPtrs const & commands,
	bool downFor)
{
	m_operands = Operands::IDENTIFIER_IDENTIFIER;
	m_identifierName = identifierName;
	m_fromIdentifier = fromIdentifier;
	m_toIdentifier = toIdentifier;
	m_commands = commands;
	m_isDownFor = downFor;
}

CommandFor::CommandFor(Identifier::IdentifierName identifierName,
	IdentifierPtr fromIdentifier,
	Number toNumber,
	CommandPtrs const & commands,
	bool downFor)
{
	m_operands = Operands::IDENTIFIER_NUMBER;
	m_identifierName = identifierName;
	m_fromIdentifier = fromIdentifier;
	m_toNumber = toNumber;
	m_commands = commands;
	m_isDownFor = downFor;
}

CommandFor::CommandFor(Identifier::IdentifierName identifierName,
	Number fromNumber,
	IdentifierPtr toIdentifier,
	CommandPtrs const & commands,
	bool downFor)
{
	m_operands = Operands::NUMBER_IDENTIFIER;
	m_identifierName = identifierName;
	m_fromNumber = fromNumber;
	m_toIdentifier = toIdentifier;
	m_commands = commands;
	m_isDownFor = downFor;
}

CommandFor::CommandFor(Identifier::IdentifierName identifierName,
	Number fromNumber,
	Number toNumber,
	CommandPtrs const & commands,
	bool downFor)
{
	m_operands = Operands::NUMBER_NUMBER;
	m_identifierName = identifierName;
	m_fromNumber = fromNumber;
	m_toNumber = toNumber;
	m_commands = commands;
	m_isDownFor = downFor;
}

CommandFor::~CommandFor()
{
}

std::ostringstream& CommandFor::checkConditions(std::ostringstream& compiled, Identifier& identifier, Calculator::Driver& driver, std::string endLabel) {
	compiled << identifier.loadToRegister(driver, 0);

	if (m_operands == Operands::IDENTIFIER_IDENTIFIER || m_operands == Operands::NUMBER_IDENTIFIER) {
		compiled << m_toIdentifier->loadToRegister(driver, 1);
	}
	else {
		compiled << "LOAD %r1 " << m_toNumber << "\n";
	}
	compiled << "COPY %r2 %r0 #copy value of counter to other register\n";
	if (m_isDownFor) {
		compiled << "GE %r0 %r1 #counter >= toNumber\n";

		compiled << "DEC %r2\n";
	}
	else {
		
		compiled << "GE %r1 %r0 #counter <= toNumber\n";

		compiled << "INC %r2\n";
	}
	
	compiled << identifier.loadPositionToRegister(driver, 1);
	compiled << "STORE %r2 %r1 #save for counter in memory for later\n";

	if (m_isDownFor) {
		compiled << "JZERO %r0 @";
	}
	else {
		compiled << "JZERO %r1 @";
	}
	compiled << endLabel << " #jump to end label if conditions are false\n";
	return compiled;
}

std::string CommandFor::compile(Calculator::Driver & driver) {
	std::ostringstream compiled;

	driver.declare(m_identifierName);
	Identifier identifier = Identifier(m_identifierName);

	std::string conditionCheckLabel = driver.getNextLabelFor("for_conditions_check");
	std::string endLabel = driver.getNextLabelFor("for_end");

	compiled << "#begin of for block\n";

	compiled << "#reset identifier " << m_identifierName << "\n";
	if (m_operands == Operands::IDENTIFIER_IDENTIFIER || m_operands == Operands::IDENTIFIER_NUMBER) {
		compiled << "#load counter " << m_identifierName << " to register\n";
		compiled << m_fromIdentifier->loadToRegister(driver, 0);
	}
	else {
		compiled << "LOAD %r0 " << m_fromNumber << "\n";
	}
	compiled << identifier.loadPositionToRegister(driver, 1);
	compiled << "STORE %r0 %r1 #save counter to memory\n";

	
	compiled << conditionCheckLabel << ": #conditions check label\n";

	checkConditions(compiled, identifier, driver, endLabel);

	for (auto cmd : m_commands) {
		std::cout << "test: for: before: " << cmd->getCommandName() << std::endl;
		compiled << cmd->compile(driver);
		std::cout << "test: for: after: " << cmd->getCommandName() << std::endl;
	}
	compiled << "JUMP @" << conditionCheckLabel << "\n";
	compiled << endLabel << ": #for end label\n";
	compiled << "#end of for block\n";

	driver.undeclare(m_identifierName);
	return compiled.str();
}

std::string CommandFor::getCommandName()
{
	return "Command For";
}

CodeBlockPtr CommandFor::getCodeBlock(Calculator::Driver & driver)
{
	CodeBlockPtr codeBlock = std::make_shared<CodeBlock>();

	codeBlock->setSource(compile(driver));

	return codeBlock;
}