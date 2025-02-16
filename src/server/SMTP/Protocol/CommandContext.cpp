#include "CommandContext.hpp"

namespace SMTP
{
    namespace Protocol
    {
        CommandContext::CommandContext(const std::string server_name)
            : m_server_name{server_name}
        {
        }

        std::string CommandContext::get_server_name() const
        {
            return m_server_name;
        }
    }    
}