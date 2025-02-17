#pragma once

#include <string>

namespace SMTP
{
    struct SessionContext
    {
        std::string server_name;
        //TODO: add server extensions fields
    };
}
