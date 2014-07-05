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

#ifndef HEADER_UUID_35AE3154F13F40B48B6A58A6FAD420FF
#define HEADER_UUID_35AE3154F13F40B48B6A58A6FAD420FF

// Nano:
#include <Nano/GlobalContext.hpp>
#include <Nano/Ast/Node.hpp>

namespace nano
{
    class Interpreter
    {
    private:
        GlobalContext& _ctx;
        
    public:
        explicit Interpreter(GlobalContext& ctx);
        
        object::Object evaluateExpression(ast::Node* expressionAst);
    };
}

#endif // HEADER_UUID_35AE3154F13F40B48B6A58A6FAD420FF