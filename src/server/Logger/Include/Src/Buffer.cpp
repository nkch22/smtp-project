#include "Buffer.h"

// Buffer

using namespace logger;

Buffer::Buffer() : Buffer{""} {}
Buffer::Buffer(const std::string& str) : m_real_buff{str} {}

Buffer::~Buffer() {}

std::string Buffer::get() const
{
	return m_real_buff;
}

void Buffer::clear()
{
	m_real_buff.clear();
}

Buffer& Buffer::operator<<(const std::string& str)
{
	m_real_buff += str;
	return *this;
}
Buffer& Buffer::operator<<(const char* str)
{
	m_real_buff += str;
	return *this;
}
Buffer& Buffer::operator<<(const char str)
{
	m_real_buff += str;
	return *this;
}

Buffer& Buffer::operator<<(const int value)
{
	m_real_buff += std::to_string(value);
	return *this;
}
Buffer& Buffer::operator<<(const unsigned int value)
{
	m_real_buff += std::to_string(value);
	return *this;
}
Buffer& Buffer::operator<<(const double value)
{
	m_real_buff += std::to_string(value);
	return *this;
}
Buffer& Buffer::operator<<(const bool value)
{
	m_real_buff += std::to_string((int)value);
	return *this;
}
