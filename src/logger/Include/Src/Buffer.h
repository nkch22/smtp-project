#pragma once

#include "SharedInclude.h"
#include "BufferWrapper.h"

namespace logger
{

class Buffer
{
private:
	std::string m_real_buff;

public:
	Buffer();
	Buffer(const std::string&);

	~Buffer();
	
	std::string get() const;
	
	void clear();
	

	Buffer& operator<<(const std::string&);
	Buffer& operator<<(const char*);
	Buffer& operator<<(const char);

	Buffer& operator<<(const int);
	Buffer& operator<<(const unsigned int);
	Buffer& operator<<(const double);
	Buffer& operator<<(const bool);

	template<typename T>
	Buffer& operator<<(const T&)
	{
		temp_wrap::wrap_warning("Can't find overloaded operator", std::source_location::current());
		return *this;
	}
};

} // namespace logger
