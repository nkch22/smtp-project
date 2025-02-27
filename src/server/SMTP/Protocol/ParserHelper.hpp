#pragma once

#include <memory>
#include <optional>

#include "ICommand.hpp"
#include "ParseCommandType.hpp"
#include "../Options.hpp"

namespace SMTP
{

namespace Protocol
{

template<ParseCommandType... Types>
class ParserHelper; 

template<>
class ParserHelper<>
{
public:
    using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

    static OptionalCommand TryParse(const std::string& request, const Options& options){return std::nullopt;}
};

template<ParseCommandType T, ParseCommandType... Types>
class ParserHelper<T, Types...>
{
public:
    using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

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