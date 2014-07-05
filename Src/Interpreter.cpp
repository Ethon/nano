// Copyright (c) <2014> <Florian Erler>
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

// C++ Standard Library:
#include <vector>
#include <cassert>

// Nano:
#include <Nano/Interpreter.hpp>
#include <Nano/Ast/All.hpp>

namespace
{
    class AstInterpreter : public nano::ast::Visitor
    {
    private:
        nano::GlobalContext& _ctx;
        std::vector<nano::object::Object> _stack;
        
    public:
        AstInterpreter(nano::GlobalContext& ctx)
            : _ctx(ctx)
        { }
        
        nano::object::Object& result()
        {
            assert(_stack.size() == 1);
            return _stack.back();
        }
        
        // Value AST:
        virtual void accept(nano::ast::IntNode* n) override
        {
            _stack.push_back(_ctx.getIntClass()->new_(n->value()));
        }
        
        virtual void accept(nano::ast::FloatNode* n) override
        {
            _stack.push_back(_ctx.getFloatClass()->new_(n->value()));
        }
        
        virtual void accept(nano::ast::VarNode* n) override
        {
            _stack.push_back(_ctx.globalObjects().get(n->value()));
        }
                
        // Binary Arithmetic AST:
        virtual void accept(nano::ast::BinaryPlusNode* n) override
        {
            n->lhs()->visit(this);
            n->rhs()->visit(this);
            assert(_stack.size() >= 2);
            _stack[_stack.size() - 2] = _stack[_stack.size() - 2] + _stack[_stack.size() - 1];
            _stack.pop_back();
        }
        
        virtual void accept(nano::ast::BinaryMinusNode* n) override
        {
            n->lhs()->visit(this);
            n->rhs()->visit(this);
            assert(_stack.size() >= 2);
            _stack[_stack.size() - 2] = _stack[_stack.size() - 2] - _stack[_stack.size() - 1];
            _stack.pop_back();
        }
        
        virtual void accept(nano::ast::BinaryMultiplyNode* n) override
        {
            n->lhs()->visit(this);
            n->rhs()->visit(this);
            assert(_stack.size() >= 2);
            _stack[_stack.size() - 2] = _stack[_stack.size() - 2] * _stack[_stack.size() - 1];
            _stack.pop_back();
        }
        
        virtual void accept(nano::ast::BinaryDivideNode* n) override
        {
            n->lhs()->visit(this);
            n->rhs()->visit(this);
            assert(_stack.size() >= 2);
            _stack[_stack.size() - 2] = _stack[_stack.size() - 2] / _stack[_stack.size() - 1];
            _stack.pop_back();
        }
        
        virtual void accept(nano::ast::BinaryPowerNode* n) override
        {
            n->lhs()->visit(this);
            n->rhs()->visit(this);
            assert(_stack.size() >= 2);
            _stack[_stack.size() - 2] = nano::object::pow(_stack[_stack.size() - 2], _stack[_stack.size() - 1]);
            _stack.pop_back();
        }
                
        // Other Expression AST:
        virtual void accept(nano::ast::CallNode* n) override
        {
            std::size_t resultIndex = _stack.size();
            n->target()->visit(this);
            std::size_t firstArgIndex = _stack.size();
            n->args()->visit(this);
            std::size_t argCount = _stack.size() - firstArgIndex;
            _stack[resultIndex] = _stack[resultIndex](_stack.data() + firstArgIndex, argCount);
            _stack.erase(_stack.begin() + firstArgIndex, _stack.end());
        }
        
        // Other:
        virtual void accept(nano::ast::ExpressionListNode* n) override
        {
            for(nano::ast::Node::PtrT& cur : n->list())
                cur->visit(this);
        }
    };
}

nano::Interpreter::Interpreter(GlobalContext& ctx)
    : _ctx(ctx)
{ }
        
nano::object::Object nano::Interpreter::evaluateExpression(ast::Node* expressionAst)
{
    AstInterpreter interpreter((_ctx));
    expressionAst->visit(&interpreter);
    return std::move(interpreter.result());
}
