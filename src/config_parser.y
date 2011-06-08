%pure-parser

%parse-param { RmeConfigLoader *pp }

%union
{
  int ival;
  char *sval;
  void *pval;
}

%{

#include <string.h>
#include <stdio.h>

#include "config.h"
#include "config_lexer.h"
#include "rme-config-loader.h"
#include "rme-config-loader-private.h"
#include "rme-rule.h"
#include "logger.h"

#define YYLEX_PARAM pp->_priv->scaninfo

#ifdef ROUTEME_YYDEBUG

yydebug = 1;

static void print_token_value (FILE *, int, YYSTYPE);
#define YYPRINT(file, type, value) print_token_value (file, type, value)

#endif

void yyerror(RmeConfigLoader *pp, const char *s);

%}

%token <sval> COMMENT "comment"
%token <ival> NUMBER  "number"
%token <sval> IP_ADDRESS "IP address"
%token <sval> TEXT "some text"
%token ANY_SYMBOL "ANY symbol"
%token PROTO_TCP "TCP"
%token PROTO_UDP "UDP"
%token END 0 "EOF"
%token EOL "EOL"
%token LEX_ERROR "LEX_ERROR"

%start config

%%

config:
| config COMMENT
{
    free($<sval>2);
}
| config EOL
{
    rme_config_loader_line_inc(pp);
}
| config routing_rule
{
    RmeRule *rr = (RmeRule *) $<pval>2;
    rme_manager_add_rule(pp->_priv->manager, rr);
    g_object_unref(rr);
}
| config errors
{
    log_warning("[config]: Syntax error at line %i",
            rme_config_loader_get_line(pp));
    pp->_priv->synerr++;
    yyerrok;
}
;

errors:
| errors error
;

routing_rule:
  ip_address port port proto
{
    $<pval>$ = rme_rule_new($<sval>1, $<ival>2, $<ival>3,
            ($<ival>4 == PROTO_TCP) ? RME_PROTO_TCP : RME_PROTO_UDP, NULL);
    free($<sval>1);
}
| ip_address port port proto TEXT
{
    $<pval>$ = rme_rule_new($<sval>1, $<ival>2, $<ival>3,
            ($<ival>4 == PROTO_TCP) ? RME_PROTO_TCP : RME_PROTO_UDP, $<sval>5);
    free($<sval>1);
    free($<sval>5);
}
| ip_address port proto
{
    $<pval>$ = rme_rule_new($<sval>1, $<ival>2, $<ival>2,
            ($<ival>3 == PROTO_TCP) ? RME_PROTO_TCP : RME_PROTO_UDP, NULL);
    free($<sval>1);
}
| ip_address port proto TEXT
{
    $<pval>$ = rme_rule_new($<sval>1, $<ival>2, $<ival>2,
            ($<ival>3 == PROTO_TCP) ? RME_PROTO_TCP : RME_PROTO_UDP, $<sval>4);
    free($<sval>1);
    free($<sval>4);
}
;

proto:
  PROTO_TCP { $<ival>$ = PROTO_TCP; }
| PROTO_UDP { $<ival>$ = PROTO_UDP; }
;

port: NUMBER { $<ival>$ = $<ival>1; }
;

ip_address:
  IP_ADDRESS { $<sval>$ = $<sval>1; }
| ANY_SYMBOL { $<sval>$ = strdup(""); }
;

%%

void yyerror(RmeConfigLoader *pp, const char *s)
{
}

#ifdef ROUTEME_YYDEBUG
static void print_token_value (FILE *file, int type, YYSTYPE value)
{
    switch (type) {
        case COMMENT:
        case IP_ADDRESS:
            fprintf(file, "%s", value.sval);
            break;
        case NUMBER:
            fprintf(file, "%i", value.ival);
            break;
    }
}
#endif

