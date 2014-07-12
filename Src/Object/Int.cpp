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
#include <sstream>

// Nano:
#include <Nano/Object/Int.hpp>
#include <Nano/Object/Float.hpp>
#include <Nano/Object/Detail.hpp>

namespace
{
    nano::object::CppIntType floatToInt(nano::object::CppFloatType const& floatVal)
    {
        // Replace the decimal seperator with a terminating zero.
        auto str = floatVal.str(0, std::ios_base::fixed);
        size_t sep = str.find('.');
        if(sep != std::string::npos)
            str[sep] = 0;
        
        return nano::object::CppIntType(str.c_str());
    }
}

////
// IntClass implementation
////

nano::object::ClassId nano::object::IntClass::id() const
{
    return CLASSID_INT;
}

char const* nano::object::IntClass::name() const
{
    return "Int";
}

nano::object::ObjectPtr nano::object::IntClass::new_() const
{
    return allocate<IntObject>();
}

nano::object::ObjectPtr nano::object::IntClass::new_(ObjectPtr* args, unsigned argCount) const
{
    // Default constructor.
    if(argCount == 0)
        return allocate<IntObject>();
    
    // Construction from a single arg.
    else if(argCount == 1)
    {
        switch((*args)->class_()->id())
        {
            case CLASSID_INT:
            {
                auto intObj = static_cast<IntObject*>(args->get());
                return allocate<IntObject>(intObj->value());
            }
            
            case CLASSID_FLOAT:
            {
                auto floatObj = static_cast<FloatObject*>(args->get());
                return allocate<IntObject>(floatToInt(floatObj->value()));
            }
        }
    }
    
    // Let base class handle construction error.
    return Class::new_(args, argCount);
}

////
// IntObject implementation
////

nano::object::IntObject::IntObject()
    : _value()
{ }

nano::object::IntObject::IntObject(CppIntType const& value)
    : _value(value)
{ }

nano::object::IntObject::IntObject(const char* str)
    : _value(str)
{ }

nano::object::CppIntType const& nano::object::IntObject::value() const
{
    return _value;
}

nano::object::ClassPtr const& nano::object::IntObject::class_() const
{
    static ClassPtr s_class = std::make_shared<IntClass>();
    return s_class;
}

nano::object::ObjectPtr nano::object::IntObject::copy(bool) const
{
    return std::make_shared<IntObject>(_value);
}

std::string nano::object::IntObject::prettyString() const
{
    return _value.str();
}

nano::object::ObjectPtr nano::object::IntObject::ladd(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<IntObject>(_value + intObj->_value);
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            auto result = _value.convert_to<CppFloatType>();
            result += floatObj->value();
            return allocate<FloatObject>(std::move(result));
        }
        
        default:
            return Object::ladd(other);
    }
}

nano::object::ObjectPtr nano::object::IntObject::lsub(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<IntObject>(_value - intObj->_value);
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            auto result = _value.convert_to<CppFloatType>();
            result -= floatObj->value();
            return allocate<FloatObject>(std::move(result));
        }
        
        default:
            return Object::lsub(other);
    }
}

nano::object::ObjectPtr nano::object::IntObject::lmul(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            return allocate<IntObject>(_value * intObj->_value);
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            auto result = _value.convert_to<CppFloatType>();
            result *= floatObj->value();
            return allocate<FloatObject>(std::move(result));
        }
        
        default:
            return Object::lmul(other);
    }
}

nano::object::ObjectPtr nano::object::IntObject::ldiv(ObjectPtr const& other) const
{
    switch(other->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intObj = static_cast<IntObject const*>(other.get());
            auto result = _value.convert_to<CppFloatType>();
            result /= intObj->_value.convert_to<CppFloatType>();
            return allocate<FloatObject>(std::move(result));
        }
        
        case CLASSID_FLOAT:
        {
            auto floatObj = static_cast<FloatObject const*>(other.get());
            auto result = _value.convert_to<CppFloatType>();
            result /= floatObj->value();
            return allocate<FloatObject>(std::move(result));
        }
        
        default:
            return Object::ldiv(other);
    }
}

nano::object::ObjectPtr nano::object::IntObject::pow(ObjectPtr const& exp) const
{
    switch(exp->class_()->id())
    {
        case CLASSID_INT:
        {
            auto intExp = static_cast<IntObject const*>(exp.get());
            return allocate<IntObject>(detail::ipow(_value, intExp->_value));
        }
        
        case CLASSID_FLOAT:
        {
            auto floatExp = static_cast<FloatObject const*>(exp.get());
            if(floatExp->value() == 2.0)
                return allocate<IntObject>(_value * _value);
            
            auto base = _value.convert_to<CppFloatType>();
            CppFloatType result = boost::multiprecision::pow(base, floatExp->value());
            return allocate<FloatObject>(std::move(result));
        }
        
        default:
            return Object::pow(exp);
    }
}


bool nano::object::isInt(ObjectPtr const& obj)
{
    return obj->class_()->id() == CLASSID_INT;
}