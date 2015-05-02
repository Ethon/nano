// Copyright (c) <2015> <Florian Erler>
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

// Boost:
#include <boost/format.hpp>

// Nano:
#include <Nano/Type/TypePopulation.hpp>
#include <Nano/Ast/All.hpp>
#include <Nano/Type/IntType.hpp>
#include <Nano/Type/RealType.hpp>

using namespace nano;
using namespace nano::ast;
using namespace nano::type;

class TypePopulator : public Visitor {
private:
   MessageListener& listener;

   enum class ErrorState {
      NoError,
      SoftError,
      HardError
   } state;

   void visitChildren(BinaryExpressionNode* n) {
      n->lhs()->visit(this);

      // We want to visit the other child even in case of a hard error
      // so we notice more errors.
      if(state == ErrorState::HardError) {
         state = ErrorState::NoError;
         n->rhs()->visit(this);
         state = ErrorState::HardError;
      } else {
         n->rhs()->visit(this);
      }
   }

   void makeResultType(BinaryExpressionNode* n, char const* op,
         Type::PtrT (Type::* fn)(Type::PtrT const&)) {
      visitChildren(n);
      if(state != ErrorState::HardError) {
         Type::PtrT const& lhs = n->lhs()->type();
         Type::PtrT const& rhs = n->rhs()->type();
         n->type((lhs.get()->*fn)(rhs));
         if(!n->type()) {
            state = ErrorState::HardError;
            listener.onBinaryOperationError(n->pos(), lhs, rhs,
               boost::str(boost::format(
                  "Can't apply operator %1% to values of types '%2%' and '%3%'")
                  % op % lhs->typeString() % rhs->typeString()));
         }
      }
   }
public:
   explicit TypePopulator(MessageListener& listener)
      : listener(listener)
   { }

   // Value AST:
   virtual void accept(IntNode* n) override {
      n->type(IntType::instance);
   }

   virtual void accept(FloatNode* n) override {
      n->type(RealType::instance);
   }

   virtual void accept(VarNode*) override {
      assert(false && "Not yet supported");
   }

   // Binary Arithmetic AST:
   virtual void accept(BinaryPlusNode* n) override {
      makeResultType(n, "+", &Type::resultTypeOfAddition);
   }

   virtual void accept(BinaryMinusNode* n) override {
      makeResultType(n, "-", &Type::resultTypeOfSubtraction);
   }

   virtual void accept(BinaryMultiplyNode* n) override {
      makeResultType(n, "*", &Type::resultTypeOfMultiplication);
   }

   virtual void accept(BinaryDivideNode* n) override {
      makeResultType(n, "/", &Type::resultTypeOfDivision);
   }

   virtual void accept(BinaryPowerNode* n) override {
      makeResultType(n, "**", &Type::resultTypeOfPotentiation);
   }

   // Assignment AST:
   virtual void accept(AssignmentNode*) override {
      assert(false && "Not yet supported");
   }

    // Other Expression AST:
    virtual void accept(CallNode*) override {
      assert(false && "Not yet supported");
    }

   // Other:
   virtual void accept(ExpressionListNode* n) override {
      for(Node::PtrT& cur : n->list()) {
         // Visit all children even in the case of hard errors to detect
         // as many errors as possible.
         if(state == ErrorState::HardError) {
            state = ErrorState::NoError;
            cur->visit(this);
            state = ErrorState::HardError;
         } else {
            cur->visit(this);
         }
      }
   }
};

void nano::type::populateTypes(ast::Node* ast, MessageListener& listener) {
   TypePopulator populator(listener);
   ast->visit(&populator);
}