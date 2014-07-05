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
#include <cstdint>
#include <string>
#include <functional>

namespace nano
{
    class GlobalContext;
    
    namespace object
    {
        struct Object;
        
        typedef std::uint32_t ClassId;
        class Class
        {
        protected:
            GlobalContext* _globalContext;
            ClassId _id;
            char const* _name;
            
        public:
            Class(GlobalContext* globalContext, ClassId id, char const* name);
            virtual ~Class();
            
            GlobalContext* globalContext() const;
            
            ClassId id() const;
            char const* name() const;
            bool is(Object const& o) const;
            
            virtual std::string prettyString(Object const& o) const;
            
            virtual Object new_() const;
            virtual Object new_(Object* args, std::size_t argCount) const;
            virtual void copyData(Object const& obj, Object& target) const;
            virtual void delete_(Object& obj) const;
            
            virtual Object ladd(Object const& lhs, Object const& rhs) const;
            virtual Object radd(Object const& lhs, Object const& rhs) const;
            virtual Object lsub(Object const& lhs, Object const& rhs) const;
            virtual Object rsub(Object const& lhs, Object const& rhs) const;
            virtual Object lmul(Object const& lhs, Object const& rhs) const;
            virtual Object rmul(Object const& lhs, Object const& rhs) const;
            virtual Object ldiv(Object const& lhs, Object const& rhs) const;
            virtual Object rdiv(Object const& lhs, Object const& rhs) const;
            virtual Object pow(Object const& lhs, Object const& rhs) const;
            
            virtual Object call(Object& obj, Object* args, std::size_t argCount) const;
        };
        
        class Object
        {
        private:
            Class const* _class;
            
        public:
            struct RefCountedData
            {
                int refCount;
                
                inline RefCountedData()
                    : refCount(1)
                { }
                
                inline void ref()
                {
                    ++refCount;
                }
                
                inline void unref()
                {
                    --refCount;
                }
                
                inline bool isLastRef() const
                {
                    return refCount == 1;
                }
            };
            
            struct TupleData;
            struct NativeFunctionData : public RefCountedData
            {
                std::string name;
                std::function<Object (Object&, Object*, std::size_t)> function;
            };
            
            // Needs to be public so the class objects can work with it.
            // Do not abuse it!
            union Data
            {
                std::int64_t intval;
                double floatval;
                int boolval;
                TupleData* tupleval;
                Class const* classval;
                NativeFunctionData* nfunctionval;
            } _data;
            
            explicit Object(Class const* class_);
            Object(Class const* class_, Data data);
            Object(Object const& other);
            Object(Object&& other);
            Object& operator=(Object const& other);
            Object& operator=(Object&& other);
            ~Object();
            
            Class const* class_() const;
            
            std::string prettyString() const;
            
            Object operator()(Object* args, std::size_t argCount);
        };
        
        struct Object::TupleData : public RefCountedData
        {
                std::size_t length;
                Object elements[1];
        };
        
        Object operator+(Object const& lhs, Object const& rhs);
        Object operator-(Object const& lhs, Object const& rhs);
        Object operator*(Object const& lhs, Object const& rhs);
        Object operator/(Object const& lhs, Object const& rhs);
        Object pow(Object const& base, Object const& exp);
    }
}

#endif // HEADER_UUID_AF709BAFD56C482889F8D7B055BDA71A