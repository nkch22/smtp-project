#include "Server.hpp"

#include "Session.hpp"

namespace SMTP
{

Server::Server(std::shared_ptr<asio::io_context> io_context, const Port port)
    : m_io_context{io_context}
    , ServerBase{io_context, port}
{
}

std::shared_ptr<SessionBase> Server::CreateSession(asio::ip::tcp::socket socket)
{
    return std::make_shared<Session>(m_io_context, std::move(socket));
}

}