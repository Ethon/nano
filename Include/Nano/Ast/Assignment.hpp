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

#ifndef HEADER_UUID_35DFDC58D5EF4E299A4CBF8E04E011B7
#define HEADER_UUID_35DFDC58D5EF4E299A4CBF8E04E011B7

// Nano:
#include <Nano/Ast/Node.hpp>
#include <Nano/Ast/Visitor.hpp>

namespace nano
{
    namespace ast
    {
        class AssignmentNode : public Node
        {
        private:
            PtrT _lhs;
            PtrT _rhs;
            
        public:
            inline AssignmentNode(int line, int col, PtrT lhs, PtrT rhs)
                : Node(line, col), _lhs(std::move(lhs)), _rhs(std::move(rhs))
            { }
            
            inline Node* lhs()
            {
                return _lhs.get();
            }
            
            inline Node* rhs()
            {
                return _rhs.get();
            }
            
            inline virtual void visit(Visitor* v) override
            {
                v->accept(this);
            }
        };
    }
}

#endif // HEADER_UUID_35DFDC58D5EF4E299A4CBF8E04E011B7