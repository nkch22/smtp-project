#include "Session.hpp"

#include <print>

namespace SMTP
{

Session::Session(std::shared_ptr<asio::io_context> io_context,
                 std::shared_ptr<asio::ssl::context> ssl_context)
    : SSL::SessionBase{io_context, ssl_context}
{
}

void Session::OnConnected()
{
    std::println("Connected: {}:{}", 
        SessionBase::get_socket().remote_endpoint().address().to_string(),
        SessionBase::get_socket().remote_endpoint().port());
    Send("220 192.168.56.1 ESMTP Postfix\r\n");
}

void Session::OnDisconnected()
{
}

void Session::OnReceived(const std::string_view data)
{
    std::println("Received: {}", data);
    auto response{m_request_parser.TryParseRequest(std::data(data))};
    if(response.has_value())
    {
        response.value()->Execute();
    }
}

void Session::OnSent(const std::size_t sent)
{
}

}