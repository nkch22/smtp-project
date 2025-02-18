#pragma once

#include <string>

#include <asio.hpp>

#include "SessionContext.hpp"

namespace SMTP 
{
	struct Session 
	{
		Session(asio::io_service& io_service) 
			: socket{io_service}
			, read_buffer{}
			, session_context{}
		{ 
		}

		Session(asio::io_service& io_service, const std::size_t max_buffer_size) 
			: socket{io_service}
			, read_buffer{max_buffer_size}
			, session_context{}
		{ 
		}

		asio::ip::tcp::socket socket;
		asio::streambuf read_buffer;
		SessionContext session_context;
	};
}