#include <asio.hpp>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "base64.h"
class SMTPServer
{
public:
	SMTPServer(asio::io_context& io_context, uint16_t port) :
		m_acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
	{
		StartAccept();
	}

private:
	asio::ip::tcp::acceptor m_acceptor;

	void StartAccept()
	{
		auto socket = std::make_shared<asio::ip::tcp::socket>(m_acceptor.get_executor());
		m_acceptor.async_accept(*socket,
								[this, socket](const asio::error_code& error)
								{
									if (!error)
									{
										std::cout << "Client connected" << std::endl;
										HandleClient(socket);
									}
									StartAccept();
								});
	}

	void HandleClient(std::shared_ptr<asio::ip::tcp::socket> socket)
	{
		Send(socket, "220 SMTP Server Ready\r\n");
		ReadCommand(socket);
	}

	void ReadCommand(std::shared_ptr<asio::ip::tcp::socket> socket)
	{
		auto buffer = std::make_shared<std::vector<char>>(1024);
		socket->async_read_some(asio::buffer(*buffer),
								[this, socket, buffer](const asio::error_code& error, std::size_t length)
								{
									if (!error)
									{
										std::string command(buffer->begin(), buffer->begin() + length);
										ProcessCommand(socket, command);
										ReadCommand(socket);
									}
									else
									{
										std::cout << "Client disconnected" << std::endl;
									}
								});
	}

	void ProcessCommand(std::shared_ptr<asio::ip::tcp::socket> socket, const std::string& command)
	{
		std::istringstream stream(command);
		std::string keyword;
		stream >> keyword;

		if (command.find("\r\n.\r\n") != std::string::npos)
		{
			std::cout << command;
			Send(socket, "250 OK\r\n");
			return;
		}

		if (keyword == "HELO" || keyword == "EHLO")
		{
			Send(socket, "250 Hello\r\n");
		}
		else if (keyword == "MAIL")
		{
			Send(socket, "250 OK\r\n");
		}
		else if (keyword == "RCPT")
		{
			Send(socket, "250 OK\r\n");
		}
		else if (keyword == "DATA")
		{
			Send(socket, "354 Start mail input; end with <CRLF>.<CRLF>\r\n");
		}
		else if (keyword == "QUIT")
		{
			Send(socket, "221 Bye\r\n");
			socket->close();
		}
		else if (keyword == "STARTTLS")
		{
			Send(socket, "454 TLS not available\r\n");
		}
		else if (keyword == "AUTH")
		{
			std::string auth_command;
			stream >> auth_command;

			if (auth_command == "PLAIN")
			{
				// Handle AUTH PLAIN
				HandleAuthPlain(socket, stream);
			}
			else if (auth_command == "LOGIN")
			{
				// Handle AUTH LOGIN
				HandleAuthLogin(socket);
			}
			else
			{
				Send(socket, "501 Syntax error in parameters or arguments\r\n");
			}
		}
		else
		{
			Send(socket, "500 Command not recognized\r\n");
		}

		std::cout << command;
	}

	void HandleAuthPlain(std::shared_ptr<asio::ip::tcp::socket> socket, std::istringstream& stream)
	{
		// Receive the base64 encoded string for AUTH PLAIN
		std::string encoded_auth;
		stream >> encoded_auth;

		// Decode the base64 string
		std::string decoded_auth = base64_decode(encoded_auth);

		// Check if decoding was successful and split the string by '\0' for username and password
		size_t first_null = decoded_auth.find('\0');
		size_t second_null = decoded_auth.find('\0', first_null + 1);

		if (first_null != std::string::npos && second_null != std::string::npos)
		{
			std::string username = decoded_auth.substr(first_null + 1, second_null - first_null - 1);
			std::string password = decoded_auth.substr(second_null + 1);

			// Validate the username and password (replace with your own logic)
			if (ValidateUser(username, password))
			{
				Send(socket, "235 Authentication successful\r\n");
			}
			else
			{
				Send(socket, "PLAIN 535 Authentication failed\r\n");
			}
		}
		else
		{
			Send(socket, "NO NULL 535 Authentication failed\r\n");
		}
	}

	void HandleAuthLogin(std::shared_ptr<asio::ip::tcp::socket> socket)
	{

		// Send the "Username" prompt for base64 encoding
		Send(socket, "334 VXNlcm5hbWU6\r\n"); // Base64 of "Username:"

		std::vector<char> buffer(1024);
		int size = socket->read_some(asio::buffer(buffer));
		std::string name = std::string(buffer.begin(), buffer.begin() + size);

		std::istringstream stream(name);

		// Receive the base64 encoded username
		std::string encoded_username;
		stream >> encoded_username;

		// Decode the base64 string
		std::string username = base64_decode(encoded_username);

		// Send the "Password" prompt for base64 encoding
		Send(socket, "334 UGFzc3dvcmQ6\r\n"); // Base64 of "Password:"

		size = socket->read_some(asio::buffer(buffer));
		std::string pass = std::string(buffer.begin(), buffer.begin() + size);

		stream.clear();
		stream.str(pass);

		// Receive the base64 encoded password
		std::string encoded_password;
		stream >> encoded_password;

		// Decode the base64 string
		std::string password = base64_decode(encoded_password);

		// Validate the username and password (replace with your own logic)
		if (ValidateUser(username, password))
		{
			Send(socket, "235 Authentication successful\r\n");
		}
		else
		{
			Send(socket, "LOGIN ERROR 535 Authentication failed\r\n");
		}
	}

	bool ValidateUser(const std::string& username, const std::string& password)
	{
		// This function should validate the user credentials
		// Replace with your logic for validating the username and password

		std::cout << "Username: " << username << std::endl;
		std::cout << "Password: " << password << std::endl;

		if (username == "username" && password == "password")
		{
			return true;
		}
		return false;
	}

	void Send(std::shared_ptr<asio::ip::tcp::socket> socket, const std::string& message)
	{
		asio::async_write(*socket, asio::buffer(message),
						  [](const asio::error_code& error, std::size_t)
						  {
							  if (error)
							  {
								  std::cerr << "Error sending message: " << error.message() << std::endl;
							  }
						  });
	}
};

int main()
{
	try
	{
		asio::io_context io_context;
		SMTPServer server(io_context, 2525);
		io_context.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return 0;
}