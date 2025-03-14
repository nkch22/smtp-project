#include "Session.hpp"

namespace SMTP
{

Session::Session(std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ssl::context> ssl_context,
				 std::shared_ptr<SSL::SessionRegister> session_register,
				 std::shared_ptr<Protocol::Parser> smtp_parser) :
	SSL::SessionBase{io_context, ssl_context, session_register}, m_smtp_parser{smtp_parser}
{
}

void Session::OnConnected()
{
	std::printf("Connected: %s:%hu\n", SessionBase::get_socket().remote_endpoint().address().to_string().c_str(),
				SessionBase::get_socket().remote_endpoint().port());
}

void Session::OnDisconnected()
{
	std::printf("Disconnected\n");
}

void Session::OnReceived(const std::string_view data)
{
	std::printf("Received: %s\n", data.data());
	const auto command{m_smtp_parser->TryParseRequest(std::data(data))};
	if (command.has_value())
	{
		const auto response{command.value()->CreateResponse(m_smtp_parser->get_global_options())};
		const auto string_response{response.CreateStringResponse()};
		Send(string_response);
		if (response.get_reply_code() == Protocol::ReplyCode::ServiceClosingTransmissionChannel)
		{
			Disconnect();
		}
	}
}

void Session::OnSent(const std::size_t sent) {}

void Session::OnHandshaked()
{
	std::printf("Handshaked is successfull %s:%hu",
				SessionBase::get_socket().remote_endpoint().address().to_string().c_str(),
				SessionBase::get_socket().remote_endpoint().port());
	const Protocol::Response response{Protocol::ReplyCode::ServiceReady,
									  std::format("{} SMTP is ready", m_smtp_parser->get_global_options().domain_name)};
	Send(response.CreateStringResponse());
}

} // namespace SMTP