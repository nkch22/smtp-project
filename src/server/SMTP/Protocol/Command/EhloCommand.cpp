#include "EhloCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

EhloCommand::EhloCommand(const std::string& domain_or_address)
    : m_domain_of_address{domain_or_address}
{
}

void EhloCommand::Execute()
{
}

}

}