#include "Config.hpp"

#include <filesystem>
#include <stdexcept>
#include <string>

#include "../JsonParser/JsonParser.hpp"

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
		JSONParser parser(file_path);
		JSON json = parser.Parse();

		const JSON& root = json["root"];

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

void Config::ParseServer(const JSON& server_json)
{
	m_server.server_name = server_json["servername"].AsString();
	m_server.server_display_name = server_json["serverdisplayname"].AsString();
	m_server.port = static_cast<int>(server_json["listenerport"].AsNumber());
	m_server.ip = server_json["ipaddress"].AsString();
}

void Config::ParseCommunication(const JSON& comm_json)
{
	m_communication.blocking = (static_cast<int>(comm_json["blocking"].AsNumber()) != 0);
	m_communication.socket_timeout = static_cast<int>(comm_json["socket_timeout"].AsNumber());
}

void Config::ParseLogging(const JSON& logger_json)
{
	m_logging.filename = logger_json["filename"].AsString();
	m_logging.log_level = static_cast<int>(logger_json["LogLevel"].AsNumber());
	m_logging.flush = (static_cast<int>(logger_json["flush"].AsNumber()) != 0);
}

void Config::ParseTime(const JSON& time_json)
{
	m_threads.period_time = static_cast<int>(time_json["Period_time"].AsNumber());
}

void Config::ParseThreadpool(const JSON& threadpool_json)
{
	m_threads.max_working_threads = static_cast<int>(threadpool_json["maxworkingthreads"].AsNumber());
}
