#pragma once

#include "Response.hpp"
#include "CommandContext.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Command
        {
        public:
            virtual ~Command(){}
            virtual Response CreateResponse(const CommandContext& context) = 0;
        };
    }
}