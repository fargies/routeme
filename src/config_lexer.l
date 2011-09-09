%option noyywrap nodefault yylineno reentrant bison-bridge

%option header-file="config_lexer.h"
%{

#include "config_parser.h"
#include "logger.h"

#include <string.h>
#include <ctype.h>

#define yysval yylval->sval
#define yyival yylval->ival

%}

NUMBER    [0-9]+
IP        {NUMBER}(\.{NUMBER}){3}
WORD      [^[:space:]]+
LOCALHOST (127\.0\.0\.1|localhost)

%%

#.*$        { yysval = strdup(yytext); return COMMENT; }
{NUMBER}    { yyival = atoi(yytext); return NUMBER; }
{LOCALHOST} { return LOCALHOST; }
{IP}        { yysval = strdup(yytext); return IP_ADDRESS; }
\".*\"      { yysval = strndup(yytext + 1, strlen(yytext) - 2); return TEXT; }
-|\*        { return ANY_SYMBOL; }
TCP         { return PROTO_TCP; }
UDP         { return PROTO_UDP; }
\n          { return EOL; }

[ \t]+      { }

{WORD}      { return LEX_ERROR; }

%%

