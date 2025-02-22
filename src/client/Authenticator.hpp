#pragma once
#include <string>

#include "ISocket.h"

namespace SMTP
{
class IAuthenticator
{
public:
	IAuthenticator() = default;
	virtual ~IAuthenticator() = default;

	// Perform authentication
	virtual bool Authenticate(ISocket& socket, const std::string& username, const std::string& password) = 0;
};

class AuthPlain : public IAuthenticator
{
public:
	AuthPlain() = default;
	virtual ~AuthPlain() = default;

	bool Authenticate(ISocket& socket, const std::string& username, const std::string& password) override;
};

class AuthLogin : public IAuthenticator
{
public:
	AuthLogin() = default;
	virtual ~AuthLogin() = default;

	bool Authenticate(ISocket& socket, const std::string& username, const std::string& password) override;
};

} // namespace SMTP
