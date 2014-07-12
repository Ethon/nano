%{
// C++ Standard Library:
#include <cassert>

// Nano:
#include <Nano/Detail/ParseContext.hpp>
#include <Nano/Detail/Token.h>
#include <Nano/Ast/All.hpp>

int yylex(YYSTYPE* yylval, YYLTYPE* loc, void* yyscanner);
void yyerror(YYLTYPE* loc, void* ctx, char const* s);

#define YYLEX_PARAM context->lexer()
%}

%pure-parser
%locations
%lex-param {void* lexer}
%parse-param {nano::detail::ParseContext* context}

%union
{
    nano::object::CppIntType const* ival;
    double fval;
    std::string const* sval;
}

%start	input 

// Keywords:
%token  FUNCTION
%token  RETURN
%token  FOR
%token  IF

// Special
%token OPAREN
%token CPAREN
%token OBRACE
%token CBRACE

%token COMMA
%token SEMICOLON
%token COLON

%token EQUAL
%token ASSIGN
%token PLUSASSIGN
%token MAPSTO
%token PLUS
%token MINUS
%token MULTIPLY
%token DIVIDE
%token MODULO
%token POWER

// Terminal Tokens:
%token  <ival>  INT_LIT
%token  <fval>  FP_LIT
%token  <sval>  IDENT

%%

input
    : /* empty */
    | ExpressionStatement
	;
    
PrimaryExpression
    : INT_LIT { context->push<nano::ast::IntNode>(@1.first_line, @1.first_column, *$1); }
    | FP_LIT { context->push<nano::ast::FloatNode>(@1.first_line, @1.first_column, $1); }
    | IDENT { context->push<nano::ast::VarNode>(@1.first_line, @1.first_column, *$1); }
    | OPAREN Expression CPAREN
    ;
    
PostfixExpression
    : PrimaryExpression
    | PrimaryExpression OPAREN ArgumentExpressionList CPAREN /* Function call */
        {
            auto args = context->pop();
            assert(dynamic_cast<nano::ast::ExpressionListNode*>(args.get()));
            auto target = context->pop();
            context->push<nano::ast::CallNode>(
                @1.first_line, @1.first_column, std::move(target), std::move(args));
        }
    ;
    
ArgumentExpressionList
    : /* empty */
        {
            context->push<nano::ast::ExpressionListNode>(@$.first_line, @$.first_column);
        }
    | ArgumentExpressionList COMMA Expression
        {
            auto expr = context->pop();
            auto eln = dynamic_cast<nano::ast::ExpressionListNode*>(context->top());
            assert(eln);
            eln->list().push_back(std::move(expr));
        }
    | Expression
        {
            auto expr = context->pop();
            context->push<nano::ast::ExpressionListNode>(@1.first_line, @1.first_column);
            auto eln = static_cast<nano::ast::ExpressionListNode*>(context->top());
            eln->list().push_back(std::move(expr));
        }
    ;
    
PrefixExpression
    : PostfixExpression
    ;
    
ExponentiationExpression
    : PrefixExpression
    | PrefixExpression POWER PrefixExpression
        {
            auto r = context->pop(), l = context->pop();
            context->push<nano::ast::BinaryPowerNode>(
                @1.first_line, @1.first_column, std::move(l), std::move(r));
        }
    ;
    
MultiplicativeExpression
    : ExponentiationExpression
    | MultiplicativeExpression MULTIPLY ExponentiationExpression
        {
            auto r = context->pop(), l = context->pop();
            context->push<nano::ast::BinaryMultiplyNode>(
                @1.first_line, @1.first_column, std::move(l), std::move(r));
        }
    | MultiplicativeExpression DIVIDE ExponentiationExpression
        {
            auto r = context->pop(), l = context->pop();
            context->push<nano::ast::BinaryDivideNode>(
                @1.first_line, @1.first_column, std::move(l), std::move(r));
        }
    ;
    
AdditiveExpression
    : MultiplicativeExpression
    | AdditiveExpression PLUS MultiplicativeExpression
        {
            auto r = context->pop(), l = context->pop();
            context->push<nano::ast::BinaryPlusNode>(
                @1.first_line, @1.first_column, std::move(l), std::move(r));
        }
    | AdditiveExpression MINUS MultiplicativeExpression
        {
            auto r = context->pop(), l = context->pop();
            context->push<nano::ast::BinaryMinusNode>(
                @1.first_line, @1.first_column, std::move(l), std::move(r));
        }
    ;
   
AssignmentExpression
    : AdditiveExpression
    | AdditiveExpression ASSIGN AssignmentExpression
        {
            auto r = context->pop(), l = context->pop();
            context->push<nano::ast::AssignmentNode>(
                @1.first_line, @1.first_column, std::move(l), std::move(r));
        }
    ;
    
Expression
    : AssignmentExpression
    ;
    
ExpressionStatement
    : Expression
    | Expression SEMICOLON
    ;

%%

void yyerror(YYLTYPE* loc, void* ctx, char const* s)
{
    static_cast<nano::detail::ParseContext*>(ctx)->triggerParserError(
        nano::SourcePos{loc->first_line, loc->first_column}, s);
    
}

int nano::detail::ParseContext::parseImpl(nano::detail::ParseContext* ctx)
{
    return yyparse(ctx);
}
