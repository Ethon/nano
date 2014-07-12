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
#include <sstream>

// Nano:
#include <Nano/Object/Object.hpp>
#include <Nano/Object/OperationError.hpp>

////
// Class implementation
////

nano::object::Class::~Class()
{ }

nano::object::ObjectPtr nano::object::Class::new_() const
{
    throw InvalidConstructorCallError(shared_from_this(),
        std::vector<ObjectPtr>());
}

nano::object::ObjectPtr nano::object::Class::new_(ObjectPtr* args, unsigned argCount) const
{
    throw InvalidConstructorCallError(shared_from_this(),
        std::vector<ObjectPtr>(args, args + argCount));
}

////
// Object implementation
////

nano::object::Object::~Object()
{ }
            
std::string nano::object::Object::prettyString() const
{
    std::ostringstream oss;
    oss << '<' << class_()->name() << " object @ "
        << this << '>';
    return oss.str();
}
            
nano::object::ObjectPtr nano::object::Object::ladd(ObjectPtr const& other) const
{
    return radd(other);
}

nano::object::ObjectPtr nano::object::Object::radd(ObjectPtr const& other) const
{
    throw InvalidBinaryOperationError("+", shared_from_this(), other);
}

nano::object::ObjectPtr nano::object::Object::lsub(ObjectPtr const& other) const
{
    return rsub(other);
}

nano::object::ObjectPtr nano::object::Object::rsub(ObjectPtr const& other) const
{
    throw InvalidBinaryOperationError("-", shared_from_this(), other);
}

nano::object::ObjectPtr nano::object::Object::lmul(ObjectPtr const& other) const
{
    return rmul(other);
}

nano::object::ObjectPtr nano::object::Object::rmul(ObjectPtr const& other) const
{
    throw InvalidBinaryOperationError("*", shared_from_this(), other);
}

nano::object::ObjectPtr nano::object::Object::ldiv(ObjectPtr const& other) const
{
    return rdiv(other);
}

nano::object::ObjectPtr nano::object::Object::rdiv(ObjectPtr const& other) const
{
    throw InvalidBinaryOperationError("/", shared_from_this(), other);
}

nano::object::ObjectPtr nano::object::Object::pow(ObjectPtr const& exp) const
{
    throw InvalidBinaryOperationError("**", shared_from_this(), exp);
}
            
nano::object::ObjectPtr nano::object::Object::call(ObjectPtr*, unsigned)
{
    throw InvalidOperationError("call", shared_from_this());
}


////
// Object operator overloads and free functions.
////

nano::object::ObjectPtr nano::object::operator+(ObjectPtr const& lhs, ObjectPtr const& rhs)
{
    return lhs->ladd(rhs);
}

nano::object::ObjectPtr nano::object::operator-(ObjectPtr const& lhs, ObjectPtr const& rhs)
{
    return lhs->lsub(rhs);
}

nano::object::ObjectPtr nano::object::operator*(ObjectPtr const& lhs, ObjectPtr const& rhs)
{
    return lhs->lmul(rhs);
}

nano::object::ObjectPtr nano::object::operator/(ObjectPtr const& lhs, ObjectPtr const& rhs)
{
    return lhs->ldiv(rhs);
}

nano::object::ObjectPtr nano::object::pow(ObjectPtr const& base, ObjectPtr const& exp)
{
    return base->pow(exp);
}