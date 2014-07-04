#ifndef HEADER_UUID_3ED270843E0F444FADE2BDC3F7D82FA2
#define HEADER_UUID_3ED270843E0F444FADE2BDC3F7D82FA2

// Nano:
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        class FloatClass : public Class
        {
        public:
            FloatClass(GlobalContext* globalContext, ClassId id);
            
            virtual std::string prettyString(Object const& o) const override;
            
            virtual Object new_() const override;
            virtual Object new_(Object* args, std::size_t argCount) const;
            Object new_(double floatval) const;
            
            virtual Object ladd(Object const& lhs, Object const& rhs) const override;
            virtual Object lsub(Object const& lhs, Object const& rhs) const override;
            virtual Object lmul(Object const& lhs, Object const& rhs) const override;
            virtual Object ldiv(Object const& lhs, Object const& rhs) const override;
            virtual Object pow(Object const& lhs, Object const& rhs) const override;
        };
        
        bool isFloat(Object const& obj);
    }
}

#endif // HEADER_UUID_3ED270843E0F444FADE2BDC3F7D82FA2
