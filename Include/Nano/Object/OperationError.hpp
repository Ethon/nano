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
            ObjectPtr _object;
            
        public:
            inline InvalidOperationError(char const* operation, ObjectPtr object,
                    SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid operation"),
                  _operation(operation), _object(std::move(object))
            { }
            
            char const* operation() const
            {
                return _operation;
            }
            
            ObjectPtr const& object() const
            {
                return _object;
            }
        };
        
        class InvalidBinaryOperationError : public EvaluationError
        {
        private:
            char const* _operation;
            ObjectPtr _lhs;
            ObjectPtr _rhs;
            
        public:
            inline InvalidBinaryOperationError(char const* operation, ObjectPtr lhs,
                    ObjectPtr rhs, SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid binary operation"),
                  _operation(operation), _lhs(std::move(lhs)), _rhs(std::move(rhs))
            { }
            
            char const* operation() const
            {
                return _operation;
            }
            
            ObjectPtr const& lhs() const
            {
                return _lhs;
            }
            
            ObjectPtr const& rhs() const
            {
                return _rhs;
            }
        };
        
        class InvalidConstructorCallError : public EvaluationError
        {
        private:
            ClassPtr _class;
            std::vector<ObjectPtr> _args;
            
        public:
            inline InvalidConstructorCallError(ClassPtr class_, std::vector<ObjectPtr> args,
                    SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid constructor call"),
                  _class(std::move(class_)), _args(std::move(args))
            { }
            
            ClassPtr const& class_() const
            {
                return _class;
            }
            
            std::vector<ObjectPtr> const& args() const
            {
                return _args;
            }
        };
        
        class InvalidAssignment : public EvaluationError
        {
        public:
            inline InvalidAssignment(SourcePos pos = SourcePos())
                : EvaluationError(pos, "invalid assignment")
            { }
        };
    }
}

#endif // HEADER_UUID_D221FA69AA99425599FD65C5A8721B50