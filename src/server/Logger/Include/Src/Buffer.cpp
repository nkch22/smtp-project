#include "Buffer.h"

// Buffer

using namespace logger;

Buffer::Buffer() : Buffer{""} {}
Buffer::Buffer(const std::string& str) : m_real_buff{new std::string{str}} {}

Buffer::~Buffer()
{
	delete m_real_buff;
}

std::string Buffer::get() const
{
	return *m_real_buff;
}

void Buffer::clear()
{
	m_real_buff->clear();
}

Buffer& Buffer::operator<<(const std::string& str)
{
	*m_real_buff += str;
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const char* str)
{
	*m_real_buff += str;
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const char str)
{
	*m_real_buff += str;
	*m_real_buff += " ";
	return *this;
}

Buffer& Buffer::operator<<(const int value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const unsigned int value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const double value)
{
	*m_real_buff += std::to_string(value);
	*m_real_buff += " ";
	return *this;
}
Buffer& Buffer::operator<<(const bool value)
{
	*m_real_buff += std::to_string((int)value);
	*m_real_buff += " ";
	return *this;
}


Buffer& RealLoggerSerializer<int>::serialize(Buffer& buff, const int& obj) {
	buff << obj;
	return buff;
}

Buffer& RealLoggerSerializer<double>::serialize(Buffer& buff, const double& obj)
{
	buff << obj;
	return buff;
}

 logger::Buffer& RealLoggerSerializer<char>::serialize(logger::Buffer& buff, const char& obj)
{
	buff << obj;
	return buff;
}


Buffer& RealLoggerSerializer<char*>::serialize(Buffer& buff, const char*& obj)
{
	buff << obj;
	return buff;
}

Buffer& RealLoggerSerializer<unsigned int>::serialize(Buffer& buff, const unsigned int& obj)
{
	buff << obj;
	return buff;
}

Buffer& RealLoggerSerializer<bool>::serialize(Buffer& buff, const bool& obj)
{
	buff << obj;
	return buff;
}

Buffer& RealLoggerSerializer<std::string>::serialize(Buffer& buff, const std::string& obj)
{
	buff << obj;
	return buff;
}