#include "Server.hpp"

#include "Session.hpp"

namespace SMTP
{

Server::Server(std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ssl::context> ssl_context,
			   ContextGenerator context_generator, const Port port) :
	SSL::ServerBase{io_context, ssl_context, port}, m_context_generator{context_generator}
{
}

std::shared_ptr<SSL::SessionBase> Server::CreateSession()
{
	return std::make_shared<Session>(m_io_context, m_ssl_context, m_session_register, m_context_generator());
}

void Server::OnAccepted()
{
	std::printf("Accepting Connection\n");
}

void Server::OnStarted()
{
	std::printf("Server is started\n");
}

} // namespace SMTP