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

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>

namespace nano
{
   namespace ast
   {
      class CallNode : public ExpressionNode
      {
      private:
         PtrT _target;
         PtrT _args;

      public:
         inline CallNode(int line, int col, PtrT target, PtrT args)
            : ExpressionNode(line, col),
               _target(std::move(target)), _args(std::move(args))
         { }

         inline Node* target() {
            return _target.get();
         }

         inline Node* args() {
            return _args.get();
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };
   }
}
