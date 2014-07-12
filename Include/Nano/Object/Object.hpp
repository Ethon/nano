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

#ifndef HEADER_UUID_AF709BAFD56C482889F8D7B055BDA71A
#define HEADER_UUID_AF709BAFD56C482889F8D7B055BDA71A

// C++ Standard Library:
#include <string>
#include <memory>
#include <type_traits>

// Nano:
#include <Nano/Object/BuiltinClassIds.hpp>

namespace nano
{
    class GlobalContext;
    
    namespace object
    {
        struct Object;
        typedef std::shared_ptr<Object> ObjectPtr;
        typedef std::shared_ptr<Object const> ConstObjectPtr;
        
        class Class : public std::enable_shared_from_this<Class>
        {
        public:
            virtual ~Class();
            
            virtual ClassId id() const = 0;
            virtual char const* name() const = 0;
            
            virtual ObjectPtr new_() const;
            virtual ObjectPtr new_(ObjectPtr* args, unsigned argCount) const;
        };
        typedef std::shared_ptr<Class const> ClassPtr;
        
        class Object : public std::enable_shared_from_this<Object>
        {
        public:
            virtual ~Object();
            
            // Management:
            virtual ClassPtr const& class_() const = 0;
            virtual ObjectPtr copy(bool deep) const = 0;
            
            // Operations:
            virtual std::string prettyString() const;
            
            // Binary Arithmetic:
            virtual ObjectPtr ladd(ObjectPtr const& other) const;
            virtual ObjectPtr radd(ObjectPtr const& other) const;
            virtual ObjectPtr lsub(ObjectPtr const& other) const;
            virtual ObjectPtr rsub(ObjectPtr const& other) const;
            virtual ObjectPtr lmul(ObjectPtr const& other) const;
            virtual ObjectPtr rmul(ObjectPtr const& other) const;
            virtual ObjectPtr ldiv(ObjectPtr const& other) const;
            virtual ObjectPtr rdiv(ObjectPtr const& other) const;
            virtual ObjectPtr pow(ObjectPtr const& exp) const;
            
            // Other Expressions:
            virtual ObjectPtr call(ObjectPtr* args, unsigned argCount);
        };
        
        ObjectPtr operator+(ObjectPtr const& lhs, ObjectPtr const& rhs);
        ObjectPtr operator-(ObjectPtr const& lhs, ObjectPtr const& rhs);
        ObjectPtr operator*(ObjectPtr const& lhs, ObjectPtr const& rhs);
        ObjectPtr operator/(ObjectPtr const& lhs, ObjectPtr const& rhs);
        ObjectPtr pow(ObjectPtr const& base, ObjectPtr const& exp);
        
        template<typename T, typename... Args>
        ObjectPtr allocate(Args&&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    }
}

#endif // HEADER_UUID_AF709BAFD56C482889F8D7B055BDA71A