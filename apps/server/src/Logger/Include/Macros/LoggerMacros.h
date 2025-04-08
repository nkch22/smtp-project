#pragma once
#include "SharedMacros.h"

/*! @file LoggerMacros.h
 *	There are every logger macros
 */

#define INNER_LOGGER_ACTION(value) buff << '(' << #value << ':' << obj.value << ')';
#define INNER_LOGGER_LOOP(...) FOR_EACH(INNER_LOGGER_ACTION, __VA_ARGS__)

#define LOGGER_GET_PRIVATE(Type) friend logger::Buffer& operator<<(logger::Buffer&, const Type&);

#define MAKE_LOGGABLE(Type, ...)                                                                                       \
	logger::Buffer& operator<<(logger::Buffer& buff, const Type& obj)                                                  \
	{                                                                                                                  \
		__VA_OPT__(INNER_LOGGER_LOOP(__VA_ARGS__))                                                                     \
		return buff;                                                                                                   \
	}

#define DEFAULT_AMOUNT 30
#define DEFAULT_PATH ""
#define DEFAULT_CONFIG false
#define DEFAULT_FLUSH true

#define ERROR_COLOR "\033[41m"
#define WARNING_COLOR "\033[43m"
#define INFORMATION_COLOR "\033[42m"
#define DEFAULT_COLOR "\033[0m"

#define NO_LOG_LEVEL logger_inner::GlobalLogLevel::get(0)
#define PROD_LOG_LEVEL logger_inner::GlobalLogLevel::get(1)
#define DEBUG_LOG_LEVEL logger_inner::GlobalLogLevel::get(2)
#define TRACE_LOG_LEVEL logger_inner::GlobalLogLevel::get(3)

#define DEFAULT_LOG_LEVEL PROD_LOG_LEVEL

#define INNER_DEFAULT_LEVEL -1

// "[{:i}] [{:T}] {:t} [{:l}] [{:L}] [{:m}]"
#define FORMAT_NO ""
#define FORMAT_PROD "[{:T}]{:t}| {:m}"
#define FORMAT_DEBUG "[{:T}]{:t}[{:l}][{:L}]| {:m}"
#define FORMAT_TRACE "[{:i}][{:T}]{:t}[{:l}][{:L}]| {:m}"

#define DEFAULT_FORMAT FORMAT_PROD

#define FUNCTION_NAME __FUNCTION__

#define LOGGER(NAME)                                                                                                   \
	logger::Logger NAME                                                                                                \
	{                                                                                                                  \
		FUNCTION_NAME                                                                                                  \
	}
#define LOGGER_IN_INIT_LIST                                                                                            \
	logger::Logger                                                                                                     \
	{                                                                                                                  \
		FUNCTION_NAME                                                                                                  \
	}

#define CREATE_LOG_LEVEL(name, format)                                                                                 \
	static logger::LogLevel name{format};                                                                              \
	logger_inner::GlobalLogLevel::add(LEVEL_##name)

#define GET_LOG_LEVEL(value) logger_inner::GlobalLogLevel::get(value)

#define CREATE_TYPE(name, short_name, color)                                                                           \
	static logger_inner::MessageType name                                                                              \
	{                                                                                                                  \
		short_name, color                                                                                              \
	}

/*! @def INNER_LOGGER_ACTION(value)
 *	@brief Inner serialization for other macros
 */
/*! @def INNER_LOGGER_LOOP(...)
 *	@brief Inner serialization for multiple objects
 */
/*! @def LOGGER_GET_PRIVATE(Type)
 *	@brief Defines operator<< for generation
 */
/*! @def MAKE_LOGGABLE(Type, ...)
 *	@brief Overloaded serialization operator generator
 * 
 *	Generates overloaded operator<< for given class with given members
 */

/*! @def DEFAULT_AMOUNT
 *	@brief Default logs amount
 */
/*! @def DEFAULT_PATH
 *	@brief Default output path
 */
/*! @def DEFAULT_CONFIG
 *	@brief Default config flag value
 */
/*! @def DEFAULT_FLUSH
 *	@brief Default flush flag value
 */

/*! @def DEFAULT_COLOR
 *	@brief Default console color
 */
/*! @def ERROR_COLOR
 *	@brief Error console color
 */
/*! @def WARNING_COLOR
 *	@brief Warning console color
 */
/*! @def INFORMATION_COLOR
 *	@brief Information console color
 */

/*! @def NO_LOG_LEVEL
 *	@brief No log level getter
 */
/*! @def PROD_LOG_LEVEL
 *	@brief Production log level getter
 */
/*! @def DEBUG_LOG_LEVEL
 *	@brief Debug log level getter
 */
/*! @def TRACE_LOG_LEVEL
 *	@brief Trace log level getter
 */
/*! @def DEFAULT_LOG_LEVEL
 *	@brief Default log level
 */

/*! @def INNER_DEFAULT_LEVEL
 *	@brief Default int level (for inner logger use only)
 */

/*! @def FORMAT_NO
 *	@brief Format for no log level
 */
/*! @def FORMAT_PROD
 *	@brief Format for prod log level
 */
/*! @def FORMAT_DEBUG
 *	@brief Format for debug log level
 */
/*! @def FORMAT_TRACE
 *	@brief Format for trace log level
 */
/*! @def DEFAULT_FORMAT
 *	@brief Default format for logger (by default - prod)
 */

/*! @def FUNCTION_NAME
 *	@brief Alias for function name getter
 */

/*! @def LOGGER(NAME)
 *	@brief In method (or function) logger creator
 * 
 *  Creates instance of logger with given @a name
 */
/*!@def LOGGER_IN_INIT_LIST
 *	@brief In initialization list logger creator
 * */

/*! @def CREATE_LOG_LEVEL(name, format)
 *	@brief Custom log level creator
 * 
 *	Creates static LogLevel instance with given @a name and @a format
 * 
 *	Makes registration in log levels map after creation
 */

/*! @def GET_LOG_LEVEL(value)
 *	@brief Log level getter
 *	
 *	@return stored log level from map with given key
 */

/*! @def CREATE_TYPE(name, short_name, color)
 *	@brief Custom message type creator
 *
 *	Creates static MessageType instance with given @a name, in log @a short_name and @a color
 */
