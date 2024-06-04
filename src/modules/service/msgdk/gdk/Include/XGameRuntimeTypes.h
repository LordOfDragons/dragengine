// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>

extern "C" 
{

#pragma warning(push)
#pragma warning(disable:4201) // anonymous unions warning

struct XColor
{
    union
    {
        struct
        {
            uint8_t A;
            uint8_t R;
            uint8_t G;
            uint8_t B;
        };
        uint32_t Value;
    };
};

struct XVersion
{
    union
    {
        struct
        {
            uint16_t major;
            uint16_t minor;
            uint16_t build;
            uint16_t revision;
        };
        uint64_t Value;
    };
};

#pragma warning(pop)

}
