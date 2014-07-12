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

// Nano:
#include <Nano/Object/Int.hpp>
#include <Nano/Object/Float.hpp>
#include <Nano/Object/Detail.hpp>

////
// FloatClass implementation
////

nano::object::ClassId nano::object::FloatClass::id() const
{
    return CLASSID_FLOAT;
}

char const* nano::object::FloatClass::name() const
{
    return "Float";
}
            
nano::object::ObjectPtr nano::object::FloatClass::new_() const
{
    return allocate<FloatObject>();
}

nano::object::ObjectPtr nano::object::FloatClass::new_(ObjectPtr* args, unsigned argCount) const
{
    // Default constructor.
    if(argCount == 0)
        return allocate<FloatObject>();
    
    // Construction from a single arg.
    else if(argCount == 1)
    {
        switch((*args)->class_()->id())
        {
            case CLASSID_INT:
            {
                auto intObj = static_cast<IntObject*>(args->get());
                return allocate<FloatObject>(intObj->value().convert_to<CppFloatType>());
            }
            
            case CLASSID_FLOAT:
            {
                auto floatObj = static_cast<FloatObject*>(args->get());
                return allocate<FloatObject>(floatObj->value());
            }
        }
    }
    
    // Let base class handle construction error.
    return Class::new_(args, argCount);
}

////
// FloatObject implementation
////
        
nano::object::FloatObject::FloatObject()
    : _value()
{ }

nano::object::FloatObject::FloatObject(CppFloatType const& value)
    : _value(value)
{ }

nano::object::FloatObject::FloatObject(const char* str)
    : _value(str)
{ }
            
nano::object::CppFloatType const& nano::object::FloatObject::value() const
{
    return _value;
}
            
nano::object::ClassPtr const& nano::object::FloatObject::class_() const
{
    static ClassPtr s_class = std::make_shared<FloatClass>();
    return s_class;
}

nano::object::ObjectPtr nano::object::FloatObject::copy(bool) const
{
    return allocate<FloatObject>(_value);
}

std::string nano::object::FloatObject::prettyString() const
{
    return _value.str();
}
            
nano::object::ObjectPtr nano::object::FloatObject::ladd(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<FloatObject>(_value + intObj->value().convert_to<CppFloatType>());
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            return allocate<FloatObject>(_value + floatObj->_value);
        }
        
        default:
            return Object::ladd(other);
    }
}

nano::object::ObjectPtr nano::object::FloatObject::lsub(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<FloatObject>(_value - intObj->value().convert_to<CppFloatType>());
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            return allocate<FloatObject>(_value - floatObj->_value);
        }
        
        default:
            return Object::lsub(other);
    }
}

nano::object::ObjectPtr nano::object::FloatObject::lmul(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<FloatObject>(_value * intObj->value().convert_to<CppFloatType>());
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            return allocate<FloatObject>(_value * floatObj->_value);
        }
        
        default:
            return Object::lmul(other);
    }
}

nano::object::ObjectPtr nano::object::FloatObject::ldiv(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<FloatObject>(_value / intObj->value().convert_to<CppFloatType>());
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            return allocate<FloatObject>(_value / floatObj->_value);
        }
        
        default:
            return Object::ldiv(other);
    }
}

nano::object::ObjectPtr nano::object::FloatObject::pow(ObjectPtr const& exp) const
{
    switch(exp->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intExp = static_cast<IntObject const*>(exp.get());
            return allocate<FloatObject>(detail::ipow(_value, intExp->value()));
        }
        
        case CLASSID_FLOAT:
        {
            auto floatExp = static_cast<FloatObject const*>(exp.get());
            if(floatExp->value() == 2.0)
                return allocate<FloatObject>(_value * _value);
            
            CppFloatType result = boost::multiprecision::pow(_value, floatExp->value());
            return allocate<FloatObject>(std::move(result));
        }
        
        default:
            return Object::pow(exp);
    }
}

bool nano::object::isFloat(ObjectPtr const& obj)
{
    return obj->class_()->id() == CLASSID_FLOAT;
}