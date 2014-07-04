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


#ifndef HEADER_UUID_6242DFF369EA4DD1907BDDF0173ADB0C
#define HEADER_UUID_6242DFF369EA4DD1907BDDF0173ADB0C

// C++ Standard Library:
#include <cstdint>
#include <string>
#include <vector>

namespace nano
{
    namespace util
    {
        namespace detail
        {
            std::uint32_t const FNV_DEFAULT_PRIME_32        = 16777619u;
            std::uint64_t const FNV_DEFAULT_PRIME_64        = 1099511628211ull;

            std::uint32_t const FNV_DEFAULT_OFFSETBASIS_32  = 2166136261u;
            std::uint64_t const FNV_DEFAULT_OFFSETBASIS_64  = 14695981039346656037ull;

            template<unsigned Bits>
            struct FnvDefaultParams
            { };
            
            template<>
            struct FnvDefaultParams<32>
            {
                constexpr static std::uint32_t prime()
                {
                    return FNV_DEFAULT_PRIME_32;
                }
                
                constexpr static std::uint32_t offsetbasis()
                {
                    return FNV_DEFAULT_OFFSETBASIS_32;
                }
            };
            
            template<>
            struct FnvDefaultParams<64>
            {
                constexpr static std::uint64_t prime()
                {
                    return FNV_DEFAULT_PRIME_64;
                }
                
                constexpr static std::uint64_t offsetbasis()
                {
                    return FNV_DEFAULT_OFFSETBASIS_64;
                }
            };
        }
        
        template<   typename UIntT,
                    UIntT OffsetBasis = detail::FnvDefaultParams<sizeof(UIntT) * 8>::offsetbasis(),
                    UIntT FnvPrime = detail::FnvDefaultParams<sizeof(UIntT) * 8>::prime()>
        UIntT fnv1a(std::uint8_t const* begin, std::uint8_t const* end)
        {
            static_assert(std::is_unsigned<UIntT>::value, "Hash must be an unsigned integer");
            
            UIntT hash = OffsetBasis;
            for(; begin != end; ++begin)
            {
                hash ^= *begin;
                hash *= FnvPrime;
            }
            return hash;
        }
        
        template<   typename UIntT,
                    UIntT OffsetBasis = detail::FnvDefaultParams<sizeof(UIntT) * 8>::offsetbasis(),
                    UIntT FnvPrime = detail::FnvDefaultParams<sizeof(UIntT) * 8>::prime(),
                    typename CharT>
        UIntT fnv1a(std::basic_string<CharT> const& s)
        {
            auto data = (std::uint8_t const*)s.data();
            return fnv1a(data, data + s.length() * sizeof(CharT));
        }
        
        
        template<   typename UIntT,
                    UIntT OffsetBasis = detail::FnvDefaultParams<sizeof(UIntT) * 8>::offsetbasis(),
                    UIntT FnvPrime = detail::FnvDefaultParams<sizeof(UIntT) * 8>::prime(),
                    typename ValueT>
        typename std::enable_if<std::is_pod<ValueT>::value, UIntT>::type 
            fnv1a(std::vector<ValueT> const& v)
        {
            auto data = (std::uint8_t const*)v.data();
            return fnv1a(data, data + v.size() * sizeof(ValueT));
        }
        
        template<   typename UIntT,
                    UIntT OffsetBasis = detail::FnvDefaultParams<sizeof(UIntT) * 8>::offsetbasis(),
                    UIntT FnvPrime = detail::FnvDefaultParams<sizeof(UIntT) * 8>::prime()>
        UIntT fnv1a_zero(char const* begin)
        {
            static_assert(std::is_unsigned<UIntT>::value, "Hash must be an unsigned integer");
            
            UIntT hash = OffsetBasis;
            for(; *begin; ++begin)
            {
                hash ^= *begin;
                hash *= FnvPrime;
            }
            return hash;
        }
    }
}

#endif // HEADER_UUID_6242DFF369EA4DD1907BDDF0173ADB0C