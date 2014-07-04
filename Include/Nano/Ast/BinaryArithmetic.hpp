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

#ifndef HEADER_UUID_782C7C6A7D994BA18F52DA256CC1164D
#define HEADER_UUID_782C7C6A7D994BA18F52DA256CC1164D

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>

namespace nano
{
    namespace ast
    {
        struct BinaryPlusNode : public BinaryExpressionNode
        {
            inline BinaryPlusNode(int line, int col, PtrT lhs, PtrT rhs)
                : BinaryExpressionNode(line, col, std::move(lhs), std::move(rhs))
            { }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
    
        struct BinaryMinusNode : public BinaryExpressionNode
        {
            inline BinaryMinusNode(int line, int col, PtrT lhs, PtrT rhs)
                : BinaryExpressionNode(line, col, std::move(lhs), std::move(rhs))
            { }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
        
        struct BinaryMultiplyNode : public BinaryExpressionNode
        {
            inline BinaryMultiplyNode(int line, int col, PtrT lhs, PtrT rhs)
                : BinaryExpressionNode(line, col, std::move(lhs), std::move(rhs))
            { }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
   
        struct BinaryDivideNode : public BinaryExpressionNode
        {
            inline BinaryDivideNode(int line, int col, PtrT lhs, PtrT rhs)
                : BinaryExpressionNode(line, col, std::move(lhs), std::move(rhs))
            { }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
        
        struct BinaryPowerNode : public BinaryExpressionNode
        {
            inline BinaryPowerNode(int line, int col, PtrT lhs, PtrT rhs)
                : BinaryExpressionNode(line, col, std::move(lhs), std::move(rhs))
            { }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
    }
}

#endif // HEADER_UUID_782C7C6A7D994BA18F52DA256CC1164D