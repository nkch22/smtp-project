#pragma once
/**
 * @file ParseCommandType.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <type_traits>
#include <memory>

#include "../Context.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Concept that implements 
 * 
 * @tparam T 
 */
template<typename T>
concept ParseCommandType = requires(T type)
{
    /**
     * @brief 
     * 
     */
    {T::TryParseCommand(std::declval<std::string>(), std::declval<Context>())} -> std::same_as<OptionalCommand>;
};

}

}