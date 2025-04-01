#pragma once
#include <unordered_map>

#include "SharedInclude.h"

#define DEFAULT_FORMAT "[{:i}] [{:T}] {:t} [{:l}] [{:L}] [{:m}]"

namespace logger
{
using Format = std::string;

class LogLevel
{
private:
	int m_level;
	Format m_format;
public:
	LogLevel(const int numb = logger_inner::GlobalLogLevel::get_size(), const Format& ft = DEFAULT_FORMAT);
	~LogLevel() = default;


};
}


namespace logger_inner
{
using logger::Format;

class GlobalLogLevel
{
private:
	using inner_map = std::unordered_map<std::string, std::pair<int, Format>>;

	static inner_map m_levels_map;

public:
	static void add(const std::string&, const Format& ft = DEFAULT_FORMAT);

	static int get_size();

	static int get_int(const std::string&);
	static const std::string& get_str(const std::string&);
};

} // namespace logger_inner

