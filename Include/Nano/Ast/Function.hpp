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

#pragma once

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>
#include <Nano/Ast/VariableDecleration.hpp>
#include <Nano/Ast/CodeBlock.hpp>

namespace nano {
   namespace ast {
      class FunctionDefinitionNode : public Node {
      private:
         std::string _name;
         PtrT _arglist;
         PtrT _code;

      public:
         inline FunctionDefinitionNode(int line, int col,
            std::string name, PtrT arglist, PtrT code)
            :  Node(line, col), _name(std::move(name)),
               _arglist(std::move(arglist)), _code(std::move(code))
         { }

         std::string const& name() {
            return _name;
         }

         VariableDeclerationListNode* arglist() {
            assert(dynamic_cast<VariableDeclerationListNode*>(_arglist.get()));
            return static_cast<VariableDeclerationListNode*>(_arglist.get());
         }

         CodeBlockNode* code() {
            assert(dynamic_cast<VariableDeclerationListNode*>(_arglist.get()));
            return static_cast<CodeBlockNode*>(_code.get());
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };
   }
}
