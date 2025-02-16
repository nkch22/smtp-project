#include "Config.hpp"

Config::Server Config::GetServer() const
{
	return m_server;
}

Config::Communication Config::GetCommunication() const
{
	return m_communication;
}

Config::Logger Config::GetLogger() const
{
	return m_logger;
}

Config::Threads Config::GetThreads() const
{
	return m_threads;
}
