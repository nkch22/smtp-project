#pragma once
#include <unordered_map>

#include "SharedInclude.h"

/*!
 *	@file LogLevels.h
 *	@brief LogLevels and Format declaration
 *
 */

namespace logger
{
using Format = std::string;

/*! @class LogLevel
*	@brief Class to store needed log level values
* 
*	It stores integer representation of log level and associated format
*/
class LogLevel
{
private:
	int m_level;
	Format m_format;
public:
	/*! @brief Copy constructor*/
	LogLevel(const LogLevel&);
	/*! @brief Move constructor*/
	LogLevel(LogLevel&&);

	/*! @brief Default constructor*/
	LogLevel(const Format& ft = DEFAULT_FORMAT, const int level = INNER_DEFAULT_LEVEL);
	/*! @brief Trivial destructor*/
	~LogLevel() = default;

	/*! @brief Associated int level setter*/
	void set_level(const int);
	/*! @brief Associated format setter*/
	void set_format(const Format&);

	/*! @brief Associated int level getter*/
	int get_level() const;
	/*! @brief Associated format getter*/
	const Format& get_format() const;

	/*! @brief Assignment operator*/
	void operator=(const LogLevel&);

	/*! @brief Equality operator*/
	const bool operator==(const LogLevel&) const;
};
}

namespace logger_inner
{
using logger::LogLevel;

/*! @class GlobalLogLevel
 *	@brief Log levels register
 *
 *	It stores every log level in map
 */

class GlobalLogLevel
{
private:
	using inner_map = std::unordered_map<int, LogLevel>;

	static inner_map m_levels_map;

public:
	/*! @brief adds new log level */
	static void add(LogLevel&);

	/*! @brief edits log level*/
	static void edit(const int, const LogLevel&);

	/*! @brief deletes log level*/
	static void erase(const int);

	/*! @brief returns map size*/
	static int get_size();

	/*! @brief Log level getter*/
	static const LogLevel& get(const int);
};

} // namespace logger_inner

