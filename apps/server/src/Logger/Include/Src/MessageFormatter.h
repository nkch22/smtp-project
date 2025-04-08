#pragma once
#include <format>

#include "MessageType.h"
#include "SharedInclude.h"

/*! @file MessageFormatter.h
* @brief File, that formats and declares messages
*/

namespace logger_inner
{
using logger::Format;

/*! @struct Message
*	@brief Logger message
* 
*	It is used to store everything that is needed to flush and format message
*/

struct Message
{
	std::string msg;
	logger_inner::MessageType type;
	std::string location;
	logger::LogLevel level;
	std::string thr_id;
	Format ft;
};

/*! @struct Message_cout
 *	@brief Logger message for cout
 *
 *	It is used to flush and format message for cout
 */

struct Message_cout
{
	std::string msg;
	logger_inner::MessageType type;
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
		if (time) formatted += std::format("{:%H.%M.%S-%d.%m.%y}", std::chrono::system_clock::now());
		if (type) formatted += obj.type.get_name();
		if (level) formatted += std::to_string(obj.level.get_level());
		if (location) formatted += obj.location;
		if (text) formatted += obj.msg;

		return std::ranges::copy(std::move(formatted), context.out()).out;
	}
};

template<>
class std::formatter<logger_inner::Message_cout> : public std::formatter<logger_inner::Message>
{
public:
	auto format(const logger_inner::Message_cout& obj, auto& context) const
	{
		std::string formatted{DEFAULT_COLOR};

		if (thr_id) formatted += obj.thr_id;
		if (time) formatted += std::format("{:%H.%M.%S-%d.%m.%y}", std::chrono::system_clock::now());
		if (type) formatted += obj.type.get_color() + obj.type.get_name() + DEFAULT_COLOR;
		if (level) formatted += std::to_string(obj.level.get_level());
		if (location) formatted += obj.location;
		if (text) formatted += obj.msg;

		return std::ranges::copy(std::move(formatted), context.out()).out;
	}
};