#include "Session.hpp"

#include "Protocol/Command/MailCommand.hpp"

namespace SMTP
{

Session::Session(std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ssl::context> ssl_context,
				 std::shared_ptr<SSL::SessionRegisterBase> session_register, std::shared_ptr<Context> context) :
	SSL::SessionBase{io_context, ssl_context, session_register, context->max_message_size}, m_smtp_parser{},
	m_context{context}, m_receiving_mail{false}
{
}

void Session::OnConnected()
{
}

void Session::OnDisconnected()
{
}

void Session::OnSent(const std::size_t sent) {}

void Session::OnHandshaked()
{
	const Protocol::Response response{Protocol::ReplyCode::ServiceReady,
									  std::format("{} SMTP is ready", m_context->domain_name)};
	Send(response.CreateStringResponse());
}

void Session::OnReceived(const std::string_view data)
{
	if (m_receiving_mail)
	{
		if (data == Protocol::MailCommand::END_OF_MAIL)
		{
			m_receiving_mail = false;
			const Protocol::Response response{Protocol::ReplyCode::Ok, "Message accepted for delivery"};
			const auto string_response{response.CreateStringResponse()};
			Send(string_response);
		}
	}
	else
	{
		const auto command{m_smtp_parser.TryParseRequest(std::data(data), *m_context)};
		if (command.has_value())
		{
			const auto response{command.value()->CreateResponse(*m_context)};
			HandleResponse(response);
		}
	}
}

void Session::HandleResponse(const Protocol::Response& response)
{
	const auto string_response{response.CreateStringResponse()};
	Send(string_response);
	HandleReplyCode(response.get_reply_code());
}

void Session::HandleReplyCode(const Protocol::ReplyCode reply_code)
{
	using enum Protocol::ReplyCode;

	switch (reply_code)
	{
	case ServiceClosingTransmissionChannel:
		Disconnect();
		break;
	case StartMailInput:
		m_receiving_mail = true;
		break;
	default:
		break;
	}
}

} // namespace SMTP