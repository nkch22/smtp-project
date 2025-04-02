#include "../Session.hpp"

#include <format>

Session::Session(std::shared_ptr<asio::io_context> io_context, 
                 std::shared_ptr<asio::ssl::context> ssl_context, 
                 std::shared_ptr<SMTP::SSL::SessionRegisterBase> session_register,
                 std::shared_ptr<Context> context)
    : SMTP::Session{io_context, ssl_context, session_register, context}
{
}

void Session::OnConnected()
{
    log.log_message(std::format("Connected: {}:{}", 
        SessionBase::get_socket().remote_endpoint().address().to_string(),
        SessionBase::get_socket().remote_endpoint().port()));
}

void Session::OnDisconnected()
{
    log.log_message("Session is disconnected");
}

void Session::OnSent(const std::size_t sent)
{
    log.log_message(std::format("Server sent {} bytes", sent));
}

void Session::OnHandshaked()
{
    log.log_message(std::format("Handshaked is successfull {}:{}", 
        SessionBase::get_socket().remote_endpoint().address().to_string(),
        SessionBase::get_socket().remote_endpoint().port()));
    SMTP::Session::OnHandshaked();
}

void Session::OnReceived(const std::string_view data)
{
    log.log_message(std::format("Received: {}", data));
    SMTP::Session::OnReceived(data);
}