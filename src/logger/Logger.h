#pragma once

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <thread>

/*!
	@file Logger.h
	@brief Interface of Logger shared library
*/

#define LOG_START_ARGS(...)                                                                                            \
	Logger log;                                                                                                        \
	switch (log.get_level())                                                                                           \
	{                                                                                                                  \
	case LOG_LEVEL_TRACE:                                                                                              \
		log.save_arguments(__VA_ARGS__);                                                                               \
		break;                                                                                                         \
	case LOG_LEVEL_DEBUG:                                                                                              \
		log.save_func_start();                                                                                         \
		break;                                                                                                         \
	}

#define LOG_START()                                                                                                    \
	Logger log;                                                                                                        \
	if (log.get_level() >= LOG_LEVEL_DEBUG) log.save_func_start()

#define LOG_RETURN(value)                                                                                              \
	switch (log.get_level())                                                                                           \
	{                                                                                                                  \
	case LOG_LEVEL_TRACE:                                                                                              \
		log.save_return(value);                                                                                        \
		break;                                                                                                         \
	case LOG_LEVEL_DEBUG:                                                                                              \
		log.save_return_nothing();                                                                                     \
		break;                                                                                                         \
	}                                                                                                                  \
	return value

#define LOG_RETURN_NOTHING()                                                                                           \
	if (log.get_level() >= LOG_LEVEL_DEBUG) log.save_return_nothing()

#define LOG_SAVE_ERROR(value) log.save_error(value)
#define LOG_SAVE_WARNING(value) log.save_warning(value)
#define LOG_SAVE_MESSAGE(value) log.save_message(value)

//! Enum for log levels
enum LogLevels
{
	LOG_LEVEL_NO,
	LOG_LEVEL_PROD,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE
};

/*! @class Buffer
 *   @brief Logger class, that is used to transform variables into string
 *
 *	This class is used to transform different variables into std::stiring for futher saving by Logger
 */

class Buffer
{
private:
	std::string* m_real_buff; //!< String buffer

public:
	Buffer(); /*!< Default empty constructor*/

	Buffer(const std::string&); /*!< Copies value of given string into string buffer */

	~Buffer(); /*!< Deletes string buffer */

	std::string get() const; /*!< Method to get stored string */

	void clear(); /*!< Method to clear string buffer*/

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
	 * Spesifies how buffer writes given variables into string buffer.
	 *
	 *		Be careful:
	 *			If operator<< is not found for given variable in default Buffer operators <<, it will use template
	 *			operator<<().
	 *
	 *			If template operator<< is not defined with friend specifier in variable definition, it will throw
	 *			std::exception
	 *
	 *	Overload example:
	 *
	 *			friend Buffer& operator<<(Buffer&, const YOUR_CLASS &)
	 */
	/*! @fn operator<<(const char*)
		Default overloaded operator<< with const char*
	*/
	/*! @fn operator<<(const int&)
		Default overloaded operator<< with const int&
	*/
	/*! @fn operator<<(const unsigned int&)
		Default overloaded operator<< with const unsigned int&
	*/
	/*! @fn operator<<(const double&)
		Default overloaded operator<< with const double&
	*/
	/*! @fn operator<<(const bool&)
		Default overloaded operator<< with const bool&
	*/
	/*! @fn operator<<(const T&)
	 *	Default template operator<<
	 *
	 *	Be careful:
	 *
	 *			It will be called, if none of default overloaded operators << can be used
	 *
	 *			If not properly overloaded in given variable, it will throw std::exception
	 */
};

