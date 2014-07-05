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

#ifndef HEADER_UUID_D221FA69AA99425599FD65C5A8721B50
#define HEADER_UUID_D221FA69AA99425599FD65C5A8721B50

// C++ Standard Library:
#include <vector>

// Nano:
#include <Nano/Error.hpp>
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        class InvalidOperationError : public EvaluationError
        {
        private:
            char const* _operation;
            Class const* _objectClass;
            
        public:
            inline InvalidOperationError(char const* operation, Class const* objectClass,
                    SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid operation"),
                  _operation(operation), _objectClass(objectClass)
            { }
            
            char const* operation() const
            {
                return _operation;
            }
            
            Class const* objectClass() const
            {
                return _objectClass;
            }
        };
        
        class InvalidBinaryOperationError : public EvaluationError
        {
        private:
            char const* _operation;
            Class const* _lhsClass;
            Class const* _rhsClass;
            
        public:
            inline InvalidBinaryOperationError(char const* operation, Class const* lhsClass,
                    Class const* rhsClass, SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid binary operation"),
                  _operation(operation), _lhsClass(lhsClass), _rhsClass(rhsClass)
            { }
            
            char const* operation() const
            {
                return _operation;
            }
            
            Class const* lhsClass() const
            {
                return _lhsClass;
            }
            
            Class const* rlhsClass() const
            {
                return _rhsClass;
            }
        };
        
        class InvalidConstructorCallError : public EvaluationError
        {
        private:
            Class const* _class;
            std::vector<Object> _args;
            
        public:
            inline InvalidConstructorCallError(Class const* class_, std::vector<Object>&& args,
                    SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid constructor call"),
                  _class(class_), _args(std::move(args))
            { }
            
            Class const* class_() const
            {
                return _class;
            }
            
            std::vector<Object> const& args() const
            {
                return _args;
            }
        };
    }
}

#endif // HEADER_UUID_D221FA69AA99425599FD65C5A8721B50