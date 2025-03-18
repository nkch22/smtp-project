#include "Session.hpp"

Session::Session(std::shared_ptr<asio::io_context> io_context, 
                 std::shared_ptr<asio::ssl::context> ssl_context, 
                 std::shared_ptr<SMTP::SSL::SessionRegisterBase> session_register,
                 std::shared_ptr<Context> context)
    : SMTP::Session{io_context, ssl_context, session_register, context}
{
}