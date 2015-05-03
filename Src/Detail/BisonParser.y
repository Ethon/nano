%{
// C++ Standard Library:
#include <cassert>

// Boost:
#include <boost/multiprecision/cpp_int.hpp> 
#include <boost/multiprecision/cpp_dec_float.hpp> 

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
%lex-param {YYLEX_PARAM}
%parse-param {nano::detail::ParseContext* context}

%union
{
    boost::multiprecision::cpp_int const* ival;
    boost::multiprecision::cpp_dec_float_50 const* fval;
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
   | TopLevelStatement
   ;

TopLevelStatement
   : FunctionDefinition
   ;

FunctionDefinition
   :  FUNCTION IDENT OPAREN ArgumentDeclerationList CPAREN CodeBlock {
         auto code = context->pop();
         auto arglist = context->pop();
         context->push<nano::ast::FunctionDefinitionNode>(@$.first_line,
            @$.first_column, *$2, std::move(arglist), std::move(code));
      }
   ;

CodeBlock
   : OBRACE CodeBlockContent CBRACE
   ;

CodeBlockContent
   :  /* empty */ {
         context->push<nano::ast::CodeBlockNode>(@$.first_line, @$.first_column);
      }
   |  CodeBlock Statement {
         auto stmnt = context->pop();
         auto cbn = dynamic_cast<nano::ast::CodeBlockNode*>(context->top());
         assert(cbn);
         cbn->list().push_back(std::move(stmnt));
      }
   |  Statement {
         auto stmnt = context->pop();
         context->push<nano::ast::CodeBlockNode>(@1.first_line, @1.first_column);
         auto cbn = static_cast<nano::ast::CodeBlockNode*>(context->top());
         cbn->list().push_back(std::move(stmnt));
      }
   ;

ArgumentDeclerationList
   : /* empty */
      {
         context->push<nano::ast::VariableDeclerationListNode>(@$.first_line, @$.first_column);
      }
   | ArgumentExpressionList COMMA VariableDecleration
      {
         auto decl = context->pop();
         auto vdln = dynamic_cast<nano::ast::VariableDeclerationListNode*>(context->top());
         assert(vdln);
         vdln->list().push_back(std::move(decl));
      }
   | VariableDecleration
      {
         auto decl = context->pop();
         context->push<nano::ast::VariableDeclerationListNode>(@1.first_line, @1.first_column);
         auto vdln = static_cast<nano::ast::VariableDeclerationListNode*>(context->top());
         vdln->list().push_back(std::move(decl));
      }
   ;

VariableDecleration
   :  Type IDENT {
         auto type = context->pop();
         assert(dynamic_cast<nano::ast::TypeNode*>(type.get()));
         context->push<nano::ast::VariableDeclerationNode>(@1.first_line, @1.first_column,
            std::move(type), *$2);
      }
   ;

Type
   : IDENT { context->push<nano::ast::TypeNode>(@1.first_line, @1.first_column, *$1); }
   ;

Statement
   : ExpressionStatement
   ;

PrimaryExpression
   : INT_LIT { context->push<nano::ast::IntNode>(@1.first_line, @1.first_column, *$1); }
   | FP_LIT { context->push<nano::ast::FloatNode>(@1.first_line, @1.first_column, *$1); }
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
   : Expression SEMICOLON
   ;

%%

void yyerror(YYLTYPE* loc, void* ctx, char const* s) {
   static_cast<nano::detail::ParseContext*>(ctx)->triggerParserError(
      nano::SourcePos{loc->first_line, loc->first_column}, s);
}

int nano::detail::ParseContext::parseImpl(nano::detail::ParseContext* ctx) {
   return yyparse(ctx);
}
