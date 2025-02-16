#include "HeloCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        HeloCommand::HeloCommand(const std::string& domain_or_address)
            : m_domain_of_address{domain_or_address}
        {
        }

        Response HeloCommand::CreateResponse()
        {
            Response response{};
            response.data = "HELLO";
            return response;
        }
    }
}