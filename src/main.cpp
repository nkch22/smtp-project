#include <cstdlib>
#include <functional>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

#include "Server.hpp"
#include "Database.hpp"

#include "Config/Settings.hpp"
#include "Config/Context.hpp"
#include "Config/Database.hpp"
#include "Config/SSL.hpp"
#include "Config/Server.hpp"

#include "Logger/Logger.hpp"

std::shared_ptr<asio::io_context> SetupAsioIO(const Config::Settings& settings);
std::shared_ptr<asio::ssl::context> SetupAsioSSL(
	const Config::Settings& settings, 
	const std::filesystem::path& certificates_folder
);
std::shared_ptr<Database> SetupDatabase(const Config::Settings& settings);
Server::ContextGenerator SetupSessionContextGenerator(
	const Config::Settings& settings, 
	std::shared_ptr<Database> database
);
std::shared_ptr<Server> SetupServer(
	const Config::Settings& settings, 
	std::shared_ptr<asio::io_context> io_context,
	std::shared_ptr<asio::ssl::context> ssl_context,
	Server::ContextGenerator session_context_generator
);

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		Logger::error("Provide directory which contains .json configs and directory with .pem certificates\n");
		return EXIT_FAILURE;
	}

	Config::Settings settings{argv[1]};

	auto io_context{SetupAsioIO(settings)};
	auto ssl_context{SetupAsioSSL(settings, argv[2])};
	auto database{SetupDatabase(settings)};
	auto session_context_generator{SetupSessionContextGenerator(
		settings, 
		database
	)};
	auto server{SetupServer(
		settings,
		io_context,
		ssl_context,
		session_context_generator
	)};

	server->Start();
	io_context->run();
	server->Stop();

	return EXIT_SUCCESS;
}

std::shared_ptr<asio::io_context> SetupAsioIO(const Config::Settings& settings)
{
	auto io_context{std::make_shared<asio::io_context>()};
	return io_context;
}

std::shared_ptr<asio::ssl::context> SetupAsioSSL(
	const Config::Settings& settings, 
	const std::filesystem::path& certificates_directory
)
{
	if(!std::filesystem::is_directory(certificates_directory))
    {
        throw std::invalid_argument{"SetupAsioSSL function expects path for directory which contains certificates!"};
    }
	const auto ssl_result{settings.Get<Config::SSL>()};
	if(!ssl_result.has_value())
	{
		throw std::runtime_error{"Failed to get config for SSL class!"};
	}
	auto ssl_context{std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv13_server)};
	const auto ssl_settings{ssl_result.value()};
	ssl_context->set_password_callback(
		[password = ssl_settings.password_callback](
			const std::size_t max_length, 
			const asio::ssl::context::password_purpose& purpose
		) -> std::string
		{
			return password; 
		});
	ssl_context->use_certificate_chain_file(certificates_directory / ssl_settings.certificate_chain_file);
	ssl_context->use_private_key_file(
		certificates_directory / ssl_settings.private_key_file, 
		asio::ssl::context::file_format::pem
	);
	ssl_context->use_tmp_dh_file(certificates_directory / ssl_settings.tmp_dh_param);
	return ssl_context;
}

std::shared_ptr<Database> SetupDatabase(const Config::Settings& settings)
{
	using namespace std::chrono_literals;

	const auto database_result{settings.Get<Config::Database>()};
	if(!database_result.has_value())
	{
		throw std::runtime_error{"Failed to get config for Database class!"};
	}
	const auto database_settings{database_result.value()};
	std::this_thread::sleep_for(database_settings.wait_time);
	auto database{std::make_shared<Database>(database_settings.connection_string)};
	return database;
}

Server::ContextGenerator SetupSessionContextGenerator(const Config::Settings& settings, std::shared_ptr<Database> database)
{
	const auto server_settings_result{settings.Get<Config::Server>()};
	if(!server_settings_result.has_value())
	{
		throw std::runtime_error{"Failed to get config for Server class!"};
	}
	const auto server_settings{server_settings_result.value()};
	const auto session_context_result{settings.Get<Config::Context>()};
	if(!session_context_result.has_value())
	{
		throw std::runtime_error{"Failed to get config for SessionContext class!"};
	}
	const auto session_context_settings{session_context_result.value()};
	auto context_generator{[database, server_settings, session_context_settings]() -> std::shared_ptr<Server::Context>
						   {
							   auto options{std::make_shared<Server::Context>()};
							   options->database = database;
							   options->domain_name = server_settings.domain;
							   options->max_message_size = session_context_settings.max_message_size;
							   options->plain_login_allowed = session_context_settings.plain_login_allowed;
							   options->is_secure = session_context_settings.is_secure;
							   return options;
						   }};

	return context_generator;
}

std::shared_ptr<Server> SetupServer(
	const Config::Settings& settings, 
	std::shared_ptr<asio::io_context> io_context,
	std::shared_ptr<asio::ssl::context> ssl_context,
	Server::ContextGenerator session_context_generator
)
{
	const auto server_settings_result{settings.Get<Config::Server>()};
	if(!server_settings_result.has_value())
	{
		throw std::runtime_error{"Failed to receive config for Server class!"};
	}
	const auto server_settings{server_settings_result.value()};
	auto server{std::make_shared<Server>(
		io_context, 
		ssl_context, 
		session_context_generator, 
		server_settings.port
	)};
	return server;
}