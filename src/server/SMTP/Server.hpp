#pragma once

#include "Utils.hpp"

namespace SMTP
{

class Server : public ServerBase
{
public:
    Server(std::shared_ptr<asio::io_context> io_context, const Port port);
    ~Server() = default;
    std::shared_ptr<SessionBase> CreateSession(asio::ip::tcp::socket socket) override;
private:
    std::shared_ptr<asio::io_context> m_io_context;
};

}