// C++ Standard Library:
#include <cassert>
#include <cmath>

// Nano:
#include <Nano/GlobalContext.hpp>

nano::object::FloatClass::FloatClass(GlobalContext* globalContext, ClassId id)
    : Class(globalContext, id, "Float")
{ }

std::string nano::object::FloatClass::prettyString(Object const& o) const
{
    assert(is(o));
    return std::to_string(o._data.floatval);
}

nano::object::Object nano::object::FloatClass::new_() const
{
    return new_(0.0);
}

nano::object::Object nano::object::FloatClass::new_(
        Object* args, std::size_t argCount) const
{
    // Default constructor.
    if(argCount == 0)
        return new_();
    
    if(argCount == 1)
    {
        // Just copy float args.
        if(is(args[0]))
            return new_(args[0]._data.floatval);
        
        // Just copy int args.
        if(isInt(args[0]))
            return new_(args[0]._data.intval);
    }
    
    // Let base class do the error handling.
    return Class::new_(args, argCount);
}

nano::object::Object nano::object::FloatClass::new_(double floatval) const
{
    Object result(this);
    result._data.floatval = floatval;
    return result;
}

nano::object::Object nano::object::FloatClass::ladd(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle float + float
    if(is(rhs))
        return new_(lhs._data.floatval + rhs._data.floatval);
    
    // Handle float + int
    if(isInt(rhs))
        return new_(lhs._data.floatval + rhs._data.intval);
    
    return Class::ladd(lhs, rhs);
}

nano::object::Object nano::object::FloatClass::lsub(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle float - float
    if(is(rhs))
        return new_(lhs._data.floatval - rhs._data.floatval);
    
    // Handle float - int
    if(isInt(rhs))
        return new_(lhs._data.floatval - rhs._data.intval);
    
    return Class::lsub(lhs, rhs);
}

nano::object::Object nano::object::FloatClass::lmul(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle float * float
    if(is(rhs))
        return new_(lhs._data.floatval * rhs._data.floatval);
    
    // Handle float * int
    if(isInt(rhs))
        return new_(lhs._data.floatval * rhs._data.intval);
    
    return Class::lmul(lhs, rhs);
}

nano::object::Object nano::object::FloatClass::ldiv(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle float / float
    if(is(rhs))
        return new_(lhs._data.floatval / rhs._data.floatval);
    
    // Handle float / int
    if(isInt(rhs))
        return new_(lhs._data.floatval / rhs._data.intval);
    
    return Class::ldiv(lhs, rhs);
}

nano::object::Object nano::object::FloatClass::pow(Object const& lhs, Object const& rhs) const
{
    assert(is(lhs));
    
    // Handle float ** float
    if(is(rhs))
        return new_(std::pow(lhs._data.floatval, rhs._data.floatval));
    
    // Handle float ** int
    if(globalContext()->getIntClass()->is(rhs))
        return new_(std::pow(lhs._data.floatval, rhs._data.intval));
    
    return Class::pow(lhs, rhs);
}


bool nano::object::isFloat(Object const& obj)
{
    return obj.class_()->globalContext()->getFloatClass()->is(obj);
}