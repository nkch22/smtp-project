#include <cstdlib>

#include "Database.hpp"
#include "Server.hpp"
#include "Session.hpp"

int main()
{
	try
	{
		auto io_context{std::make_shared<asio::io_context>()};
		auto ssl_context{std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv13_server)};
		ssl_context->set_password_callback(
			[](const std::size_t max_length, const asio::ssl::context::password_purpose& purpose) -> std::string
			{ return "hello"; });
		ssl_context->use_certificate_chain_file("../tools/certificates/cert.pem");
		ssl_context->use_private_key_file("../tools/certificates/key.pem", asio::ssl::context::file_format::pem);
		ssl_context->use_tmp_dh_file("../tools/certificates/dhparam.pem");

		auto context_generator{[]() -> std::shared_ptr<Server::Context>
							   {
								   auto options{std::make_shared<Server::Context>()};
								   options->database = std::make_shared<Database>();
								   options->domain_name = asio::ip::host_name();
								   options->max_message_size = 1024 * 10;
								   options->plain_login_allowed = false;
								   options->is_secure = true;
								   return options;
							   }};

		auto server{std::make_shared<Server>(io_context, ssl_context, context_generator, 465)};
		printf("Server started on port 465\n");
		server->Start();
		io_context->run();
		printf("Server after run started on port 465\n");
		server->Stop();
	}
	catch (const std::exception& error)
	{
		std::printf("Error: %s\n", error.what());
	}
	catch (...)
	{
		std::printf("Unknown error thrown\n");
	}
	return EXIT_SUCCESS;
}