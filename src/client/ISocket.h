#pragma once
#include <functional>
#include <string>

#include "SMTPResponse.h"

namespace SMTP
{

using ReceiveDelegate = std::function<void(const SMTPResponse&)>;

// TODO : If using async inherit enable_shared_from_this
class ISocket
{
public:
	virtual ~ISocket() = default;

	virtual void Connect(const std::string& server, uint16_t port) = 0;
	virtual void Disconnect() = 0;

	virtual void Send(const std::string& message) = 0;

	void AsyncSend(const std::string& message) { AsyncSendInternal(std::make_shared<std::string>(message)); };
	void AsyncSend(std::string&& message) { AsyncSendInternal(std::make_shared<std::string>(std::move(message))); };

	virtual SMTPResponse Receive() = 0;
	virtual void AsyncReceive(ReceiveDelegate receiveDelegate) = 0;

	bool IsConnected() { return m_isConnected; }

protected:
	virtual void AsyncSendInternal(std::shared_ptr<std::string> buffer) = 0;

protected:
	bool m_isConnected = false;
};
} // namespace SMTP