/*! @class Logger
 *	@brief Main logger interface
 *
 *	This class implements saving messages of different types in console and txt file
 *
 *	Warning:
 *
 *		It's using inner singleton class as global variable
 *
 *		So, before using logger, user must initialize it with init(),
 *			and after using, delete with destoy()
 *
 *		Init() and destoy() calls can be replaced with MainLogger constructor and destructor
 *
 *		Every saving method ignores log levels, except log level NO
 *
 *		Using saving methods or macros without previous init() call in any other place is undefined behavior
 *
 *		It is recomended to use macros that calls needed method with respect to log level
 *
 *	@sa Logger.h, MainLogger
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
		static RealLogger* m_instance;

		static unsigned short* m_level;
		static std::string* m_output_path;
		static std::ofstream* m_file;
		static std::mutex* m_mutex;

		RealLogger(const unsigned short&, const std::string&, const unsigned int&);

		~RealLogger() = default;

	public:
		static RealLogger* get_instance();
		static RealLogger* get_instance(const unsigned short&, const std::string&, const unsigned int&);

		static void destroy();

		void real_save(const std::string&, const Logger::MessageTypes&, const std::source_location&);

		void real_set_level(const unsigned short&);
		unsigned short real_get_level() const;
	};

	RealLogger* m_real;

	Buffer m_buff;

	const std::source_location m_location;

	template<typename T>
	void save_argument(const T& value)
	{
		m_buff << value;
	}

	void save_arguments();

public:
	Logger(const std::source_location location = std::source_location::current());
	~Logger(); //!< Default destructor

	/*! @fn Logger(const std::source_location location = std::source_location::current())
	 *	@brief Default constructor
	 *
	 *	It stores location where it was created
	 */

	static bool init();
	static bool init(const unsigned short& level);
	static bool init(const std::string& save_path);
	static bool init(const unsigned int& amount);
	static bool init(const unsigned short& level, const std::string& save_path, const unsigned int& amount);
	/*! @fn init(const unsigned short& level, const std::string& save_path, const unsigned int& amount)
	 *   @brief Singleton initialization method
	 *
	 *		level - is log level on start,		default value = 1
	 *		save_path - is path to output directory,		default value = ""
	 *		amount - is amount of how many logs can be in log folder,		default value = 30
	 *
	 *	It will save message of successful initialization
	 *
	 *	It return true if initialization is successful, otherwise false
	 */
	/*! @fn init()
	 *	Initialize with all default values
	 */
	/*! @fn init(const unsigned short&)
	 *	Initialize with partly default values
	 */
	/*! @fn init(const std::string&)
	 *	Initialize with partly default values
	 */
	/*! @fn init(const unsigned int&)
	 *	Initialize with partly default values
	 */

	static bool destroy();
	/*! @fn destroy()
	 *	@brief Destroyes singleton valiable
	 *
	 *	It will save message of destroying singleton
	 *
	 *	It returns true, if singleton was destroyed successfully, else false
	 */

	void save_error(const std::string&);
	/*! @fn save_error(const std::string&)
	 *	@brief It saves error
	 *
	 *	Saves messages with error flag
	 *
	 *		Be careful:
	 *			It won`t stop function execution!
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

	void set_level(const unsigned short&);
	/*! @fn set_level(const unsigned short&)
	 *	@brief It sets log level to new value
	 *
	 *	It checks value to be valid log level, and then set log level to that value
	 */

	unsigned short get_level() const;
	/*! @fn get_level()
	 *	@brief It returns stored log level value
	 */

	template<typename T>
	void save_return(const T& value)
	{
		if (m_real->real_get_level() == LOG_LEVEL_TRACE)
		{
			m_buff << value;
			m_real->real_save({"returned: " + m_buff.get()}, INFORMATION, m_location);
			m_buff.clear();
		}
		else
			this->save_return_nothing();
	}
	/*! @fn save_return(const T&)
	 *	@brief It saves return value of funtion
	 *
	 *	Be careful:
	 *
	 *		If custom class is given as parameter, then it needs to meet the requirements of Buffer::operator<<()
	 */

	void save_return_nothing();
	/*! @fn save_return_nothing()
		@brief It save end of function execution

		It saves end of function execution as inforation message

		This method should be called at the end of void functions
	*/

	template<typename T, typename... Args>
	void save_arguments(const T& first, Args&... args)
	{
		if (m_real->real_get_level() == LOG_LEVEL_TRACE)
		{
			save_argument(first);
			save_arguments(std::forward<Args>(args)...);
		}
		else
			this->save_func_start();
	}
	/*! @fn save_arguments(const T& first, Args&... args)
	 *	@brief It save input arguments
	 *
	 *	Be careful:
	 *
	 *		If custom class is given as parameter, then it needs to meet the requirements of Buffer::operator<<()
	 *
	 *	This method should be called at the beginning of parameterized function
	 */

	void save_func_start();
	/*! @fn save_func_start()
		@brief It saves start of function

		It saves message that function has successfully started
	*/
};

/*! @class MainLogger
 *   @brief Class that automatically controls lifecycle of Logger singleton
 *
 *	Be careful:
 *
 *		There can be only one MainLogger, otherwise its undefined behavior
 */
class MainLogger
{
private:
	Logger* m_log;

public:
	MainLogger();
	MainLogger(const unsigned short&);
	MainLogger(const unsigned short&, const std::string&);
	MainLogger(const unsigned short&, const std::string&, const unsigned int&);
	/*! @fn MainLogger(const unsigned short&, const std::string&, const unsigned int&)
	 *	@brief Full MainLogger constructor
	 *
	 *	It calls to Logger::init() with given parameters
	 */
	/*! @fn MainLogger(const unsigned short&, const std::string&)
	 *	It calles full constructor
	 */
	/*! @fn MainLogger(const unsigned short&)
	 *	It calles full constructor
	 */
	/*! @fn MainLogger()
	 *	It calls full constructor with default values
	 */

	~MainLogger();
	/*! @fn ~MainLogger()
		@brief Destructor

		It calls Logger::destroy()
	*/

	Logger& get() const;
	/*! @fn get()
	 *	@brief It returns stored log
	 */
};