#pragma once

#include <string>

namespace SMTP
{
    struct SessionContext
    {
        std::string hostname;
        //TODO: add server extensions fields
    };
}
