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

// C++ Standard Library:
#include <cassert>
#include <iostream>
#include <cstdio>

// Readline:
#include <readline/readline.h>
#include <readline/history.h>

// Nano:
#include <Nano/Detail/ParseContext.hpp>
#include <Nano/GlobalContext.hpp>
#include <Nano/Interpreter.hpp>
#include <Nano/Object/OperationError.hpp>
#include <Nano/Object/Int.hpp>
            
namespace builtin
{
    nano::object::ObjectPtr acc(nano::object::ObjectPtr&,
            nano::object::ObjectPtr* args, std::size_t count)
    {
        nano::object::ObjectPtr sum = std::make_shared<nano::object::IntObject>();
        for(std::size_t i = 0; i < count; ++i)
            sum = sum + *(args + i);
        return sum;
    }
}

int eval(nano::detail::ParseContext& pctx, nano::Interpreter& interpreter)
{
    int count = 0;
    while(nano::ast::Node::PtrT ast = pctx.parseOne())
    {
        try
        {
            std::cout << interpreter.evaluateExpression(ast.get())->prettyString() << std::endl;
        }
        catch(nano::UnknownObjectError const& e)
        {
            std::cout << "Error: '" << e.objectName() << "' is unknown" <<  std::endl;
        }
        catch(nano::object::InvalidBinaryOperationError const& e)
        {
            std::cout << "Error: Can't apply operation '" << e.operation() << "' to objects of type '"
                << e.lhs()->class_()->name() << "' and '" << e.rhs()->class_()->name() << "'" << std::endl;
        }
        ++count;
    }
    return count;
}

int main(int argc, char** argv)
{
    nano::GlobalContext gctx;
    nano::Interpreter interpreter((gctx));
    
    if(argc == 2)
    {
        nano::detail::ParseContext pctx(argv[1], nano::detail::ParseContext::FileInput());
        eval(pctx, interpreter);
    }
    else
    {
        for(;;)
        {
            std::unique_ptr<char, void(*)(void*)> line(readline("> "), &std::free); 
            if(!line)
            {
                std::cout << std::endl;
                break;
            }
            add_history(line.get());
            nano::detail::ParseContext pctx(line.get(), nano::detail::ParseContext::StringInput());
            eval(pctx, interpreter);
        }
    }
}