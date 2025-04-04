#pragma once
#include <format>

#include "SharedInclude.h"

namespace logger_inner
{
using logger::Format;

struct Message
{
	std::string msg;
	logger::MessageTypes type;
	std::string location;
	logger::LogLevel level;
	std::string thr_id;
	Format ft;
};

struct Message_cout
{
	std::string msg;
	logger::MessageTypes type;
	std::string location;
	logger::LogLevel level;
	std::string thr_id;
	Format ft;
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
			break;
		case 'T':
			time = true;
			break;
		case 't':
			type = true;
			break;
		case 'l':
			level = true;
			break;
		case 'L':
			location = true;
			break;
		case 'm':
			text = true;
			break;
		}
		++it;

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
		if (location) formatted += obj.location;
		if (text) formatted += obj.msg;

		return std::ranges::copy(std::move(formatted), context.out()).out;
	}
};

template<>
class std::formatter<logger_inner::Message_cout>
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
			break;
		case 'T':
			time = true;
			break;
		case 't':
			type = true;
			break;
		case 'l':
			level = true;
			break;
		case 'L':
			location = true;
			break;
		case 'm':
			text = true;
			break;
		}
		++it;

		if (it != cxt.end() && *it != '}') throw std::format_error("Invalid format args.");

		return it;
	}

	auto format(const logger_inner::Message_cout& obj, auto& context) const
	{
		std::string formatted{DEFAULT_COLOR};

		if (thr_id) formatted += obj.thr_id;
		if (time) formatted += std::format("[{:%H.%M.%S-%d.%m.%y}]", std::chrono::system_clock::now());
		if (type)
		{
			switch (obj.type)
			{
			case logger::ERROR:
				formatted += ERROR_COLOR "E" DEFAULT_COLOR;
				break;
			case logger::WARNING:
				formatted += WARNING_COLOR "W" DEFAULT_COLOR;
				break;
			case logger::INFORMATION:
				formatted += INFORMATION_COLOR "I" DEFAULT_COLOR;
				break;
			}
		}
		if (level) formatted += std::to_string(obj.level.get_int());
		if (location) formatted += obj.location;
		if (text) formatted += obj.msg;

		return std::ranges::copy(std::move(formatted), context.out()).out;
	}
};