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

#ifndef HEADER_UUID_43DEB27742CC4EAA9CD5F1ED4B8118C5
#define HEADER_UUID_43DEB27742CC4EAA9CD5F1ED4B8118C5

// C++ Standard Library:
#include <string>

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>
#include <Nano/Object/Int.hpp>

namespace nano
{
    namespace ast
    {
        class IntNode : public Node
        {
        private:
            object::CppIntType _val;
            
        public:
            inline IntNode(int line, int col, object::CppIntType val)
                : Node(line, col), _val(std::move(val))
            { }
            
            inline object::CppIntType const& value()
            {
                return _val;
            }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
        
        class FloatNode : public Node
        {
        private:
            double _val;
            
        public:
            inline FloatNode(int line, int col, double val)
                : Node(line, col), _val(val)
            { }
            
            inline double value()
            {
                return _val;
            }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
        
        class VarNode : public Node
        {
        private:
            std::string _name;
            
        public:
            inline VarNode(int line, int col, std::string name)
                : Node(line, col), _name(name)
            { }
            
            inline std::string const& value()
            {
                return _name;
            }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
    }
}

#endif // HEADER_UUID_43DEB27742CC4EAA9CD5F1ED4B8118C5