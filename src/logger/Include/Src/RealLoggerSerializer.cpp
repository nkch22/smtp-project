#include "RealLoggerSerializer.h"

using namespace logger;
using namespace logger_serialization;

Buffer& RealLoggerSerializer<int>::serialize(Buffer& buff, const int& obj)
{
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