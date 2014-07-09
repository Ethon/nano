%option noyywrap
%option reentrant
%option bison-bridge
%option bison-locations

%{
// C++ Standard Library:
#include <cstdlib>

// Nano:
#include <Nano/Detail/ParseContext.hpp>
#include <Nano/Detail/Token.h>

#define YY_EXTRA_TYPE nano::detail::ParseContext*

// Disable warnings created by gcc for Flex code.
#ifdef __GNUC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Wunused-function"
#endif

%}

BINDIGIT    0|1
OCTDIGIT    [0-7]
DECDIGIT    [0-9]
HEXDIGIT    [0-9a-fA-F]
ALPHA       [a-zA-Z]
ALPHANUM    [0-9a-zA-Z]

%%

function[ \t\n] { return FUNCTION; }
return[ \t\n]   { return RETURN; }
for[ \t\n]      { return FOR; }
if[ \t\n]       { return IF; }

\(      { return OPAREN; }
\)      { return CPAREN; }
\{      { return OBRACE; }
\}      { return CBRACE; }
,       { return COMMA; }
;       { return SEMICOLON; }
:       { return COLON; }

==      { return EQUAL; }
=       { return ASSIGN; }
\+=     { return PLUSASSIGN; }
->      { return MAPSTO; }
\+      { return PLUS; }
-       { return MINUS; }
\*      { return MULTIPLY; }
\/      { return DIVIDE; }
\%      { return MODULO; }
\*\*    { return POWER; }


0(b|B){BINDIGIT}+           {yylval->ival = std::strtoll(yytext + 2, nullptr, 2); return INT_LIT;}
0{OCTDIGIT}+                {yylval->ival = std::strtoll(yytext + 1, nullptr, 8); return INT_LIT;}
0(x|X){HEXDIGIT}+           {yylval->ival = std::strtoll(yytext + 2, nullptr, 16); return INT_LIT;}
{DECDIGIT}+"."{DECDIGIT}+   {yylval->fval = std::strtod(yytext, nullptr); return FP_LIT;}
{DECDIGIT}+                 {yylval->ival = std::strtoll(yytext, nullptr, 10); return INT_LIT;}
({ALPHA}|_)({ALPHANUM}|_)*  {yylval->sval = yyextra->storage().storeIdentifier(std::string(yytext, yytext + yyleng)); return IDENT;}

[ \t\n]+          /* eat up whitespace */

. yyextra->triggerLexerError(nano::SourcePos{yylloc->first_line, yylloc->first_column}, yytext);

%%

// Enable gcc warnings again.
#ifdef __GNUC__
#   pragma GCC diagnostic pop
#endif

void* nano::detail::ParseContext::createFileLexer(
        std::FILE* file,
        nano::detail::ParseContext* context)
{
    void* lexer;
    yylex_init(&lexer);
    yyset_in(file, lexer);
    yyset_extra(context, lexer);
    return lexer;
}

void* nano::detail::ParseContext::createStringLexer(
        char const* string,
        nano::detail::ParseContext* context)
{
    void* lexer;
    yylex_init(&lexer);
    yy_scan_string(string, lexer);
    yyset_extra(context, lexer);
    return lexer;
}

void nano::detail::ParseContext::destroyLexer(void* lexer)
{
    yylex_destroy(lexer);
}