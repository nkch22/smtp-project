#pragma once

#include "Response.hpp"
#include "../SessionContext.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Command
        {
        public:
            virtual ~Command(){}
            virtual Response CreateResponse(const SessionContext& context) = 0;
        };
    }
}