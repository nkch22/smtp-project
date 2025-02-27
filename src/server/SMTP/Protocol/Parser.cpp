#include "Parser.hpp"

#include <algorithm>
#include <sstream>

#include "ParserHelper.hpp"
#include "HeloCommand.hpp"
#include "EhloCommand.hpp"
#include "QuitCommand.hpp"
#include "DataCommand.hpp"
#include "AuthCommand.hpp"
#include "MailFromCommand.hpp"
#include "NoopCommand.hpp"
#include "RcptToCommand.hpp"
#include "RsetCommand.hpp"
#include "VrfyCommand.hpp"
#include "HelpCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Parser::Parser(const Options global_options)
    : m_global_options{global_options}
{
}

Parser::OptionalCommand Parser::TryParseRequest(const std::string& request) const
{
    return TryParseRequest(request, m_global_options);
}

Parser::OptionalCommand Parser::TryParseRequest(const std::string& request, const Options& custom_options) const
{
    const auto upper_request{ToUpper(request)};
    ParserHelper<HeloCommand, EhloCommand, 
                 HelpCommand, QuitCommand, 
                 DataCommand, AuthCommand,
                 MailFromCommand, NoopCommand,
                 QuitCommand, RcptToCommand,
                 RsetCommand, VrfyCommand>::TryParse(request, custom_options);
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseHelo(const std::string& request, const Options& custom_options) const
{
    if(const auto position{request.find(HeloCommand::COMMAND)};
       position != std::string::npos)
    {
        auto client_name{request.substr(position + std::size(HeloCommand::COMMAND))};
        client_name = RemoveWhitespaces(client_name);
        return std::make_unique<HeloCommand>(client_name);
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseEhlo(const std::string& request, const Options& custom_options) const
{
    if(const auto position{request.find(EhloCommand::COMMAND)};
       position != std::string::npos)
    {
        auto client_name{request.substr(position + std::size(EhloCommand::COMMAND))};
        client_name = RemoveWhitespaces(client_name);
        return std::make_unique<EhloCommand>(client_name);
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseQuit(const std::string& request, const Options& custom_options) const
{
    if(request.contains(QuitCommand::COMMAND))
    {
        return std::make_unique<QuitCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseData(const std::string& request, const Options& custom_options) const
{
    if(request.contains(DataCommand::COMMAND))
    {
        return std::make_unique<DataCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseAuth(const std::string& request, const Options& custom_options) const
{
    if(request.contains(AuthCommand::COMMAND))
    {
        return std::make_unique<AuthCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseMailFrom(const std::string& request, const Options& custom_options) const
{
    if(request.contains(MailFromCommand::COMMAND))
    {
        return std::make_unique<MailFromCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseNoop(const std::string& request, const Options& custom_options) const
{
    if(request.contains(NoopCommand::COMMAND))
    {
        return std::make_unique<NoopCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseRcptTo(const std::string& request, const Options& custom_options) const
{
    if(request.contains(RcptToCommand::COMMAND))
    {
        return std::make_unique<RcptToCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseRset(const std::string& request, const Options& custom_options) const
{
    if(request.contains(RsetCommand::COMMAND))
    {
        return std::make_unique<RsetCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseVrfy(const std::string& request, const Options& custom_options) const
{
    if(request.contains(VrfyCommand::COMMAND))
    {
        return std::make_unique<VrfyCommand>();
    }
    return std::nullopt;
}

Parser::OptionalCommand Parser::TryParseHelp(const std::string& request, const Options& custom_options) const
{
    if(request.contains(HelpCommand::COMMAND))
    {
        return std::make_unique<HelpCommand>();
    }
    return std::nullopt;
}

std::string Parser::ToUpper(const std::string& string)
{
    std::string upper_copy{};
    upper_copy.resize(std::size(string));
    std::transform(std::begin(string), std::end(string), std::begin(upper_copy), ::toupper);
    return upper_copy;
}

std::string Parser::RemoveWhitespaces(const std::string& string)
{
    auto copy{string};
    copy.erase(std::remove_if(std::begin(copy), std::end(copy), ::isspace), std::end(copy));
    return copy;
}

std::vector<std::string> Parser::Split(const std::string& string)
{
    std::vector<std::string> data{};
    std::stringstream string_stream{string};
    for(std::string string{}; 
        std::getline(string_stream, string, ' ');)
    {
        data.emplace_back(string);
    }
    return data;
}
 
const Options& Parser::get_global_options() const noexcept
{
    return m_global_options;
}

void Parser::set_global_options(const Options& options)
{
    std::unique_lock<std::mutex> lock{m_global_options_mutex};
    m_global_options = options;
}

}

}