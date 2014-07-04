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

#ifndef HEADER_UUID_C309FCA11F2F459CAFC383DF3A4251D3
#define HEADER_UUID_C309FCA11F2F459CAFC383DF3A4251D3

namespace nano
{
    namespace ast
    {
        // Value AST:
        class IntNode;
        class FloatNode;
        class VarNode;
        
        // Binary Arithmetic AST:
        class BinaryPlusNode;
        class BinaryMinusNode;
        class BinaryMultiplyNode;
        class BinaryDivideNode;
        class BinaryPowerNode;
        
        // Other Expression AST:
        class CallNode;
        
        // Other:
        class ExpressionListNode;
        
        struct Visitor
        {
            // Value AST:
            virtual void accept(IntNode*) = 0;
            virtual void accept(FloatNode*) = 0;
            virtual void accept(VarNode*) = 0;
            
            // Binary Arithmetic AST:
            virtual void accept(BinaryPlusNode*) = 0;
            virtual void accept(BinaryMinusNode*) = 0;
            virtual void accept(BinaryMultiplyNode*) = 0;
            virtual void accept(BinaryDivideNode*) = 0;
            virtual void accept(BinaryPowerNode*) = 0;
            
            // Other Expression AST:
            virtual void accept(CallNode*) = 0;
            
            // Other:
            virtual void accept(ExpressionListNode*) = 0;
        };
    }
}

#endif // HEADER_UUID_C309FCA11F2F459CAFC383DF3A4251D3