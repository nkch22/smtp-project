#pragma once
#include <format>

#include "SharedInclude.h"

namespace logger_inner
{

struct Message
{
	std::string msg;
	logger::MessageTypes type;
	std::source_location location;
	logger::LogLevel level;
	std::string thr_id;
};

} // namespace logger_inner

template<>
struct std::formatter<logger_inner::Message>
{
	bool thr_id{false}, time{false}, type{false}, level{false}, location{false}, text{false};

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

		if (thr_id)
			formatted = obj.thr_id;
		else if (time)
			formatted = std::format("[{:%H.%M.%S-%d.%m.%y}]", std::chrono::system_clock::now());
		else if (type)
			formatted = obj.type;
		else if (location)
			formatted = obj.location.function_name();
		else if (text)
			formatted = obj.msg;

		return std::ranges::copy(std::move(formatted), context.out()).out;
	}
};