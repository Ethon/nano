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

// C++ Standard Library:
#include <memory>

// Nano:
#include <Nano/Common/SourcePos.hpp>
#include <Nano/Type/Type.hpp>

namespace nano {
   namespace ast {
      class Visitor;

      class Node {
      public:
         typedef std::shared_ptr<Node> PtrT;

         template<typename T, typename... Args>
         static PtrT make(Args&&... args) {
            return std::make_shared<T>(std::forward<Args>(args)...);
         }

         private:
            SourcePos _pos;

         public:
            inline Node(int line, int col)
               : _pos{line, col}
            { }

            inline virtual ~Node()
            { }

            inline SourcePos const& pos() const {
               return _pos;
            }

            virtual void visit(Visitor* v) = 0;
      };

      class ExpressionNode : public Node {
      private:
         type::Type::PtrT type_;

      public:
         inline ExpressionNode(int line, int col)
            : Node(line, col)
         { }

         inline type::Type::PtrT const& type() const {
            return type_;
         }

         inline void type(type::Type::PtrT t) {
            type_ = std::move(t);
         }
      };

      class BinaryExpressionNode : public ExpressionNode {
      private:
         PtrT _lhs, _rhs;

      public:
         inline BinaryExpressionNode(int line, int col, PtrT lhs, PtrT rhs)
            : ExpressionNode(line, col),
               _lhs(std::move(lhs)), _rhs(std::move(rhs))
         { }

         inline ExpressionNode* lhs() {
            return static_cast<ExpressionNode*>(_lhs.get());
         }

         inline ExpressionNode* rhs() {
            return static_cast<ExpressionNode*>(_rhs.get());
         }
      };
   }
}
