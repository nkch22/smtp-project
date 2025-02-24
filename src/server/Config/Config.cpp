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
	return m_server;
}

Config::Communication Config::GetCommunication() const
{
	return m_communication;
}

Config::Logging Config::GetLogging() const
{
	return m_logging;
}

Config::Threads Config::GetThreads() const
{
	return m_threads;
}

Config::Config(const std::filesystem::path& file_path)
{
	try
	{
		// Initialize JSONParser with the configuration file path.
		ISXJson::Parser parser(file_path);
		// Parse the complete JSON configuration.
		ISXJson::JSON json = parser.Parse();

		// Retrieve the root object from the configuration.
		const ISXJson::JSON& root = json["root"];

		// Parse individual configuration sections.
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
}

void Config::ParseServer(const ISXJson::JSON& server_json)
{
	// Map JSON keys to server configuration fields.
	m_server.server_name = server_json["servername"].AsString();
	m_server.server_display_name = server_json["serverdisplayname"].AsString();
	m_server.port = static_cast<int>(server_json["listenerport"].AsNumber());
	m_server.ip = server_json["ipaddress"].AsString();
}

void Config::ParseCommunication(const ISXJson::JSON& comm_json)
{
	// Map JSON keys to communication settings.
	m_communication.blocking = (static_cast<int>(comm_json["blocking"].AsNumber()) != 0);
	m_communication.socket_timeout = static_cast<int>(comm_json["socket_timeout"].AsNumber());
}

void Config::ParseLogging(const ISXJson::JSON& logger_json)
{
	// Map JSON keys to logging configuration.
	m_logging.logs_directory = logger_json["logs_directory"].AsString();
	m_logging.log_level = static_cast<int>(logger_json["LogLevel"].AsNumber());
	m_logging.flush = (static_cast<int>(logger_json["flush"].AsNumber()) != 0);
}

void Config::ParseTime(const ISXJson::JSON& time_json)
{
	// Map JSON key to thread period configuration.
	m_threads.period_time = static_cast<int>(time_json["Period_time"].AsNumber());
}

void Config::ParseThreadpool(const ISXJson::JSON& threadpool_json)
{
	// Map JSON key to thread pool maximum working threads configuration.
	m_threads.max_working_threads = static_cast<int>(threadpool_json["maxworkingthreads"].AsNumber());
}
