#pragma once

#include <string>

namespace SMTP
{
    namespace Protocol
    {
        class CommandContext
        {
        public:
            CommandContext(const std::string server_name);
            std::string get_server_name() const;
        private:
            std::string m_server_name;
        };
    }
}
