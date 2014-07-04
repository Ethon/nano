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

#ifndef HEADER_UUID_4D158924626647EF8FFBED8D284A0654
#define HEADER_UUID_4D158924626647EF8FFBED8D284A0654

// C++ Standard Library:
#include <memory>
#include <limits>
#include <cstddef>
#include <cstring>
#include <cassert>

// Nano:
#include <Nano/Util/Hash.hpp>
#include <Nano/Util/RawAllocator.hpp>

namespace nano
{
    namespace util
    {
        namespace detail
        {
            std::uint32_t const STRINGHASHTABLE_PRIMES[] =
            {
                13, 29, 59, 121, 251, 503, 1009, 2027, 4057, 8117, 16249, 32503,
                65011, 130027, 260081, 520193, 1040387, 2080777, 4161557, 8323151,
                16646317, 33292687, 66585377, 133170769, 266341583, 532683227,
                1065366479, 2130732959, 4261465919
            };
            float const STRINGHASHTABLE_DEFAULT_MAX_LOADFACTOR = 1;
        }
        
        template<typename CharT, typename ValueT, class Allocator = MallocAllocator>
        class StringHashtable
        { 
        public:
            typedef CharT CharType;
            typedef ValueT ValueType;
            typedef std::basic_string<CharT> StringType;
            
        private:
            struct Entry
            {
                std::size_t keyLength;
                std::size_t hash;
                Entry* next;
                typename std::aligned_storage<sizeof(ValueT), alignof(ValueT)>::type value;
                CharT key[1];
                
                ValueT* valuePtr()
                {
                    return (ValueT*)&value;
                }
            };
            
        public:
            template<typename HashtableT>
            class BasicIterator
            {
            private:
                HashtableT* _ht;
                std::size_t _bucket;
                Entry* _last;
                Entry* _cur;
                
            public:
                BasicIterator(HashtableT* ht, std::size_t bucket, Entry* last, Entry* cur)
                    : _ht(ht), _bucket(bucket), _last(last), _cur(cur)
                { }
                
                typename HashtableT::CharType const* key() const
                {
                    assert(_bucket < _ht->bucketCount());
                    return _cur->key;
                }
                
                std::size_t keyLength() const
                {
                    assert(_bucket < _ht->bucketCount());
                    return _cur->keyLength;
                }
                
                typename HashtableT::ValueType& value() const
                {
                    assert(_bucket < _ht->bucketCount());
                    return *_cur->valuePtr();
                }
                
                typename HashtableT::ValueType& operator*() const
                {
                    return value();
                }
                
                typename HashtableT::ValueType* operator->() const
                {
                    return &value();
                }
            };
            
            typedef BasicIterator<StringHashtable<CharT, ValueT, Allocator>> Iterator;
            typedef BasicIterator<StringHashtable<CharT, ValueT const, Allocator>> ConstIterator;
            template<typename Value2T>
            using AnyIterator = BasicIterator<StringHashtable<CharT, Value2T, Allocator>>;

        private:
            unsigned _primeIndex;
            std::unique_ptr<Entry*[]> _buckets;
            size_t _size;
            Allocator _alloc;
            float _maxLoadFactor;
            
            static std::size_t hash(CharT const* key, std::size_t keyLength)
            {
                auto const data = (std::uint8_t const*)key;
                return fnv1a<std::size_t>(data, data + keyLength * sizeof(CharT));
            }

            template<typename... Args>
            Entry* makeEntry(CharT const* key, std::size_t keyLength, std::size_t hash, Args&&... args)
            {
                auto const MAXA = std::numeric_limits<size_t>::max();
                if(MAXA / sizeof(CharT) < keyLength)
                    throw std::bad_alloc();
                
                auto const keySize = keyLength * sizeof(CharT);
                auto const keyOffset = offsetof(Entry, key);
                if(MAXA - keyOffset < keySize)
                    throw std::bad_alloc();
                
                Entry* data = (Entry*)_alloc.alloc(keyOffset + keySize);
                data->hash = hash;
                data->keyLength = keyLength;
                data->next = nullptr;
                new(&data->value) ValueT(std::forward<Args>(args)...);
                std::memcpy(data->key, key, keySize);
                return data;
            }
            
            void destroyEntry(Entry* entry)
            {
                entry->valuePtr()->~ValueT();
                _alloc.free(entry);
            }
            
            void destroyEntryList(Entry* first)
            {
                Entry* cur = first;
                while(cur)
                {
                    Entry* next = cur->next;
                    destroyEntry(cur);
                    cur = next;
                };
            }
            
            std::size_t bucket(Entry* entry) const
            {
                return entry->hash % bucketCount();
            }
            
            std::size_t bucket(std::size_t hashValue) const
            {
                return hashValue % bucketCount();
            }

