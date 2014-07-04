#ifndef HEADER_UUID_4299409B858B43D4B14C51D8562E347A
#define HEADER_UUID_4299409B858B43D4B14C51D8562E347A

// Nano:
#include <Nano/Object/Object.hpp>

namespace nano
{
    namespace object
    {
        class NativeFunctionClass : public Class
        {
        public:
            NativeFunctionClass(GlobalContext* globalContext, ClassId id);
            
            virtual std::string prettyString(Object const& o) const override;
            
            Object new_(std::string name, std::function<Object (Object&, Object*, std::size_t)> function) const;
            virtual void copyData(Object const& obj, Object& target) const;
            virtual void delete_(Object& obj) const;
            
            virtual Object call(Object& obj, Object* args, std::size_t argCount) const override;
        };
        
        bool isNativeFunction(Object const& obj);
    }
}

#endif // HEADER_UUID_4299409B858B43D4B14C51D8562E347A
