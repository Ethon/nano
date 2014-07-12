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

#ifndef HEADER_UUID_A26AE361182A4BA08666FFE820766A7C
#define HEADER_UUID_A26AE361182A4BA08666FFE820766A7C

// C++ Standard Library:
#include <type_traits>

// Boost:
#include <boost/multiprecision/cpp_int.hpp> 

// Nano:
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        typedef boost::multiprecision::cpp_int CppIntType;
        
        class IntClass : public Class
        {
        public:
            virtual ClassId id() const override;
            virtual char const* name() const override;
            
            virtual ObjectPtr new_() const override;
            virtual ObjectPtr new_(ObjectPtr* args, unsigned argCount) const override;
        };
        
        class IntObject : public Object
        {
        private:
            CppIntType _value;
            
        public:
            // Constructors:
            IntObject();
            IntObject(CppIntType const& value);
            template<typename IntT>
            IntObject(typename std::enable_if<std::is_integral<IntT>::value, IntT>::type value)
                : _value(value)
            { }
            IntObject(const char* str);
            
            // Internals:
            CppIntType const& value() const;
            
            // Management:
            virtual ClassPtr const& class_() const override;
            virtual ObjectPtr copy(bool deep) const override;
            
            // Operations:
            virtual std::string prettyString() const override;
            
            // Binary Arithmetic:
            virtual ObjectPtr ladd(ObjectPtr const& other) const override;
            virtual ObjectPtr lsub(ObjectPtr const& other) const override;
            virtual ObjectPtr lmul(ObjectPtr const& other) const override;
            virtual ObjectPtr ldiv(ObjectPtr const& other) const override;
            virtual ObjectPtr pow(ObjectPtr const& exp) const override;
        };
        
        bool isInt(ObjectPtr const& obj);
    }
}

#endif // HEADER_UUID_A26AE361182A4BA08666FFE820766A7C
