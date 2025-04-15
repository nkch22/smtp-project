#include "SessionRegisterBase.hpp"

#include <mutex>

namespace SMTP
{

namespace SSL
{

SessionRegisterBase::~SessionRegisterBase()
{
	Clear();
}

void SessionRegisterBase::RegisterSession(std::shared_ptr<ISession> session)
{
	std::unique_lock<std::shared_mutex> m_sessions_lock{get_sessions_mutex()};
	m_sessions.insert(session);
}

void SessionRegisterBase::UnregisterSession(std::shared_ptr<ISession> session)
{
	std::unique_lock<std::shared_mutex> m_sessions_lock{get_sessions_mutex()};
	m_sessions.erase(session);
}

bool SessionRegisterBase::Multicast(const std::string_view data)
{
	if (std::size(data) == 0)
	{
		return false;
	}
	std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
	for (auto& session : m_sessions)
	{
		session->Send(data);
	}
	return true;
}

void SessionRegisterBase::DisconnectAll()
{
	std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
	for (auto& session : m_sessions)
	{
		session->Disconnect();
	}
}

void SessionRegisterBase::Clear()
{
	std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
	for (auto& session : m_sessions)
	{
		session->Disconnect();
	}
	m_sessions.clear();
}

std::shared_mutex& SessionRegisterBase::get_sessions_mutex()
{
	return m_sessions_mutex;
}

} // namespace SSL

} // namespace SMTP
