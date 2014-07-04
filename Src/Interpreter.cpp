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
