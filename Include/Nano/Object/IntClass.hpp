#ifndef HEADER_UUID_A26AE361182A4BA08666FFE820766A7C
#define HEADER_UUID_A26AE361182A4BA08666FFE820766A7C

// Nano:
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        class IntClass : public Class
        {
        public:
            IntClass(GlobalContext* globalContext, ClassId id);
            
            virtual std::string prettyString(Object const& o) const override;
            
            virtual Object new_() const override;
            virtual Object new_(Object* args, std::size_t argCount) const;
            Object new_(std::int64_t intval) const;
            
            virtual Object ladd(Object const& lhs, Object const& rhs) const override;
            virtual Object lsub(Object const& lhs, Object const& rhs) const override;
            virtual Object lmul(Object const& lhs, Object const& rhs) const override;
            virtual Object ldiv(Object const& lhs, Object const& rhs) const override;
            virtual Object pow(Object const& lhs, Object const& rhs) const override;
        };
        
        bool isInt(Object const& obj);
    }
}

#endif // HEADER_UUID_A26AE361182A4BA08666FFE820766A7C
