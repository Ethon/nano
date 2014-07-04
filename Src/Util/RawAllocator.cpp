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

// C++ Standard Library:
#include <cstdlib>
#include <cassert>

// Nano:
#include <Nano/Util/RawAllocator.hpp>

////
// MallocAllocator implementation
////

void* nano::util::MallocAllocator::alloc(std::size_t bytes) const
{
    void* data = std::malloc(bytes);
    if(!data)
        throw std::bad_alloc();
    return data;
}
    
void nano::util::MallocAllocator::free(void* ptr) const
{
    std::free(ptr);
}
    
void nano::util::MallocAllocator::swap(MallocAllocator const&) const
{ }

////
// ArenaAllocator::Block implementation
////
    
nano::util::ArenaAllocator::Block::Block(std::size_t bytes)
    : data((std::uint8_t*)std::malloc(bytes)), end(data + bytes), put(data)
{
    if(!data)
        throw std::bad_alloc();
}
                
void nano::util::ArenaAllocator::Block::destroy()
{
    std::free(data);
}
                
void* nano::util::ArenaAllocator::Block::alloc(std::size_t bytes)
{
    if(available() >= bytes)
    {
        void* result = put;
        put += bytes;
        return result;
    }
    return nullptr;
}
                
std::size_t nano::util::ArenaAllocator::Block::available() const
{
    return end - put;
}
                
std::size_t nano::util::ArenaAllocator::Block::capacity() const
{
    return end - data;
}

////
// ArenaAllocator implementation
////       

nano::util::ArenaAllocator::ArenaAllocator(std::size_t blockSize, std::size_t growthFactor)
    : _blocks(), _blockSize(blockSize), _growthFactor(growthFactor)
{
    _blocks.emplace_back(blockSize);
}
            
nano::util::ArenaAllocator::ArenaAllocator(ArenaAllocator&& other)
    : _blocks(std::move(other._blocks)),
      _blockSize(other._blockSize), _growthFactor(other._growthFactor)
{ }
            
nano::util::ArenaAllocator& nano::util::ArenaAllocator::operator=(ArenaAllocator other)
{
    swap(other);
    return *this;
}
            
nano::util::ArenaAllocator::~ArenaAllocator()
{
    for(Block& cur : _blocks)
        cur.destroy();
}
            
void* nano::util::ArenaAllocator::alloc(std::size_t bytes)
{
    assert(_blocks.size() > 0 && "Use of moved arena !");
    
    void* result = _blocks.back().alloc(bytes);
    if(!result)
    {
        do
        {
            _blockSize *= _growthFactor;
        }
        while(_blockSize < bytes);
        
        _blocks.emplace_back(_blockSize);
        return _blocks.back().alloc(bytes);
    }
    return result;
}
            
void nano::util::ArenaAllocator::free(void*) const
{ }
            
void nano::util::ArenaAllocator::swap(ArenaAllocator& other)
{
    _blocks.swap(other._blocks);
    std::swap(_blockSize, other._blockSize);
    std::swap(_growthFactor, other._growthFactor);
}
