#pragma once
#include "Command.hpp"
#include "Identifier.hpp"
#include "Number.hpp"

class CommandFor : public Command
{
public:
	CommandFor(
		Identifier::IdentifierName identifierName,
		IdentifierPtr fromIdentifier,
		IdentifierPtr toIdentifier,
		CommandPtrs const & commands,
		bool downFor = false);
	CommandFor(
		Identifier::IdentifierName identifierName, 
		IdentifierPtr fromIdentifier,
		Number toNumber, 
		CommandPtrs const & commands,
		bool downFor = false);
	CommandFor(
		Identifier::IdentifierName identifierName,
		Number fromNumber,
		IdentifierPtr toIdentifier,
		CommandPtrs const & commands,
		bool downFor = false);
	CommandFor(
		Identifier::IdentifierName identifierName,
		Number fromNumber,
		Number toNumber,
		CommandPtrs const & commands,
		bool downFor = false);

	~CommandFor();
	std::string compile(Calculator::Driver & driver) override;
	std::string getCommandName() override;
	CodeBlockPtr getCodeBlock(Calculator::Driver & driver) override;
protected:
	std::ostringstream& checkConditions(std::ostringstream& compiled, Identifier& identifier, Calculator::Driver& driver, std::string endLabel);
private:
	enum class Operands {
		IDENTIFIER_IDENTIFIER,
		IDENTIFIER_NUMBER,
		NUMBER_IDENTIFIER,
		NUMBER_NUMBER
	};

	Operands m_operands;
	Identifier::IdentifierName m_identifierName;
	CommandPtrs m_commands;

	IdentifierPtr m_fromIdentifier;
	IdentifierPtr m_toIdentifier;

	Number m_fromNumber;
	Number m_toNumber;

	bool m_isDownFor;
};
