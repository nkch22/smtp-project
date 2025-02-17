#pragma once

#include <filesystem>
#include <string>

#include "../JsonParser/JsonParser.hpp"

class Config
{
public:
	struct Server
	{
		std::string server_name;
		std::string server_display_name;
		int port;
		std::string ip;
	};

	struct Communication
	{
		bool blocking;
		int socket_timeout;
	};

	struct Logging
	{
		std::string filename;
		int log_level;
		bool flush;
	};

	struct Threads
	{
		int period_time;
		int max_working_threads;
	};

	Server GetServer() const;
	Communication GetCommunication() const;
	Logging GetLogging() const;
	Threads GetThreads() const;

public:
	Config(const std::filesystem::path& file_path);

private:
	void ParseServer(const JSON& server_json);
	void ParseCommunication(const JSON& comm_json);
	void ParseLogging(const JSON& logger_json);
	void ParseTime(const JSON& time_json);
	void ParseThreadpool(const JSON& threadpool_json);

private:
	Server m_server;
	Communication m_communication;
	Logging m_logging;
	Threads m_threads;
};
