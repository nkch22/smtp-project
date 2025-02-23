#include "Client.hpp"

#include "Command.hpp"
namespace SMTP
{

ClientImpl::~ClientImpl()
{
	if (m_socket) Quit();
}

void ClientImpl::Connect(const std::string& server, uint16_t port)
{
	if (!m_socket) throw std::runtime_error("Client not initialized");
	if (!m_authenticator) throw std::runtime_error("Authenticator not set");

	m_socket->Connect(server, port);

	SMTPResponse response = m_socket->Receive();
	AssertCode(response, ResultCode::READY);

	m_socket->Send(Command::EHLO(server));
	AssertCode(m_socket->Receive(), ResultCode::OKAY);

	m_authenticator->Authenticate(*m_socket, "username", "password");
}

void ClientImpl::SendMail(const Mail& mail)
{
	if (!m_socket) throw std::runtime_error("Client not initialized");

	if (!m_socket->IsConnected()) throw std::runtime_error("Client not connected");

	m_socket->Send(Command::MAIL_FROM(mail.from));
	AssertCode(m_socket->Receive(), ResultCode::OKAY);

	for (const auto& recipient : mail.to)
	{
		m_socket->Send(Command::RCPT_TO(recipient));
		AssertCode(m_socket->Receive(), ResultCode::OKAY);
	}

	m_socket->Send(Command::DATA());
	AssertCode(m_socket->Receive(), ResultCode::DATA);

	m_socket->Send(Command::MAIL(mail));
	AssertCode(m_socket->Receive(), ResultCode::OKAY);
}

void ClientImpl::Quit()
{
	if (!m_socket) throw std::runtime_error("Client not initialized");

	if (!m_socket->IsConnected()) return;

	m_socket->Send(Command::QUIT());
	AssertCode(m_socket->Receive(), ResultCode::GOODBYE);
	m_socket->Disconnect();
}

void ClientImpl::set_authenticator(std::unique_ptr<IAuthenticator> authenticator)
{
	m_authenticator = std::move(authenticator);
}

bool Client::Init(std::unique_ptr<IAuthenticator> authenticator /*= nullptr*/)
{
	m_socket = std::make_unique<Socket>();
	m_authenticator = authenticator ? std::move(authenticator) : std::make_unique<AuthLogin>();

	return true;
}

bool SSLClient::Init(std::unique_ptr<IAuthenticator> authenticator /*= nullptr*/)
{
	m_socket = std::make_unique<SSLSocket>();
	m_authenticator = authenticator ? std::move(authenticator) : std::make_unique<AuthLogin>();

	return true;
}

} // namespace SMTP