#include "MessageType.h"

using namespace logger_inner;

MessageType::MessageType(MessageType&& obj) : m_name{std::move(obj.m_name)}, m_color{std::move(obj.m_color)} {};
MessageType::MessageType(const MessageType& obj) : m_name{obj.m_name}, m_color{obj.m_color} {};

MessageType::MessageType(const std::string& name, const std::string& color) : m_name{name}, m_color{color} {};

void MessageType::set_name(const std::string& val)
{
	m_name = val;
}
const std::string& MessageType::get_name() const
{
	return m_name;
}

void MessageType::set_color(const std::string& val)
{
	m_color = val;
}
const std::string& MessageType::get_color() const
{
	return m_color;
}

void MessageType::operator=(const MessageType& obj) {
	m_name = obj.m_name;
	m_color = obj.m_color;
}

