#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "scanner.hpp"
#include "parser.tab.hh"
#include "Number.hpp"


namespace Calculator
{

class Driver{
public:
   Driver() : parser( nullptr ),
	   scanner( nullptr )
   {
     std::cout << __FUNCTION__ << std::endl;
     m_lastLabelNumbers.clear();
     m_declaredVariables.clear();
   };

   virtual ~Driver();

   void parse( std::string input );

   double getResult() const;
   void setResult(double result);

   void addError(std::string const & error);
   std::string getError() const;
   bool isError() const;

   CodeBlockPtr compileToBlock();

   std::ostream& compile(std::ostream &stream);

   void setCommands(CommandPtrs const & commands);

   std::string getNextLabelFor(std::string labelName = "");
   void declare(std::string identifierName);
   void declare(std::string identifierName, Number size);
   void undeclare(std::string identifierName);

   Number getDeclarationPosition(std::string identifierName);
   Number getTmpMemoryPosition();
protected:
	bool doesDeclarationExist(std::string identifierName);
private:
   std::stringstream m_errorStream;

   CommandPtrs m_commands;

   Calculator::Parser *parser;
   Calculator::Scanner *scanner;
   std::unordered_map<std::string, unsigned long> m_lastLabelNumbers;
   Number m_lastMemoryAddress;
   std::unordered_map<std::string, Number> m_declaredVariables;
};

} /* end namespace Calculator */
