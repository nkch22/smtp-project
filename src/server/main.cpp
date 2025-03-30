#include <cstdlib>
#include <functional>
#include <iostream>

#include "Database.hpp"
#include "Initializer/Initializer.hpp"
#include "Server.hpp"
#include "Session.hpp"

void SetupLogger();
std::shared_ptr<asio::io_context> SetupAsioIO();
std::shared_ptr<asio::ssl::context> SetupAsioSSL();
Server::ContextGenerator SetupContextGenerator();

int main()
{
	try
	{
		Initializer::Init("configs/config.json");

		auto io_context{SetupAsioIO()};
		auto ssl_context{SetupAsioSSL()};
		auto context_generator{SetupContextGenerator()};

		auto server{std::make_shared<Server>(io_context, ssl_context, context_generator, 465)};
		server->Start();
		io_context->run();
		server->Stop();
	}
	catch (const std::exception& error)
	{
		std::cout << "Error: " << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error thrown" << std::endl;
	}
	return EXIT_SUCCESS;
}

void SetupLogger()
{
	logger::Logger::init(logger::LOG_LEVEL_TRACE, DEFAULT_PATH, DEFAULT_AMOUNT, true);
	logger::Logger log;

	std::string new_path = "";
	logger::Logger::set_output_dir(new_path);
	logger::Logger::stop_config();
}

std::shared_ptr<asio::io_context> SetupAsioIO()
{
	auto io_context{std::make_shared<asio::io_context>()};
	return io_context;
}

std::shared_ptr<asio::ssl::context> SetupAsioSSL()
{
	auto ssl_context{std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv13_server)};
	ssl_context->set_password_callback(
		[](const std::size_t max_length, const asio::ssl::context::password_purpose& purpose) -> std::string
		{ return "hello"; });
	ssl_context->use_certificate_chain_file("../tools/certificates/cert.pem");
	ssl_context->use_private_key_file("../tools/certificates/key.pem", asio::ssl::context::file_format::pem);
	ssl_context->use_tmp_dh_file("../tools/certificates/dhparam.pem");

	return ssl_context;
}

Server::ContextGenerator SetupContextGenerator()
{
	auto context_generator{[]() -> std::shared_ptr<Server::Context>
						   {
							   auto options{std::make_shared<Server::Context>()};
							   options->database = std::make_shared<Database>();
							   options->domain_name = asio::ip::host_name();
							   options->max_message_size = 1024;
							   options->plain_login_allowed = false;
							   options->is_secure = true;
							   return options;
						   }};

	return context_generator;
}
