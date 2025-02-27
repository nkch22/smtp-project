#pragma once

#include <type_traits>

namespace SMTP
{

namespace Protocol
{

template<typename T>
concept ParseCommandType = requires(T type)
{
    requires(true);
};

}

}