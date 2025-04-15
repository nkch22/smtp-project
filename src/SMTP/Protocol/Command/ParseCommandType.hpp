#pragma once

#include <type_traits>
#include <memory>

#include "../../Context.hpp"

namespace SMTP
{

namespace Protocol
{

template<typename T>
concept ParseCommandType = requires(T type)
{
    {T::TryParseCommand(
        std::declval<std::string>(), std::declval<Context>()
    )} -> std::same_as<OptionalCommand>;
};

}

}