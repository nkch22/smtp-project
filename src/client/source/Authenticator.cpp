#include "Authenticator.hpp"

#include "ISocket.h"
#include "base64.h"

namespace SMTP
{
bool AuthPlain::Authenticate(ISocket& socket, const std::string& username, const std::string& password)
{
	std::string auth = "\0" + username + "\0" + password;
	std::string encoded_auth = base64_encode(auth);

	socket.Send("AUTH PLAIN " + encoded_auth + "\r\n");

	AssertCode(socket.Receive(), AUTH_SUCCESSFUL);

	return true;
}

bool AuthLogin::Authenticate(ISocket& socket, const std::string& username, const std::string& password)
{
	socket.Send("AUTH LOGIN\r\n");
	AssertCode(socket.Receive(), ACCEPTED);

	// Send Base64 encoded username
	socket.Send(base64_encode(username) + "\r\n");
	AssertCode(socket.Receive(), ACCEPTED);

	// Send Base64 encoded password
	socket.Send(base64_encode(password) + "\r\n");
	AssertCode(socket.Receive(), AUTH_SUCCESSFUL);

	return true;
}
} // namespace SMTP