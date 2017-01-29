%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {Calculator}
%define parser_class_name {Parser}
%define api.value.type variant

%code requires{
   namespace Calculator {
      class Driver;
      class Scanner;
   }

#include <memory>
#include <vector>

#include "Command.hpp"
#include "Number.hpp"
#include "Identifier.hpp"
#include "Expression.hpp"
#include "Condition.hpp"

}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%code{
   #include <memory>
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <string>
   #include <cmath>
   #include <vector>

   /* include for all driver functions */
   #include "driver.hpp"

   #include "CommandPut.hpp"
   #include "CommandGet.hpp"
   #include "CommandWhile.hpp"
   #include "CommandFor.hpp"
   #include "CommandIfElse.hpp"
   #include "CommandAssign.hpp"
   #include "CommandSkip.hpp"

   #include "CommandDummy.hpp"

   #include "ExpressionIdentifier.hpp"
   #include "ExpressionNumber.hpp"
   #include "ExpressionOperation.hpp"

   #include "logging.h"

#undef yylex
#define yylex scanner.yylex
}


%token DECLARE IN END IF THEN ELSE ENDIF WHILE DO ENDWHILE FOR DOWNTO FROM TO ENDFOR GET PUT SKIP
%token EOF_TOKEN

%token OP_ASSIGN
%token OP_ADD OP_MINUS LP RP
%token OP_MULTIPLY OP_DIVIDE OP_MOD
%token OP_POWER
%token OP_EQ
%token OP_NEQ
%token OP_LT OP_GT OP_LET OP_GET

%left OP_ADD OP_MINUS
%left OP_MULTIPLY OP_DIVIDE
%right OP_POWER
%left NEG

%token <std::string> PIDENTIFIER
%token <Number> NUMBER
%token        SEMICOLON

/* destructor rule for <sval> objects */
//%destructor { if ($$)  { delete ($$); ($$) = nullptr; } } <sval>

%type <CommandPtr> command
%type <CommandPtrs> commands
%type <IdentifierPtr> identifier
%type <ExpressionPtr> expression
%type <ConditionPtr> condition

%%
program: DECLARE vdeclarations IN commands END { LOG("parser: declare"); driver.setCommands($4); }

vdeclarations: vdeclarations PIDENTIFIER { LOG("parser: declare: " << $2); driver.declare($2);}
  | vdeclarations PIDENTIFIER LP NUMBER RP { LOG("parser: declare: " << $2 << "[" << $4 << "]");  driver.declare($2, $4);}
	|

commands: commands command { LOG("parser: commands command"); $1.push_back($2); $$ = $1; }
  |  command { LOG("parser: command"); $$.push_back($1); }

command: identifier OP_ASSIGN expression SEMICOLON { $$ = std::make_shared<CommandAssign>($1, $3); }
  | identifier OP_EQ expression SEMICOLON { error("Did you mean ':='?"); $$ = std::make_shared<CommandAssign>($1, $3); }
  | IF condition THEN commands ENDIF { LOG("parser: IF"); $$ = std::make_shared<CommandIfElse>($2, $4); }
	| IF condition THEN commands ELSE commands ENDIF { LOG("parser: IF-ELSE"); $$ = std::make_shared<CommandIfElse>($2, $4, $6); }

	| WHILE condition DO commands ENDWHILE { LOG("parser: while"); $$ = std::make_shared<CommandWhile>($2, $4); }

	| FOR PIDENTIFIER FROM NUMBER TO NUMBER DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8);}
	| FOR PIDENTIFIER FROM identifier TO NUMBER DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8); }
	| FOR PIDENTIFIER FROM NUMBER TO identifier DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8); }
  | FOR PIDENTIFIER FROM identifier TO identifier DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $6, $4, $8); }

	| FOR PIDENTIFIER FROM NUMBER DOWNTO NUMBER DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8, true); }
	| FOR PIDENTIFIER FROM identifier DOWNTO NUMBER DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8, true); }
	| FOR PIDENTIFIER FROM NUMBER DOWNTO identifier DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8, true); }
  | FOR PIDENTIFIER FROM identifier DOWNTO identifier DO commands ENDFOR { $$ = std::make_shared<CommandFor>($2, $4, $6, $8, true); }

	| GET identifier SEMICOLON { LOG("parser: GET"); $$ = std::make_shared<CommandGet>($2); }
	| PUT identifier SEMICOLON { LOG("parser: PUT"); $$ = std::make_shared<CommandPut>($2); }
	| PUT NUMBER SEMICOLON { LOG("parser: PUT-const"); $$ = std::make_shared<CommandPut>($2); }
  | SKIP SEMICOLON { $$ = std::make_shared<CommandSkip>(); }

