#pragma once

#include "../Response.hpp"
#include "../../SessionContext.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class ICommand
        {
        public:
            virtual ~ICommand() = default;
            virtual Response CreateResponse(const SessionContext& context) = 0;
        };
    }
}