            Entry* findEntry(CharT const* key, size_t keyLength, size_t bucketIndex, Entry*& last)
            {
                if(!_buckets[bucketIndex])
                    return nullptr;
                
                Entry* cur = _buckets[bucketIndex];
                last = nullptr;
                do
                {
                    if(keyLength == cur->keyLength)
                    {
                        auto const cmpResult = std::memcmp(key, cur->key, keyLength * sizeof(CharT));
                        if(cmpResult == 0)
                            return cur;
                    }
                } while( (last = cur, cur = cur->next) );
                return nullptr;
            }
            
            template<typename Value2T>
            Entry* insertOrReturn(CharT const* key, std::size_t keyLength, std::size_t hash,
                Value2T&& value,  Entry*& last, bool* existed = nullptr)
            {
                size_t const bucketIndex = bucket(hash);
                Entry* entry = findEntry(key, keyLength, bucketIndex, last);
                if(entry)
                {
                    if(existed)
                        *existed = true;
                    return entry;
                }
            
                if(existed)
                    *existed = false;
                last = nullptr;
                entry = makeEntry(key, keyLength, hash, std::forward<Value2T>(value));
                insertEntry(entry, bucketIndex);
                return entry;
            }
            
            template<bool Rehash = true>
            void insertEntry(Entry* entry, std::size_t bucketIndex)
            {
                if(Rehash && loadFactor() > maxLoadFactor() && bucketCount() < maxBucketCount())
                {
                    rehash();
                    bucketIndex = bucket(entry);
                }  
                  
                if(!_buckets[bucketIndex])
                {
                    _buckets[bucketIndex] = entry;
                    _buckets[bucketIndex]->next = nullptr;
                }
                else
                {
                    Entry* old = _buckets[bucketIndex];
                    _buckets[bucketIndex] = entry;
                    _buckets[bucketIndex]->next = old;
                }
                ++_size;
            }
            
            void rehash()
            {
                StringHashtable newTable(_primeIndex + 1, _maxLoadFactor, std::move(_alloc));
                for(size_t i = 0, count = bucketCount(); i != count; ++i)
                {
                    Entry* curEntry = _buckets[i];
                    while(curEntry)
                    {
                        Entry* next = curEntry->next;
                        size_t const bucketIndex = newTable.bucket(curEntry);
                        newTable.insertEntry<false>(curEntry, bucketIndex);
                        curEntry = next;
                    }
                    _buckets[i] = nullptr;
                }
                swap(newTable);
            }

        public:
            StringHashtable(unsigned primeIndex = 0,
                    float maxLoadFactor= detail::STRINGHASHTABLE_DEFAULT_MAX_LOADFACTOR,
                    Allocator alloc = Allocator())
                : _primeIndex(primeIndex), _buckets(new Entry*[bucketCount()]), _size(0),
                  _alloc(std::move(alloc)), _maxLoadFactor(maxLoadFactor)
            {
                std::memset(_buckets.get(), 0, bucketCount() * sizeof(Entry*));
            }
            
            StringHashtable(StringHashtable const&) = delete;
            
            StringHashtable(StringHashtable&& other)
                : _primeIndex(other._primeIndex), _buckets(other._buckets), _size(other._size),
                  _alloc(std::move(other._alloc)), _maxLoadFactor(other._maxLoadFactor)  
            { }
            
            ~StringHashtable()
            {
                if(!Allocator::hasNopFree || !std::is_trivially_destructible<ValueT>::value)
                {
                    for(size_t i = 0, count = bucketCount(); i != count; ++i)
                        destroyEntryList(_buckets[i]);
                }
            }
            
            StringHashtable& operator=(StringHashtable const&) = delete;
            
            StringHashtable& operator=(StringHashtable other)
            {
                swap(other);
            }

            ////
            // Capacity
            ////
            
            bool empty() const
            {
                return size() == 0;
            }
            
            std::size_t size() const
            {
                return _size;
            }
            
            constexpr std::size_t maxSize() const
            {
                return std::numeric_limits<std::size_t>::max();
            }
      
            ////
            // Element access
            ////
            
            ValueT& operator[](StringType const& key)
            {        
                auto const hashValue = hash(key.data(), key.length());
                auto const bucketIndex = bucket(hashValue);
                Entry* last;
                Entry* entry = findEntry(key.data(), key.length(), bucketIndex, last);
                if(entry)
                    return entry->value;
                entry = makeEntry(key.data(), key.length(), hashValue, ValueT());
                insertEntry(entry, bucketIndex);
                return entry->value;
            }
            
