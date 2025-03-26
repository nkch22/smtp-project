#pragma once
#include "Macros/LoggerMacros.h"
#include "Src/Buffer.h"
#include "Src/LoggerWrapper.h"
#include "Src/SharedInclude.h"
#include "Src/ThreadMap.h"

class RealLogger;

/*!
 *	@file Logger.h
 *	@brief Interface of Logger shared library
 *
 *	This file contains logger interface
 */
namespace logger
{

/*! @class Logger
 *	@brief Main logger interface
 *
 *	This class implements saving messages of different types in console and txt file
 *
 *	@warning It's using inner singleton class as global variable
 *
 *	@warning So, before using logger, user must initialize it with init()
 *
 *	@warning init() method will delete Logger singleton AFTER program ends or crashes
 *
 *	@warning Using saving methods without previous init() call in any other place is undefined behavior
 *
 *  @warning By setting flush value to false, Logger will stop storing any log messages
 *
 *	@attention When using log_return() or log_arguments(), you need to have overloaded operator<<.
 *
 *	@section code_example Example of overloaded operator<< for class Test with int a member
 *	@code
 *		friend Buffer& operator<<(Buffer& buff, const Test& obj){
 *			buff << obj.a;
 *			return buff;
 *		}
 *	@endcode
 *
 *	@attention There are such default overloaded operators:	int, char, char*, std::string, unsigned int, double, bool
 *
 *	@warning If class doesnt have operator, but it was passed to log_return or log_arguments, then it will log warning
 *
 * @attention You can use macros instead of overloading operator<<.
 * 
 * @section macros_example Example of macros for class Test with a and b members
 *	@code
 *		class Test{
 *			const int a = 5;
 *			const double b = 7.8;
 *			
 *			LOGGER_GET_PRIVATE(Test) //Defines operator<< for class Test
 *	    }
 *		MAKE_LOGGABLE(Test, a, b) //Creates operator<< for given class and members
 *	@endcode
 */
class Logger
{
private:
	RealLogger* m_real;

	Buffer m_buff;

	const std::source_location m_location;

	LogLevels m_local_level;

	template<typename T>
	void log_argument(const T& value)
	{
		m_buff << value;
	}

	void log_arguments();

public:
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	void operator=(const Logger&) = delete;

	Logger(const std::source_location location = std::source_location::current());
	/*! @fn Logger(const std::source_location location = std::source_location::current())
	 *	@brief Default constructor
	 *
	 *	It stores location where it was created
	 */

	~Logger() = default;
	/*! @fn ~Logger()
	 *	@brief Default destructor
	 *
	 *	Trivial destructor
	 */

	static void destroy();
	/*! @fn destroy()
	 *	@brief Destroy method
	 *
	 *	Destroys global values
	 */

	static bool init(const LogLevels level = DEFAULT_LEVEL, const std::string& save_path = DEFAULT_PATH,
					 const unsigned int amount = DEFAULT_AMOUNT, const bool is_config = DEFAULT_CONFIG,
					 const bool do_flush = DEFAULT_FLUSH);
	/*! @fn init(const unsigned short& level, const std::string& save_path, const unsigned int& amount)
	 *  @brief Singleton initialization method
	 *
	 *	@important @a level - is global log level on start, default value = 1
	 *	@important @a save_path - is path to output directory,	default value = ""
	 *	@important @a amount - is amount of how many logs can be in log folder, default value = 30
	 *	@important @a is_config - is flag to stop printing messages to console and file, default value = false
	 *	@important @a do_flush - is flag to stop storing any messages (if set to false), default value = true
	 *
	 *	It will save message of successful initialization
	 *
	 *	@warning It will log warning if
	 *	@warning 1. Save_path is incorrect OR
	 *	@warning 2. Amount is less than 1
	 *
	 *  @attention this method need to be called only once
	 *
	 *	@return initialization state (true or false)
	 */

	void log_error(const std::string&);
	/*! @fn log_error(const std::string&)
	 *	@brief It saves error
	 *
	 *	Saves messages with error flag
	 *
	 *	@attention It won`t stop function execution!
	 */

	void log_warning(const std::string&);
	/*! @fn log_warning(const std::string&)
	 *  @brief It saves warning
	 *
	 *  Saves message with warning flag
	 */

	void log_message(const std::string&);
	/*! @fn log_message(const std::string&)
	 *  @brief It saves message
	 *
	 *   Saves message with information flag
	 */

	static void set_global_level(const LogLevels);
	/*! @fn set_global_level(const LogLevels&)
	 *	@brief Global log level setter
	 *
	 *	It checks value to be valid log level, and then set global log level to that value
	 *
	 *  @warning local log level has more priority than global one
	 */

	LogLevels get_global_level() const;
	/*! @fn get_global_level()
	 *	@brief Global log level getter
	 *
	 *	@return Global log level value
	 */

	template<typename T>
	void log_return(const T& value)
	{
		if (m_local_level == LOG_LEVEL_TRACE)
		{
			m_buff << value;
			temp_wrap::wrap_return(m_buff.get(), m_location, m_local_level);
			m_buff.clear();
		}
		else
			this->log_return_nothing();
	}
	/*! @fn log_return(const T&)
	 *	@brief It saves return value of funtion
	 *
	 *	@warning If custom class is given as parameter, then it needs to meet the requirements of overloaded
	 *operator<<() (see them in class description)
	 */

	void log_return_nothing();
	/*! @fn log_return_nothing()
		@brief It save end of function execution

		It saves end of function execution as information message

		This method should be called at the end of void functions
	*/

	template<typename T, typename... Args>
	void log_arguments(const T& first, Args&... args)
	{
		if (m_local_level == LOG_LEVEL_TRACE)
		{
			log_argument(first);
			log_arguments(std::forward<Args>(args)...);
		}
		else
			this->log_func_start();
	}
	/*! @fn log_arguments(const T& first, Args&... args)
	 *	@brief It saves input arguments
	 *
	 *	@warning If custom class is given as parameter, then it needs to meet the requirements of overloaded
	 *operator<<() (see them in class description)
	 *
	 *	This method should be called at the beginning of parameterized function
	 */

	void log_func_start();
	/*! @fn log_func_start()
		@brief It saves start of function

		It saves message that function has successfully started
	*/

	void set_local_level(const LogLevels);
	/*! @fn set_local_level(const LogLevels&)
	 *	@brief Local log level setter
	 *
	 *	Sets local log level to given value, if value is valid
	 *
	 *   @warning local log level has more priority than global one
	 */

	LogLevels get_local_level() const;
	/*! @fn get_local_level()
	 *  @brief Local log level getter
	 *
	 *  @return Local log level value
	 */

	static void stop_config();
	/*! @fn stop_config()
	 *	@brief Logger configuration stopper
	 *
	 *	Allows logger to write messages by stopping its configuration
	 *
	 *	@attention Can be used only if @a is_config in init() was set to true
	 *	@attention Can be used only once
	 */

	static void set_output_dir(const std::string&);
	/*! @fn set_output_dir(const std::string&)
	 *	@brief Output directory setter
	 *
	 *	@attention Can be used only in configuration mode (if @a is_config was set to true in init())
	 */

	static std::string get_output_path();
	/*! @fn get_output_path()
	*	@brief Output path getter
	*/

	static void set_flush(const bool);
	/*! @fn set_flush(const bool&)
	 *	@brief Flush setter
	 *
	 *	@attention If flush was set to false, logger won't store any log messages
	 */

};
} // namespace logger
