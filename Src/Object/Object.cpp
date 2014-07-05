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

// Nano:
#include <Nano/Object/Object.hpp>
#include <Nano/Object/OperationError.hpp>

////
// Class implementation
////

nano::object::Class::Class(GlobalContext* globalContext, ClassId id, char const* name)
    : _globalContext(globalContext), _id(id), _name(name)
{ }
            
nano::object::Class::~Class()
{ }

nano::GlobalContext* nano::object::Class::globalContext() const
{
    return _globalContext;
}
            
nano::object::ClassId nano::object::Class::id() const
{
    return _id;
}
            
char const* nano::object::Class::name() const
{
    return _name;
}
            
bool nano::object::Class::is(Object const& o) const
{
    return o.class_()->id() == id();
}

std::string nano::object::Class::prettyString(Object const& obj) const
{
    assert(is(obj));
   std::string result = "<Object of type '";
   result += name();
   return result += "'>";
}

nano::object::Object nano::object::Class::new_() const
{
    return new_(nullptr, 0);
}

nano::object::Object nano::object::Class::new_(Object* args, std::size_t argCount) const
{
    throw InvalidConstructorCallError(this, std::vector<Object>(args, args + argCount));
}

void nano::object::Class::copyData(Object const& obj, Object& target) const
{   
    assert(is(obj) && is(target));
    target._data = obj._data;
}

void nano::object::Class::delete_(Object& obj) const
{
    assert(is(obj));
    (void)obj;
}

nano::object::Object nano::object::Class::ladd(Object const& lhs, Object const& rhs) const
{
    return rhs.class_()->radd(lhs, rhs);
}

nano::object::Object nano::object::Class::radd(Object const& lhs, Object const& rhs) const
{
    throw InvalidBinaryOperationError("+", lhs.class_(), rhs.class_());
}

nano::object::Object nano::object::Class::lsub(Object const& lhs, Object const& rhs) const
{
    return rhs.class_()->rsub(lhs, rhs);
}

nano::object::Object nano::object::Class::rsub(Object const& lhs, Object const& rhs) const
{
    throw InvalidBinaryOperationError("-", lhs.class_(), rhs.class_());  
}

nano::object::Object nano::object::Class::lmul(Object const& lhs, Object const& rhs) const
{
    return rhs.class_()->rmul(lhs, rhs);
}

nano::object::Object nano::object::Class::rmul(Object const& lhs, Object const& rhs) const
{
    throw InvalidBinaryOperationError("*", lhs.class_(), rhs.class_());  
}

nano::object::Object nano::object::Class::ldiv(Object const& lhs, Object const& rhs) const
{
     return rhs.class_()->rdiv(lhs, rhs);   
}

nano::object::Object nano::object::Class::rdiv(Object const& lhs, Object const& rhs) const
{
    throw InvalidBinaryOperationError("/", lhs.class_(), rhs.class_());  
}

nano::object::Object nano::object::Class::pow(Object const& lhs, Object const& rhs) const
{
    throw InvalidBinaryOperationError("**", lhs.class_(), rhs.class_());  
}

nano::object::Object nano::object::Class::call(Object& obj, Object*, std::size_t) const
{
    throw InvalidOperationError("call", obj.class_());
}

////
// Object implementation
////

#define ASSERT_VALID_OBJECT() assert(class_() && "Usage of invalidated object")

nano::object::Object::Object(Class const* class_)
    : _class(class_), _data()
{
    assert(class_);
}

nano::object::Object::Object(Class const* class_, Data data)
    : _class(class_), _data(data)
{
    assert(class_);
}

nano::object::Object::Object(Object const& other)
    : _class(other.class_()), _data()
{
    _class->copyData(other, *this);
}

nano::object::Object::Object(Object&& other)
    : _class(other.class_()), _data(other._data)
{
    other._class = nullptr;
}

nano::object::Object& nano::object::Object::operator=(Object const& other)
{
    ASSERT_VALID_OBJECT();
    if(this != &other)
    {
        class_()->delete_(*this);
        _class = other._class;
        _class->copyData(other, *this);
    }
    return *this;
}

nano::object::Object& nano::object::Object::operator=(Object&& other)
{
    ASSERT_VALID_OBJECT();
    if(this != &other)
    {
        class_()->delete_(*this);
        _class = other._class;
        _data = other._data;
        other._class = nullptr;
    }
    return *this;
}

nano::object::Object::~Object()
{
    if(class_())
        class_()->delete_(*this);
}

nano::object::Class const* nano::object::Object::class_() const
{
    return _class;
}

std::string nano::object::Object::prettyString() const
{
    ASSERT_VALID_OBJECT();
    return class_()->prettyString(*this);
}

nano::object::Object nano::object::Object::operator()(Object* args, std::size_t argCount)
{
    ASSERT_VALID_OBJECT();
    return class_()->call(*this, args, argCount);
}

////
// Object operator overloads and free functions.
////

nano::object::Object nano::object::operator+(Object const& lhs, Object const& rhs)
{
    return lhs.class_()->ladd(lhs, rhs);
}

nano::object::Object nano::object::operator-(Object const& lhs, Object const& rhs)
{
    return lhs.class_()->lsub(lhs, rhs);
}

nano::object::Object nano::object::operator*(Object const& lhs, Object const& rhs)
{
    return lhs.class_()->lmul(lhs, rhs);
}

nano::object::Object nano::object::operator/(Object const& lhs, Object const& rhs)
{
    return lhs.class_()->ldiv(lhs, rhs);
}

nano::object::Object nano::object::pow(Object const& base, Object const& exp)
{
    return base.class_()->pow(base, exp);
}