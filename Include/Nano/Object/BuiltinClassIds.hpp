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

#ifndef HEADER_UUID_8E0A3BF91AF8410DAC8B70EC8C4CDC3B
#define HEADER_UUID_8E0A3BF91AF8410DAC8B70EC8C4CDC3B

// C++ Standard Library:
#include <cstdint>

namespace nano
{
    namespace object
    {
        typedef std::uint32_t ClassId;
        
        ClassId const CLASSID_INT = 1;
        ClassId const CLASSID_FLOAT = 2;
        ClassId const CLASSID_BOOL = 3;
    }
}

#endif // HEADER_UUID_8E0A3BF91AF8410DAC8B70EC8C4CDC3B