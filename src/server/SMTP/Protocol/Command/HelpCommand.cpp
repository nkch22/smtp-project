#include "HelpCommand.hpp"

#include <format>

#include "AuthCommand.hpp"
#include "DataCommand.hpp"
#include "HeloCommand.hpp"
#include "EhloCommand.hpp"
#include "MailCommand.hpp"
#include "NoopCommand.hpp"
#include "QuitCommand.hpp"
#include "RcptCommand.hpp"
#include "RsetCommand.hpp"
#include "VrfyCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response HelpCommand::CreateResponse(Context& options)
{
    std::string combine_commands{};
    combine_commands += HeloCommand::COMMAND;
    combine_commands += FormatData(EhloCommand::COMMAND);
    combine_commands += FormatData(MailCommand::COMMAND);
    combine_commands += FormatData(RcptCommand::COMMAND);
    combine_commands += FormatData(DataCommand::COMMAND);
    combine_commands += FormatData(RsetCommand::COMMAND);
    combine_commands += FormatData(VrfyCommand::COMMAND);
    combine_commands += FormatData(NoopCommand::COMMAND);
    combine_commands += FormatData(QuitCommand::COMMAND);
    combine_commands += FormatData(HelpCommand::COMMAND);
    combine_commands += FormatData(AuthCommand::COMMAND);

    const Response response{Protocol::ReplyCode::HelpResponse, combine_commands};
    return response;
}

OptionalCommand HelpCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<HelpCommand>();
    }
    return std::nullopt;
}

std::string HelpCommand::FormatData(const std::string_view data)
{
    return std::format(" {}", data);
}

}

}