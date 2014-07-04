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

#ifndef HEADER_UUID_29CAA09A6C624984907682F88176F541
#define HEADER_UUID_29CAA09A6C624984907682F88176F541

// C++ Standard Library:
#include <vector>
#include <cstdint>

namespace nano
{
    namespace util
    {
        // Just uses malloc/free
        struct MallocAllocator
        {
            void* alloc(std::size_t bytes) const;
            void free(void* ptr) const;
            void swap(MallocAllocator const&) const;
            
            static bool const hasNopFree = false;
        };
    
        // A allocator which allocates memory blocks and uses them like arenas.
        // Free is a non-op for this allocator, all memory is released at once.
        class ArenaAllocator
        {
        private:
            class Block
            {
            private:
                std::uint8_t* data;
                std::uint8_t* end;
                std::uint8_t* put;
            
            public:
                Block(std::size_t bytes);
                void destroy();
                void* alloc(std::size_t bytes);
                std::size_t available() const;
                std::size_t capacity() const;
            };
            
            std::vector<Block> _blocks;
            std::size_t _blockSize;
            std::size_t _growthFactor;
            
        public:
            ArenaAllocator(std::size_t blockSize = 1024, std::size_t growthFactor = 2);
            ArenaAllocator(ArenaAllocator&& other);
            ArenaAllocator& operator=(ArenaAllocator other);
            ~ArenaAllocator();
            
            void* alloc(std::size_t bytes);
            void free(void* ptr) const;
            void swap(ArenaAllocator& other);
            
            static bool const hasNopFree = true;
            
            ArenaAllocator(ArenaAllocator const&) = delete;
            ArenaAllocator& operator=(ArenaAllocator const&) = delete;
        };
    }
}

#endif // HEADER_UUID_29CAA09A6C624984907682F88176F541