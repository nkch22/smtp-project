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

#define DEFAULT_LEVEL logger::LOG_LEVEL_PROD
#define DEFAULT_AMOUNT 30
#define DEFAULT_PATH ""
#define DEFAULT_CONFIG false
#define DEFAULT_FLUSH true

#define DEFAULT_COLOR "\033[0m"
#define ERROR_COLOR "\033[41m"
#define WARNING_COLOR "\033[43m"
#define INFORMATION_COLOR "\033[42m"

#define DEFAULT_FORMAT "{:L} | {:t} | {:m}" 
// "[{:i}] [{:T}] {:t} [{:l}] [{:L}] [{:m}]"

#define FUNCTION_NAME __FUNCTION__

#define LOGGER(NAME)                                                                                                   \
	logger::Logger NAME {FUNCTION_NAME}

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
 *	@brief Generates overloaded operator<< for given class with given members
 */

/*! @def DEFAULT_LEVEL
 *	@brief Default log level
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