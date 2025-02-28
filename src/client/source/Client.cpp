#include "Client.hpp"

#include "Command.hpp"
#include "SSLSocket.h"
#include "Socket.h"
namespace SMTP
{

std::mutex Client::s_Mutex;
Client* Client::s_Instance;

Client::~Client()
{
	if (m_socket) Quit();
}

bool Client::Init()
{
	std::lock_guard<std::mutex> lock(s_Mutex);

	if (!s_Instance)
	{
		s_Instance = new Client(std::make_unique<Socket>(), std::make_unique<AuthLogin>());
	}

	return s_Instance != nullptr;
}

void Client::Shutdown()
{
	std::lock_guard<std::mutex> lock(s_Mutex);

	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = nullptr;
	}
}

void Client::Connect(const std::string& server, uint16_t port)
{
	if (!m_socket) throw std::runtime_error("Client not initialized");
	if (!m_authenticator) throw std::runtime_error("Authenticator not set");

	m_socket->Connect(server, port);

	SMTPResponse response = m_socket->Receive();
	AssertCode(response, ResultCode::READY);

	m_socket->Send(Command::EHLO(server));
	AssertCode(m_socket->Receive(), ResultCode::OKAY);

	m_authenticator->Authenticate(*m_socket, m_username, m_password);
}

void Client::SendMail(const Mail& mail)
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

void Client::Quit()
{
	if (!m_socket) throw std::runtime_error("Client not initialized");

	if (!m_socket->IsConnected()) return;

	m_socket->Send(Command::QUIT());
	AssertCode(m_socket->Receive(), ResultCode::GOODBYE);
	m_socket->Disconnect();
}

void Client::set_authenticator(std::unique_ptr<IAuthenticator> authenticator)
{
	m_authenticator = std::move(authenticator);
}

void Client::set_socket(std::unique_ptr<ISocket> socket)
{
	if (m_socket && m_socket->IsConnected()) Quit();

	m_socket = std::move(socket);
}

void Client::set_username(const std::string& username)
{
	m_username = username;
}

void Client::set_password(const std::string& password)
{
	m_password = password;
}

Client* Client::get_instance()
{
	return s_Instance;
}

std::string Client::get_username() const
{
	return m_username;
}

std::string Client::get_password() const
{
	return m_password;
}

} // namespace SMTP