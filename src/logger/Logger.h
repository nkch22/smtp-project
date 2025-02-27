#pragma once

#include <algorithm>
#include <chrono>
#include <format>

#include <filesystem>
#include <fstream>
#include <iostream>

#include <source_location>
#include <string>

#include <mutex>
#include <thread>
#include <condition_variable>

#include <queue>

/*!
	@file Logger.h
	@brief Interface of Logger shared library
*/

namespace logger
{

#define DEFAULT_LEVEL LOG_LEVEL_PROD
#define DEFAULT_AMOUNT 30
#define DEFAULT_PATH ""

#define DEFAULT_COLOR "\033[0m"
#define ERROR_COLOR "\033[41m"
#define WARNING_COLOR "\033[43m"
#define INFORMATION_COLOR "\033[42m"

/*! @def DEFAULT_LEVEL
*	@brief Default log level
*/
/*! @def DEFAULT_AMOUNT
 *	@brief Default logs amount
 */
/*! @def DEFAULT_PATH
 *	@brief Default output path
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

//! Enum for log levels
enum LogLevels
{
	LOG_LEVEL_NO,
	LOG_LEVEL_PROD,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE
};

/*! @class Buffer
 *  @brief Logger class, that is used to transform variables into string
 *
 *	This class is used to transform different variables into std::stiring for futher saving by Logger
 */

class Buffer
{
private:
	std::string* m_real_buff;

public:
	Buffer();
	/*! @fn Buffer()
	 *	@brief Default empty constructor
	 *
	 *	Calls Buffer(const std::string&) with empty string
	 * */

	Buffer(const std::string&);
	/*! @fn Buffer(const std::string&)
	 *	@brief Parameterized constructor
	 *
	 *	Copies given string in created string buffer
	 */

	~Buffer();
	/*! @fn ~Buffer()
	 *	@brief Default destructor
	 *
	 *	Deletes string buffer
	 */

	std::string get() const;
	/*! @fn get()
	 *	@brief String buffer getter
	 *
	 *	@return string buffer copy
	 */

	void clear();
	/*! @fn clear()
	 *	@brief String buffer cleaner
	 *
	 *	Clears string buffer without deletion
	 */

	Buffer& operator<<(const std::string&);
	Buffer& operator<<(const char*);

	Buffer& operator<<(const int&);
	Buffer& operator<<(const unsigned int&);
	Buffer& operator<<(const double&);
	Buffer& operator<<(const bool&);

	template<typename T>
	Buffer& operator<<(const T&)
	{
		throw std::exception{};
	}

	/*! @fn operator<<(const std::string&)
	 * @brief Default overloaded operator<< with const string&.
	 * Spesifies how buffer writes given variables into string buffer
	 *
	 *	@attention If operator<< is not found for given variable in default Buffer operators <<, it will use template
	 *	@attention operator<<(const T&).
	 *
	 *	@return Buffer
	 */
	/*! @fn operator<<(const char*)
		@brief Default overloaded operator<< with const char*
	*/
	/*! @fn operator<<(const int&)
		@brief Default overloaded operator<< with const int&
	*/
	/*! @fn operator<<(const unsigned int&)
		@brief Default overloaded operator<< with const unsigned int&
	*/
	/*! @fn operator<<(const double&)
		@brief Default overloaded operator<< with const double&
	*/
	/*! @fn operator<<(const bool&)
		@brief Default overloaded operator<< with const bool&
	*/
	/*! @fn operator<<(const T&)
	 *	@brief Default template operator<<
	 *
	 *	@attention It will be called, if none of default overloaded operators << can be used
	 *	@attention If not properly overloaded in given variable, it will throw std::exception
	 *
	 *  @important Overload example:
	 *  @important	friend Buffer& operator<<(Buffer&, const YOUR_CLASS &)
	 *
	 *	@throw std::exception;
	 *
	 *  @return Buffer
	 */
};

/*! @class Logger
 *	@brief Main logger interface
 *
 *	This class implements saving messages of different types in console and txt file
 *
 *	@warning It's using inner singleton class as global variable
 * 
 *	@warning So, before using logger, user must initialize it with init(), and after using, delete with destroy()	
 * 
 *	@warning init() and destroy() calls can be replaced with MainLogger constructor and destructor
 * 
 *	@warning Every saving message methods (save_message(), save_warning(), save_error()) ignores log levels, except log level NO
 *
 *	@warning Using saving methods without previous init() call in any other place is undefined behavior
 */
class Logger
{
private:
	

	enum MessageTypes
	{
		ERROR,
		WARNING,
		INFORMATION
	};

	class RealLogger
	{
	private:

		struct Message
		{
			std::string msg;
			Logger::MessageTypes type;
			std::source_location location;
			LogLevels level;
			std::thread::id thr_id;
		};

		using queue = std::queue<Message>;

		static RealLogger* m_instance;

		static LogLevels* m_level;
		static std::string* m_output_path;
		static std::ofstream* m_file;

		static std::mutex* m_mutex; 
		static std::condition_variable* m_con_var;
		static bool* m_end;

