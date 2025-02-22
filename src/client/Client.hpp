#pragma once

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <iostream>
#include <memory>

#include "Authenticator.hpp"
#include "Mail.hpp"
#include "SSLSocket.h"
#include "Socket.h"
namespace SMTP
{
class ClientImpl
{
public:
	ClientImpl() = default;
	virtual ~ClientImpl();

	virtual bool Init(std::unique_ptr<IAuthenticator> authenticator = nullptr) = 0;

	void Connect(const std::string& server, uint16_t port);
	void SendMail(const Mail& mail);
	void Quit();

	void set_authenticator(std::unique_ptr<IAuthenticator> authenticator);

protected:
	std::unique_ptr<ISocket> m_socket;
	std::unique_ptr<IAuthenticator> m_authenticator;
};

class Client : public ClientImpl
{
public:
	Client() = default;
	~Client() = default;

	bool Init(std::unique_ptr<IAuthenticator> authenticator = nullptr) override;
};

class SSLClient : public ClientImpl
{
public:
	SSLClient() = default;
	~SSLClient() = default;

	bool Init(std::unique_ptr<IAuthenticator> authenticator = nullptr) override;
};

} // namespace SMTP