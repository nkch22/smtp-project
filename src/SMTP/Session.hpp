#pragma once

#include "Protocol/Parser.hpp"
#include "SSL/SessionBase.hpp"
#include "Context.hpp"

namespace SMTP
{

class Session : public SSL::SessionBase
{
public:
    Session(std::shared_ptr<asio::io_context> io_context, 
            std::shared_ptr<asio::ssl::context> ssl_context, 
            std::shared_ptr<SSL::SessionRegisterBase> session_register,
            std::shared_ptr<Context> context);
    ~Session() = default;
protected:
    void OnHandshaked() override;
    void OnReceived(const std::string_view data) override;
    virtual void HandleResponse(const Protocol::Response& response);
private:    
    void HandleReplyCode(const Protocol::ReplyCode reply_code);
    bool UpdateMessages(const std::string_view data);

    bool m_receiving_mail;
    Protocol::Parser m_smtp_parser;
    std::shared_ptr<Context> m_context;
};

}