expression: NUMBER { LOG("parser: NYMBER");  $$ = std::make_shared<ExpressionNumber>($1);}
  | identifier { LOG("parser: identifier");  $$ = std::make_shared<ExpressionIdentifier>($1); }

	| identifier OP_ADD identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_ADD, $1, $3);}
	| identifier OP_ADD NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_ADD, $1, $3);}
	| NUMBER OP_ADD identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_ADD, $1, $3);}
	| NUMBER OP_ADD NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_ADD, $1, $3);}

	| identifier OP_MINUS identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_SUB, $1, $3);}
	| identifier OP_MINUS NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_SUB, $1, $3);}
	| NUMBER OP_MINUS identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_SUB, $1, $3);}
	| NUMBER OP_MINUS NUMBER  { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_SUB, $1, $3);}

	| identifier OP_MULTIPLY identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MUL, $1, $3);}
	| identifier OP_MULTIPLY NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MUL, $1, $3);}
	| NUMBER OP_MULTIPLY identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MUL, $1, $3);}
	| NUMBER OP_MULTIPLY NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MUL, $1, $3);}

	| identifier OP_DIVIDE identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_DIV, $1, $3);}
	| identifier OP_DIVIDE NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_DIV, $1, $3);}
	| NUMBER OP_DIVIDE identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_DIV, $1, $3);}
	| NUMBER OP_DIVIDE NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_DIV, $1, $3);}

	| identifier OP_MOD identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MOD, $1, $3);}
	| identifier OP_MOD NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MOD, $1, $3);}
	| NUMBER OP_MOD identifier { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MOD, $1, $3);}
	| NUMBER OP_MOD NUMBER { $$ = std::make_shared<ExpressionOperation>(ExpressionOperation::Type::OP_MOD, $1, $3);}

condition: identifier OP_EQ identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_EQ, $1, $3); }
  | identifier OP_EQ NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_EQ, $1, $3); }
	| NUMBER OP_EQ identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_EQ, $1, $3); }
	| NUMBER OP_EQ NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_EQ, $1, $3); }

	| identifier OP_NEQ identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_NEQ, $1, $3); }
	| identifier OP_NEQ NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_NEQ, $1, $3); }
  | NUMBER OP_NEQ identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_NEQ, $1, $3); }
  | NUMBER OP_NEQ NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_NEQ, $1, $3); }

	| identifier OP_LT identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $3, $1); }
	| identifier OP_LT NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $3, $1); }
  | NUMBER OP_LT identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $3, $1); }
  | NUMBER OP_LT NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $3, $1); }

  | identifier OP_GT identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $1, $3); }
	| identifier OP_GT NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $1, $3); }
  | NUMBER OP_GT identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $1, $3); }
  | NUMBER OP_GT NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GT, $1, $3); }

  | identifier OP_LET identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $3, $1); }
	| identifier OP_LET NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $3, $1); }
  | NUMBER OP_LET identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $3, $1); }
  | NUMBER OP_LET NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $3, $1); }

  | identifier OP_GET identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $1, $3); }
	| identifier OP_GET NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $1, $3); }
  | NUMBER OP_GET identifier { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $1, $3); }
  | NUMBER OP_GET NUMBER { $$ = std::make_shared<Condition>(Condition::Type::OP_GE, $1, $3); }

  identifier: PIDENTIFIER { $$ = std::make_shared<Identifier>($1); }
  |  PIDENTIFIER LP NUMBER RP { $$ = std::make_shared<Identifier>($1, $3); }
	|  PIDENTIFIER LP PIDENTIFIER RP { $$ = std::make_shared<Identifier>($1, $3); }
%%

void Calculator::Parser::error( const std::string &err_message )
{
    driver.addError(err_message);
}
