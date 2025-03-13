#pragma once
#include "Buffer.h"

namespace logger_serialization
{
template<typename T>
struct RealLoggerSerializer;

template<>
struct RealLoggerSerializer<int>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const int&);
};

template<>
struct RealLoggerSerializer<double>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const double&);
};

template<>
struct RealLoggerSerializer<char>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const char&);
};

template<>
struct RealLoggerSerializer<char*>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const char*&);
};

template<>
struct RealLoggerSerializer<unsigned int>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const unsigned int&);
};

template<>
struct RealLoggerSerializer<bool>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const bool&);
};

template<>
struct RealLoggerSerializer<std::string>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const std::string&);
};

template<typename T>
struct LoggerSerializer
{
	static logger::Buffer& serialize(logger::Buffer& buff, const T& obj)
	{
		return RealLoggerSerializer<T>::serialize(buff, obj);
	}
};
} // namespace logger_serialization