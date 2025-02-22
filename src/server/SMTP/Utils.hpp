#pragma once

#include "TCP/Session.hpp"
#include "TCP/Server.hpp"

namespace SMTP
{
    using ServerBase = TCP::Server;
    using SessionBase = TCP::Session;
}