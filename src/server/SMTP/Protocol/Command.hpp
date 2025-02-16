#pragma once

#include "Response.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Command
        {
        public:
            virtual ~Command(){}
            virtual Response CreateResponse() = 0;
        };
    }
}