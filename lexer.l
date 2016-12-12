%{
/* C++ string header, for string ops below */
#include <string>
/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"

/* typedef to make the returns for the tokens shorter */
using token=Calculator::Parser::token;

/* define to keep from re-typing the same code over and over */
#define STOKEN( x ) ( std::make_unique<string> ( x ) )

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

%}

%option debug 
%option nodefault 
%option yyclass="Calculator::Scanner" 
%option noyywrap 
%option c++

%x comment Error

%%
<INITIAL>[\[] BEGIN(comment);
<comment>[\]] BEGIN(INITIAL);
<comment>.

<INITIAL>"ENDWHILE" { return token::ENDWHILE; }
<INITIAL>"DECLARE" { return token::DECLARE; }
<INITIAL>"ENDFOR" { return token::ENDFOR; }
<INITIAL>"WHILE" { return token::WHILE; }
<INITIAL>"ENDIF" { return token::ENDIF; }
<INITIAL>"DOWN" { return token::DOWN; }
<INITIAL>"FROM" { return token::FROM; }
<INITIAL>"THEN" { return token::THEN; }
<INITIAL>"ELSE" { return token::ELSE; }
<INITIAL>"GET" { return token::GET; }
<INITIAL>"PUT" { return token::PUT; }
<INITIAL>"FOR" { return token::FOR; }
<INITIAL>"END" { return token::END; }
<INITIAL>"IN" { return token::IN; }
<INITIAL>"DO" { return token::DO; }
<INITIAL>"TO" { return token::TO; }
<INITIAL>"IF" { return token::IF; }


<INITIAL>[_a-zA-Z]+ { yylval->build<std::string>() = yytext; return token::PIDENTIFIER; }
<INITIAL>[0-9]+ { yylval->build<Number>() = std::stoul(yytext); return token::NUMBER; }
			
<INITIAL>":=" { return token::OP_ASSIGN; }
<INITIAL>"+"  { return token::OP_ADD; }
<INITIAL>"-"  { return token::OP_MINUS; }
<INITIAL>"*"  { return token::OP_MULTIPLY; }
<INITIAL>"/"  { return token::OP_DIVIDE; }
<INITIAL>"%"  { return token::OP_MOD; }
<INITIAL>"("  { return token::LP; }
<INITIAL>")"  { return token::RP; }


<INITIAL>"<="  { return token::OP_LET; }
<INITIAL>">="  { return token::OP_GET; }
<INITIAL>"!="  { return token::OP_NEQ; }
<INITIAL>"<"   { return token::OP_LT; }
<INITIAL>">"   { return token::OP_GT; }
<INITIAL>"="   { return token::OP_EQ; }


<INITIAL>[ \t\n]+ { }

<INITIAL>";" { return token::SEMICOLON; }

<<EOF>>  { return token::EOF_TOKEN; }
%%


