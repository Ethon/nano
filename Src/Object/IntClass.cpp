// C++ Standard Library:
#include <cassert>
#include <cmath>

// Nano:
#include <Nano/Object/IntClass.hpp>
#include <Nano/GlobalContext.hpp>

nano::object::IntClass::IntClass(GlobalContext* globalContext, ClassId id)
    : Class(globalContext, id, "Int")
{ }

std::string nano::object::IntClass::prettyString(Object const& o) const
{
    assert(is(o));
    return std::to_string(o._data.intval);
}
            
nano::object::Object nano::object::IntClass::new_() const
{
    return new_(0);
}

nano::object::Object nano::object::IntClass::new_(Object* args, std::size_t argCount) const
{
    // Default constructor.
    if(argCount == 0)
        return new_();
    
    if(argCount == 1)
    {
        // Just copy int args.
        if(is(args[0]))
            return new_(args[0]._data.intval);
        
        // Truncate float args.
        if(isFloat(args[0]))
            return new_(static_cast<std::int64_t>(args[0]._data.floatval));
    }
    
    // Let base class do the error handling.
    return Class::new_(args, argCount);
}

nano::object::Object nano::object::IntClass::new_(std::int64_t intval) const
{
    Object result(this);
    result._data.intval = intval;
    return result;
}

nano::object::Object nano::object::IntClass::ladd(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle int + int
    if(is(rhs))
        return new_(lhs._data.intval + rhs._data.intval);
    
    // Handle int + float
    auto floatClass = globalContext()->getFloatClass();
    if(floatClass->is(rhs))
        return floatClass->new_(double(lhs._data.intval) + rhs._data.floatval);
    
    return Class::ladd(lhs, rhs);
}

nano::object::Object nano::object::IntClass::lsub(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle int - int
    if(is(rhs))
        return new_(lhs._data.intval - rhs._data.intval);
    
    // Handle int - float
    auto floatClass = globalContext()->getFloatClass();
    if(floatClass->is(rhs))
        return floatClass->new_(double(lhs._data.intval) - rhs._data.floatval);
    
    return Class::lsub(lhs, rhs);
}

nano::object::Object nano::object::IntClass::lmul(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle int * int
    if(is(rhs))
        return new_(lhs._data.intval * rhs._data.intval);
    
    // Handle int - float
    auto floatClass = globalContext()->getFloatClass();
    if(floatClass->is(rhs))
        return floatClass->new_(double(lhs._data.intval) * rhs._data.floatval);
    
    return Class::lmul(lhs, rhs);
}

nano::object::Object nano::object::IntClass::ldiv(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle int / int
    auto floatClass = globalContext()->getFloatClass();
    if(is(rhs))
        return floatClass->new_(double(lhs._data.intval) / rhs._data.intval);
    
    // Handle int / float
    if(floatClass->is(rhs))
        return floatClass->new_(double(lhs._data.intval) / rhs._data.floatval);
    
    return Class::ldiv(lhs, rhs);
}

nano::object::Object nano::object::IntClass::pow(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle int ** int
    if(is(rhs))
        return new_(std::pow(lhs._data.intval, rhs._data.intval));
    
    // Handle int ** float
    auto floatClass = globalContext()->getFloatClass();
    if(floatClass->is(rhs))
        return floatClass->new_(std::pow(lhs._data.intval, rhs._data.floatval));
    
    return Class::pow(lhs, rhs);
}

bool nano::object::isInt(Object const& obj)
{
    return obj.class_()->globalContext()->getIntClass()->is(obj);
}