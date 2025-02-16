#pragma once

#include <string>

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

	struct Logger
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
	Logger GetLogger() const;
	Threads GetThreads() const;

private:
	Server m_server;
	Communication m_communication;
	Logger m_logger;
	Threads m_threads;
};
