#include "SessionRegister.hpp"

#include <mutex>

namespace SMTP
{

namespace SSL
{

SessionRegister::~SessionRegister()
{
	Clear();
}

void SessionRegister::RegisterSession(std::shared_ptr<ISession> session)
{
	std::unique_lock<std::shared_mutex> m_sessions_lock{get_sessions_mutex()};
	m_sessions.insert(session);
}

void SessionRegister::UnregisterSession(std::shared_ptr<ISession> session)
{
	std::unique_lock<std::shared_mutex> m_sessions_lock{get_sessions_mutex()};
	m_sessions.erase(session);
}

bool SessionRegister::Multicast(std::string_view data)
{
	std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
	for (auto& session : m_sessions)
	{
		session->Send(data);
	}
}

void SessionRegister::DisconnectAll()
{
	std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
	for (auto& session : m_sessions)
	{
		session->Disconnect();
	}
}

void SessionRegister::Clear()
{
	std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
	for (auto& session : m_sessions)
	{
		session->Disconnect();
	}
	m_sessions.clear();
}

std::shared_mutex& SessionRegister::get_sessions_mutex()
{
	return m_sessions_mutex;
}

} // namespace SSL

} // namespace SMTP