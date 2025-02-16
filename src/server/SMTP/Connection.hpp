#pragma once

#include <string>

#include <asio.hpp>

namespace SMTP 
{
	struct Connection 
	{
		Connection(asio::io_service & io_service) 
			: socket{io_service}, read_buffer{}
		{ 
		}

		Connection(asio::io_service & io_service, std::size_t max_buffer_size) 
			: socket( io_service ), read_buffer{max_buffer_size}
		{ 
		}

		asio::ip::tcp::socket socket;
		asio::streambuf read_buffer;
	};
}