		static std::thread* m_thr;
		static queue* m_queue;

		RealLogger(const LogLevels&, const std::string&, const unsigned int&);

		~RealLogger() = default;

	public:
		static RealLogger* get_instance();
		static RealLogger* get_instance(const LogLevels&, const std::string&, const unsigned int&);

		static void destroy();

		static void real_save(const std::string&, const Logger::MessageTypes&, const std::source_location&,
							  const LogLevels& level, std::thread::id id = std::this_thread::get_id());

		static void real_set_level(const LogLevels&);
		static LogLevels real_get_level();
	};

	RealLogger* m_real;

	Buffer m_buff;

	const std::source_location m_location;

	LogLevels m_local_level;

	template<typename T>
	void save_argument(const T& value)
	{
		m_buff << value;
	}

	void save_arguments();

public:
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

	static bool init(const LogLevels& level = DEFAULT_LEVEL, const std::string& save_path = DEFAULT_PATH,
					 const unsigned int& amount = DEFAULT_AMOUNT);
	/*! @fn init(const unsigned short& level, const std::string& save_path, const unsigned int& amount)
	 *  @brief Singleton initialization method
	 *
	 *	@important level - is global log level on start, default value = 1
	 *	@important save_path - is path to output directory,	default value = ""
	 *	@important amount - is amount of how many logs can be in log folder, default value = 30
	 *
	 *	It will save message of successful initialization
	 * 
	 *	@warning It will throw std::invalid_argument if 
	 *	@warning 1. Level is out of scope OR
	 *	@warning 2. Save_path is incorrect OR
	 *	@warning 3. Amount is less than 1
	 *
	 *	@throw std::invalid_argument
	 * 
	 *	@return initialization state (true or false)
	 */

	static bool destroy();
	/*! @fn destroy()
	 *	@brief Destroyes singleton valiable
	 *
	 *	It will save message of destroying singleton
	 *
	 *	@return deletion state (true or false)
	 */

	void save_error(const std::string&);
	/*! @fn save_error(const std::string&)
	 *	@brief It saves error
	 *
	 *	Saves messages with error flag
	 *
	 *	@attention It won`t stop function execution!
	 */

	void save_warning(const std::string&);
	/*! @fn save_warning(const std::string&)
	 *  @brief It saves warning
	 *
	 *  Saves message with warning flag
	 */

	void save_message(const std::string&);
	/*! @fn save_message(const std::string&)
	 *  @brief It saves message
	 *
	 *   Saves message with information flag
	 */

	void set_global_level(const LogLevels&);
	/*! @fn set_global_level(const unsigned short&)
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
	void save_return(const T& value)
	{
		if (m_local_level == LOG_LEVEL_TRACE)
		{
			m_buff << value;
			m_real->real_save({"returned: " + m_buff.get()}, INFORMATION, m_location, m_local_level);
			m_buff.clear();
		}
		else
			this->save_return_nothing();
	}
	/*! @fn save_return(const T&)
	 *	@brief It saves return value of funtion
	 *
	 *	@warning If custom class is given as parameter, then it needs to meet the requirements of Buffer::operator<<()
	 */

	void save_return_nothing();
	/*! @fn save_return_nothing()
		@brief It save end of function execution

		It saves end of function execution as information message

		This method should be called at the end of void functions
	*/

	template<typename T, typename... Args>
	void save_arguments(const T& first, Args&... args)
	{
		if (m_local_level == LOG_LEVEL_TRACE)
		{
			save_argument(first);
			save_arguments(std::forward<Args>(args)...);
		}
		else
			this->save_func_start();
	}
	/*! @fn save_arguments(const T& first, Args&... args)
	 *	@brief It saves input arguments
	 *
	 *	@warning If custom class is given as parameter, then it needs to meet the requirements of Buffer::operator<<()
	 *
	 *	This method should be called at the beginning of parameterized function
	 */

	void save_func_start();
	/*! @fn save_func_start()
		@brief It saves start of function

		It saves message that function has successfully started
	*/

	void set_local_level(const LogLevels&);
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
};

/*! @class MainLogger
 *   @brief Class that automatically controls lifecycle of Logger singleton
 *
 *	 @warning There can be only one MainLogger, otherwise its undefined behavior (when destructor is called)
 */
class MainLogger
{
private:
	Logger* m_log;

public:
	MainLogger(const LogLevels& level = DEFAULT_LEVEL, const std::string& path = DEFAULT_PATH,
			   const unsigned int& amount = DEFAULT_AMOUNT,
			   const std::source_location location = std::source_location::current());
	/*! @fn MainLogger(const unsigned short&, const std::string&, const unsigned int&)
	 *	@brief Default constructor
	 *
	 *	@attention Calls to Logger::init() with given parameters
	 */

	~MainLogger();
	/*! @fn ~MainLogger()
		@brief Default destructor

		@attention Calls Logger::destroy()
	*/

	Logger& get() const;
	/*! @fn get()
	 *	@brief Logger getter
	 * 
	 *	@return stored Logger reference
	 */
};
} // namespace logger