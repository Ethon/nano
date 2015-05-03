// Copyright (c) <2014-2015> <Florian Erler>
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

#pragma once

namespace nano {
   namespace ast {
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

      // Assignment AST:
      class AssignmentNode;

      // Other Expression AST:
      class CallNode;

      // Statement AST:
      class CodeBlockNode;
      class FunctionDefinitionNode;

      // Other:
      class ExpressionListNode;
      class TypeNode;
      class VariableDeclerationNode;
      class VariableDeclerationListNode;

      struct Visitor {
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

         // Assignment AST:
         virtual void accept(AssignmentNode*) = 0;

         // Other Expression AST:
         virtual void accept(CallNode*) = 0;

         // Statement AST:
         virtual void accept(CodeBlockNode*) = 0;
         virtual void accept(FunctionDefinitionNode*) = 0;

         // Other:
         virtual void accept(ExpressionListNode*) = 0;
         virtual void accept(TypeNode*) = 0;
         virtual void accept(VariableDeclerationNode*) = 0;
         virtual void accept(VariableDeclerationListNode*) = 0;
      };
   }
}
