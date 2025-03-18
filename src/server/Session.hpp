#pragma once

#include "SMTP/Session.hpp"

class Session final : public SMTP::Session
{
public:
    using Context = SMTP::Context;

    Session(std::shared_ptr<asio::io_context> io_context, 
            std::shared_ptr<asio::ssl::context> ssl_context, 
            std::shared_ptr<SMTP::SSL::SessionRegisterBase> session_register,
            std::shared_ptr<Context> context);
    ~Session() = default;
};