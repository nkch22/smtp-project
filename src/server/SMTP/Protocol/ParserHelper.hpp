#pragma once
/**
 * @file ParserHelper.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <memory>
#include <optional>

#include "ICommand.hpp"
#include "ParseCommandType.hpp"
#include "../Options.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class declaration for ParserHelper
 * 
 * @tparam Types 
 */
template<ParseCommandType... Types>
class ParserHelper; 

/**
 * @brief Class ParserHelper definion for 0 types
 * 
 * @tparam  
 */
template<>
class ParserHelper<>
{
public:
    using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

    /**
     * @brief 
     * 
     * @param request 
     * @param options 
     * @return OptionalCommand 
     */
    static OptionalCommand TryParse(const std::string& request, const Options& options){return std::nullopt;}
};

/**
 * @brief Class ParserHepler definition with multiple parameters
 * 
 * @tparam T 
 * @tparam Types 
 */
template<ParseCommandType T, ParseCommandType... Types>
class ParserHelper<T, Types...>
{
public:
    using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

/**
 * @brief Function that parses request and returns implemented command
 * 
 * @param request 
 * @param options 
 * @return OptionalCommand 
 */
    static OptionalCommand TryParse(const std::string& request, const Options& options);
};

template<ParseCommandType T, ParseCommandType... Types>
ParserHelper<T, Types...>::OptionalCommand ParserHelper<T, Types...>::TryParse(const std::string& request, const Options& options)
{
    if(auto command{T::TryParseCommand(request, options)};
       command.has_value())
    {
        return command;
    }
    else
    {
        return ParserHelper<Types...>::TryParse(request, options);
    }
}

}

}