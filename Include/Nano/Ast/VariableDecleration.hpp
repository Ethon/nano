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

// C++ Standard Library:
#include <string>
#include <vector>
#include <cassert>

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>

namespace nano {
   namespace ast {
      class TypeNode : public Node {
      private:
         std::string _typeString;

      public:
         inline TypeNode(int line, int col, std::string typeString)
            : Node(line, col),  _typeString(std::move(typeString))
         { }

         inline std::string const& typeString() const {
            return _typeString;
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };

      class VariableDeclerationNode : public Node {
      private:
         PtrT _type;
         std::string _varName;

      public:
         inline VariableDeclerationNode(int line, int col, 
                         PtrT type, std::string varName)
            : Node(line, col),  _type(std::move(type)),
               _varName(std::move(varName))
         { }

         inline TypeNode* type() const {
            assert(dynamic_cast<TypeNode*>(_type.get()));
            return static_cast<TypeNode*>(_type.get());
         }

         inline std::string const& varName() const {
            return _varName;
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };

      class VariableDeclerationListNode : public Node {
      private:
         std::vector<Node::PtrT> _list;

      public:
         inline VariableDeclerationListNode(int line, int col)
            : Node(line, col)
         { }

         inline std::vector<Node::PtrT>& list() {
            return _list;
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };
   }
}