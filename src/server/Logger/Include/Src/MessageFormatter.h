#pragma once
#include <format>

#include "SharedInclude.h"

namespace logger_inner
{
using logger::Format;

class Message
{
public:
	std::string msg;
	logger::MessageTypes type;
	std::source_location location;
	logger::LogLevel level;
	std::string thr_id;
	Format ft;

	Message(const Message&);
	Message(Message&&);

	Message(const std::string& = "", logger::MessageTypes = logger::INFORMATION,
			const std::source_location& = std::source_location::current(), const logger::LogLevel& = {},
			const std::string& = "", const Format& = "");

	void operator=(const Message&);
};

} // namespace logger_inner

template<>
class std::formatter<logger_inner::Message>
{
public:
	bool thr_id = false, time = false, type = false, level = false, location = false, text = false;

	constexpr auto parse(std::format_parse_context& cxt)
	{
		auto it = cxt.begin();
		if (it == cxt.end()) return it;

		switch (*it)
		{
		case 'i':
			thr_id = true;
			++it;
			break;
		case 'T':
			time = true;
			++it;
			break;
		case 't':
			type = true;
			++it;
			break;
		case 'l':
			level = true;
			++it;
			break;
		case 'L':
			location = true;
			++it;
			break;
		case 'm':
			text = true;
			++it;
			break;
		}

		if (it != cxt.end() && *it != '}') throw std::format_error("Invalid format args.");

		return it;
	}

	auto format(const logger_inner::Message& obj, auto& context) const
	{
		std::string formatted{};

		if (thr_id) formatted += obj.thr_id;
		if (time) formatted += std::format("[{:%H.%M.%S-%d.%m.%y}]", std::chrono::system_clock::now());
		if (type)
		{
			switch (obj.type)
			{
			case logger::ERROR:
				formatted += "E";
				break;
			case logger::WARNING:
				formatted += "W";
				break;
			case logger::INFORMATION:
				formatted += "I";
				break;
			}
		}
		if (level) formatted += std::to_string(obj.level.get_int());
		if (location) formatted += obj.location.function_name();
		if (text) formatted += obj.msg;

		return std::ranges::copy(std::move(formatted), context.out()).out;
	}
};