            ValueType* get(CharT const* key, std::size_t keyLength)
            {
                Entry* last;
                Entry* entry = findEntry(key, keyLength,
                    bucket(key, keyLength), last);
                if(!entry)
                    return nullptr;
                return entry->valuePtr();
            }
            
            ValueType const* get(CharT const* key, std::size_t keyLength) const
            {
                return const_cast<StringHashtable*>(this)->get(key, keyLength);
            }
            
            ValueType* get(StringType const& key)
            {
                return get(key.data(), key.length());
            }
            
            ValueType const* get(StringType const& key) const
            {
                return get(key.data(), key.length());
            }
            
            ValueType& at(CharT const* key, std::size_t keyLength)
            {
                ValueType* entry = get(key, keyLength);
                if(!entry)
                    throw std::out_of_range("StringHashtable::at");
                return *entry;
            }
            
            ValueType const& at(CharT const* key, std::size_t keyLength) const
            {
                return const_cast<StringHashtable*>(this)->at(key, keyLength);
            }
            
            ValueType& at(StringType const& key)
            {
                return at(key.data(), key.length());
            }
            
            ValueType const& at(StringType const& key) const
            {
                return at(key.data(), key.length());
            }
            
            ////
            // Element lookup
            ////
            
            std::size_t count(CharT const* key, std::size_t keyLength) const
            {
                Entry* last;
                auto const bucketIndex = bucket(key, keyLength);
                return (const_cast<StringHashtable*>(this)->findEntry(
                    key, keyLength, bucketIndex, last) != nullptr) ? 1 : 0;
            }
            
            std::size_t count(StringType const& key) const
            {
                return count(key.data(), key.length());
            }
            
            ////
            // Modifiers
            ////
            
            template<typename Value2T>
            std::pair<Iterator,bool> insert(CharT const* key, std::size_t keyLength, Value2T&& val)
            {
                bool existed;
                Entry* last;
                auto const hashValue = hash(key, keyLength);
                auto const bucketIndex = bucket(hashValue);
                Entry* entry = insertOrReturn(key, keyLength, hashValue,
                    std::forward<Value2T>(val), last, &existed);
                return std::make_pair(Iterator(this, bucketIndex, last, entry), existed);
            }

            template<typename Value2T>
            std::pair<Iterator,bool> insert(StringType const& key, Value2T&& val)
            {
                return insert(key.data(), key.length(), std::forward<Value2T>(val));
            }
        
            std::size_t erase(CharT const* key, std::size_t keyLength)
            {
                Entry* last;
                auto const curBucket = bucket(key, keyLength);
                Entry* entry = findEntry(key, keyLength, curBucket, last);
                if(!entry)
                    return 0;
                if(last)
                {
                    last->next = entry->next;
                    destroyEntry(entry);
                }
                else
                {
                    _buckets[curBucket] = entry->next;
                    destroyEntry(entry);
                }
                return 1;
            }
        
            std::size_t erase(StringType const& key)
            {
                return erase(key.data(), key.length());
            }
            
            void clear()
            {
                for(std::size_t i = 0, count = bucketCount(); i != count; ++i)
                {
                    if(!Allocator::hasNopFree || !std::is_trivially_destructible<ValueT>::value)
                        destroyEntryList(_buckets[i]);
                    _buckets[i] = nullptr;
                }
            }

            void swap(StringHashtable& other)
            {
                std::swap(_primeIndex, other._primeIndex);
                std::swap(_buckets, other._buckets);
                std::swap(_size, other._size);
                _alloc.swap(other._alloc);
                std::swap(_maxLoadFactor, other._maxLoadFactor);
            }
            
            ////
            // Buckets
            ////
            
            std::size_t bucketCount() const
            {
                return detail::STRINGHASHTABLE_PRIMES[_primeIndex];
            }
            
            std::size_t maxBucketCount() const
            {
                return *(std::end(detail::STRINGHASHTABLE_PRIMES) - 1);
            }
            
            std::size_t bucket(CharT const* key, std::size_t keyLength) const
            {
                return hash(key, keyLength) % bucketCount();
            }
            
            std::size_t bucket(StringType const& key) const
            {
                return bucket(key.data(), key.length());
            }
            
            ////
            // Hash policy
            ////
            
            float loadFactor() const
            {
                return float(_size) / bucketCount();
            }
            
            void maxLoadFactor(float newFactor)
            {
                _maxLoadFactor = newFactor;
            }
            
            float maxLoadFactor() const
            {
                return _maxLoadFactor;
            }
        };
    }
}

namespace std
{
    template<typename C, typename V, typename A>
    void swap(nano::util::StringHashtable<C, V, A>& _1, nano::util::StringHashtable<C, V, A>& _2)
    {
        _1.swap(_2);
    }
}

#endif // HEADER_UUID_4D158924626647EF8FFBED8D284A0654