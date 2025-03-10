/**
 * @file Config.cpp
 * @brief Implements the Config class to load and parse configuration settings from a JSON file.
 *
 * This file contains the implementation of the Config class, including methods to parse
 * various configuration sections from the JSON file and store them in structured configuration
 * parameters. It relies on the JSONParser to read and validate the configuration file.
 */

#include "Config.hpp"

#include <filesystem>
#include <stdexcept>
#include <string>

Config::Server Config::GetServer() const
{
	logger::Logger logger;
logger.log_func_start();
logger.log_return(m_server);

	return m_server;
}

Config::Communication Config::GetCommunication() const
{
	logger::Logger logger;
	logger.log_func_start();
	logger.log_return(m_communication);

	return m_communication;
}

Config::Logging Config::GetLogging() const
{
logger::Logger logger;
	logger.log_func_start();
	logger.log_return(m_logging);

	return m_logging;
}

Config::Threads Config::GetThreads() const
{
	logger::Logger logger;
	logger.log_func_start();
	logger.log_return(m_threads);

	return m_threads;
}

Config::Config(const std::filesystem::path& file_path)
{
	logger::Logger logger;
	logger.log_func_start();

	try
	{
		ISXJson::Parser parser(file_path);
		ISXJson::JSON json = parser.Parse();

		const ISXJson::JSON& root = json["root"];

		ParseServer(root["Server"]);
		ParseCommunication(root["communicationsettings"]);
		ParseLogging(root["logging"]);
		ParseTime(root["time"]);
		ParseThreadpool(root["threadpool"]);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(std::string("Error parsing config file: ") + e.what());
	}

	logger.log_return_nothing();
}

void Config::ParseServer(const ISXJson::JSON& server_json)
{
	logger::Logger logger;
	logger.log_func_start();

	m_server.server_name = server_json["servername"].AsString();
	m_server.server_display_name = server_json["serverdisplayname"].AsString();
	m_server.port = static_cast<int>(server_json["listenerport"].AsNumber());
	m_server.ip = server_json["ipaddress"].AsString();

	logger.log_return_nothing();
}

void Config::ParseCommunication(const ISXJson::JSON& comm_json)
{
	logger::Logger logger;
	logger.log_func_start();

	m_communication.blocking = (static_cast<int>(comm_json["blocking"].AsNumber()) != 0);
	m_communication.socket_timeout = static_cast<int>(comm_json["socket_timeout"].AsNumber());

	logger.log_return_nothing();
}

void Config::ParseLogging(const ISXJson::JSON& logger_json)
{
	logger::Logger logger;
	logger.log_func_start();

	m_logging.logs_directory = logger_json["logs_directory"].AsString();
	m_logging.log_level = static_cast<int>(logger_json["LogLevel"].AsNumber());
	m_logging.flush = (static_cast<int>(logger_json["flush"].AsNumber()) != 0);

	logger.log_return_nothing();
}

void Config::ParseTime(const ISXJson::JSON& time_json)
{
	logger::Logger logger;
	logger.log_func_start();

	m_threads.period_time = static_cast<int>(time_json["Period_time"].AsNumber());

	logger.log_return_nothing();
}

void Config::ParseThreadpool(const ISXJson::JSON& threadpool_json)
{
	logger::Logger logger;
	logger.log_func_start();

	m_threads.max_working_threads = static_cast<int>(threadpool_json["maxworkingthreads"].AsNumber());

	logger.log_return_nothing();
}
