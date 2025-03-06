#pragma once
#include "Src/SharedInclude.h"
#include "Src/TemplateWrapper.h"

/*!
 *	@file Logger.h
 *	@brief Interface of Logger shared library
 *
 *	This file contains logger interface
 *
 *	@section usage_example Usage example
 *	@code
 *
 *	#include "Logger.h"
 *	#include <sstream>
 * 
 *	void NoArgsNoRet();
 *	int ArgsRet(int a);
 *
 *	int LocalLevel(int a);
 *
 *	int MessageOutput(int a, int b);
 * 
 *	class Test;
 *	void CustomClass(Test);
 *
 *	void ArgsWithoutLogging(int* a, int b);
 *
 *	int main()
 *	{
 *		logger::Logger::init(logger::LOG_LEVEL_TRACE); // init logger with global trace log level
 *		//every instance of Logger will have trace log level
 *
 *		NoArgsNoRet();
 *		ArgsRet(5);
 *
 *		LocalLevel(5);
 *		MessageOutput(1, 0);
 *
 *		Test t;
 *		CustomClass(t);
 *
 *		int a = 6;
 *		ArgsWithoutLogging(&a, 5);
 *	}
 *
 *	void NoArgsNoRet()
 *	{
 *		logger::Logger log;	   // creates logger variable
 *		log.save_func_start(); // saves function start without arguments
 *
 *		// some logic, that not need to be logged
 *
 *		log.save_return_nothing(); // saves function end
 *	}
 *	int ArgsRet(int a)
 *	{
 *		logger::Logger log;
 *		log.save_arguments(a); // saves function start with a parameter (might be more parameters)
 *
 *		int b = a++; // some logic, that not need to be logged
 *
 *		log.save_return(b); // saves function return with b output
 *		return b;
 *	}
 *
 *	int LocalLevel(int a)
 *	{
 *		logger::Logger log;
 *		log.set_local_level(
 *			logger::LOG_LEVEL_DEBUG); // set local level to debug (no input parameters or return will be saved)
 *		// Global log level won't be affected
 *		log.save_arguments(a); // because of debug log level will be replaced with save_func_start()
 *
 *		int b = a++; // some logic, that not need to be logged
 *
 *		log.save_return(b); // because of debug log level will be replaced with save_return_nothing()
 *		return b;
 *	}
 *
 *	int MessageOutput(int a, int b)
 *	{
 *		logger::Logger log;
 *		log.save_arguments(a, b);
 *
 *		int c = 0;
 *		try
 *		{
 *			if (b == 0) throw std::invalid_argument("b can't be 0");
 *
 *			c = a / b;
 *		}
 *		catch (std::invalid_argument& ex)
 *		{
 *			log.save_error(ex.what());	 // saves exception message with error flag
 *			log.save_warning(ex.what()); // are also valid
 *			log.save_message(ex.what()); // the only difference is message type flag
 *			// you can use any of them based on your logic
 *		}
 *
 *		log.save_return(c);
 *		return c;
 *	}
 *
 *	class Test
 *	{
 *	private:
 *		int a;
 *		int* ptr;
 *
 *		void* none;
 *
 *	public:
 *		Test() : a{5}, ptr{&a}, none{nullptr} {}
 *
 *		// all you need to have to pass custom class into logger save args and return methods is this operator overload
 *		friend logger::Buffer& operator<<(logger::Buffer& buff, const Test& obj)
 *		{
 *			buff << obj.a; //Buffer has default operator for int, see Buffer Documentation page for more
 *
 *			//if there is none you need, than make it yourself
 *			//the main goal of every buffer operator<< is to convert data into std::string
 *			std::stringstream st;
 *			st << obj.ptr;
 *			buff << st.str();
 *
 *			//if you dont want to log value of some variable, than dont do it
 *
 *			//no logging for 'none' void ptr
 *
 *			return buff;
 *		}
 *	};
 *	void CustomClass(Test obj) {
 *		logger::Logger log;
 *		log.save_arguments(obj); //if you have overloakded operator, just pass it to the method
 *		//any type, that is not in default buffer operators, need to have overloaded one
 *		// if dont and you want to log it, method will throw exception
 *
 *		log.save_return_nothing();
 *	}
 *
 *	void ArgsWithoutLogging(int* a, int b) {
 *		logger::Logger log;
 *
 *		log.save_arguments(b); //you choose what to save
 *		//if you dont want to log any parameters, than use save_func_start()
 *
 *		log.save_return_nothing();
 *	}
 *	@endcode
 */

class RealLogger;

namespace logger
{
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

	Buffer& operator<<(const int);
	Buffer& operator<<(const unsigned int);
	Buffer& operator<<(const double);
	Buffer& operator<<(const bool);

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
 *	@warning So, before using logger, user must initialize it with init()
 *
 *	@warning init() method will delete Logger singleton AFTER program ends or crashes
 *
 *  @warning Don't use init() more than once
 *
 *	@warning Using saving methods without previous init() call in any other place is undefined behavior
 *
 *  @warning By setting flush value to false, Logger will stop storing any log messages
 */


class Logger
{
private:

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

	static void destroy();

	static bool init(const LogLevels level = DEFAULT_LEVEL, const std::string& save_path = DEFAULT_PATH,
					 const unsigned int amount = DEFAULT_AMOUNT, const bool is_config = false,
					 const bool do_flush = true);
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
	 *	@warning It will throw std::invalid_argument if
	 *	@warning 1. Level is out of scope OR
	 *	@warning 2. Save_path is incorrect OR
	 *	@warning 3. Amount is less than 1
	 *
	 *  @attention this method can be called only once
	 *
	 *	@throw std::invalid_argument
	 *
	 *	@return initialization state (true or false)
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

	void set_global_level(const LogLevels);
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
	void save_return(const T& value)
	{
		if (m_local_level == LOG_LEVEL_TRACE)
		{
			m_buff << value;
			temp_wrap::save_return(m_buff.get(), m_location, m_local_level);
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

	static void set_flush(const bool);
	/*! @fn set_flush(const bool&)
	 *	@brief Flush setter
	 *
	 *	@attention If flush was set to false, logger won't store any log messages
	 */
};
} // namespace logger
