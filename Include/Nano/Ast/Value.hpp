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
#include <string>

// Boost:
#include <boost/multiprecision/cpp_int.hpp> 
#include <boost/multiprecision/cpp_dec_float.hpp> 

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>

namespace nano {
   namespace ast {
      class IntNode : public ExpressionNode
      {
      private:
         boost::multiprecision::cpp_int _val;

      public:
         inline IntNode(int line, int col, boost::multiprecision::cpp_int val)
            : ExpressionNode(line, col), _val(std::move(val))
         { }

         inline boost::multiprecision::cpp_int const& value() {
            return _val;
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };

      class FloatNode : public ExpressionNode
      {
      private:
         boost::multiprecision::cpp_dec_float_50 _val;

      public:
         inline FloatNode(int line, int col,
                          boost::multiprecision::cpp_dec_float_50 val)
            : ExpressionNode(line, col), _val(std::move(val))
         { }

         inline boost::multiprecision::cpp_dec_float_50 const& value() {
            return _val;
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };

      class VarNode : public ExpressionNode
      {
      private:
         std::string _name;

      public:
         inline VarNode(int line, int col, std::string name)
            : ExpressionNode(line, col), _name(name)
         { }

         inline std::string const& value() {
            return _name;
         }

         inline virtual void visit(Visitor* v) override {
            v->accept(this);
         }
      };
   }
}
