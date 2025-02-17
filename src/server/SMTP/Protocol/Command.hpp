#pragma once

#include "Response.hpp"
#include "ServerContext.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Command
        {
        public:
            virtual ~Command(){}
            virtual Response CreateResponse(const ServerContext& context) = 0;
        };
    }
}