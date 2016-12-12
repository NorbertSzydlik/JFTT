#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"

namespace Calculator
{

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in),
                                  yylval( nullptr ){};
   
   int yylex(Calculator::Parser::semantic_type* lval)
   {
      yylval = lval;
      return( yylex() ); 
   }
   

private:
   /* hide this one from public view */
   int yylex();
   /* yyval ptr */
   Calculator::Parser::semantic_type* yylval;
};

} /* end namespace Calculator */

