#include <sstream>

#include "Include/Logger.h"

class ExampleOperator
{
private:
	int a;

	int* ptr;

public:
	ExampleOperator() : a{5}, ptr{new int{a}} {}
	~ExampleOperator() { delete ptr; }

	// all you need to have to pass custom class into logger save args and return methods is this operator overload
	friend logger::Buffer& operator<<(logger::Buffer& buff, const ExampleOperator& obj)
	{
		buff << obj.a; // Buffer has default operator for int

		// if there is none you need, than make it yourself
		// the main goal of every buffer operator<< is to convert data into std::string
		std::stringstream st;
		st << obj.ptr;
		buff << st.str();

		// if you dont want to log value of some variable, than dont do it

		return buff;
	}
};

class ExampleMacros
{
private:
	int a;
	double b;

	ExampleOperator op;

	LOGGER_GET_PRIVATE(ExampleMacros) // you have to use this macros, when you want to log any members
									  // to make this class fully loggable you have to use one more macros, but
									  // you cant use it in class declaration
public:
	ExampleMacros() : a{5}, b{2.6}, op{} {}
	~ExampleMacros() {}
};
MAKE_LOGGABLE(ExampleMacros, a, b,
			  op) // this macros generates operator<< overloading for ExampleMarcos class with a and b members
				  // It is recomended to use this macros in cpp files

void NoArgsNoRet()
{
	LOGGER(log);		  // creates logger variable
	log.log_func_start(); // saves function start without arguments

	// some logic, that not need to be logged

	log.log_return_nothing(); // saves function end
}
int ArgsRet(int a)
{
	LOGGER(log);
	log.log_arguments(a); // saves function start with a parameter (might be more parameters)

	int b = a++; // some logic, that not need to be logged

	log.log_return(b); // saves function return with b output
	return b;
}

int MessageOutput(int a, int b)
{
	LOGGER(log);
	log.log_arguments(a, b);

	int c = 0;
	try
	{
		if (b == 0) throw std::invalid_argument("b can't be 0");

		c = a / b;
	}
	catch (std::invalid_argument& ex)
	{
		log.log_error(ex.what());	// saves exception message with error flag
		log.log_warning(ex.what()); // are also valid
		log.log_message(ex.what()); // the only difference is message type flag

		// you can use any of them based on your logic
	}

	log.log_return(c);
	return c;
}

void CustomClassOperator(ExampleOperator& obj)
{
	LOGGER(log);
	log.log_arguments(obj); // if you have overloaded operator, just pass it to the method
	// any type, that is not in default buffer operators, need to have overloaded one
	//  if dont and you want to log it, method will throw exception

	log.log_return_nothing();
}

void CustomClassMacros(ExampleMacros& obj)
{
	LOGGER(log);
	log.log_arguments(obj); // works petty much the same as overloaded operator, but you cant make you own overloading

	log.log_return_nothing();
}

void ArgsWithoutLogging(int*, int b)
{
	LOGGER(log);

	log.log_arguments(b); // you choose what to save
	// if you dont want to log any parameters, than use log_func_start()

	log.log_return_nothing();
}

void LogLevelsDemo()
{
	LOGGER(log);
	log.log_func_start();

	log.set_local_level(PROD_LOG_LEVEL); // Now, lets talk about log levels
	// local levels wont affect global one, and local has more priority
	// Prod log level will save only time, type and message

	log.log_func_start(); // also it wont be able to call some methods (log_func_start, log_arguments,
						  // log_return_nothing and log_return)

	log.log_message("Hi, PROD_LOG_LEVEL");

	log.set_local_level(DEBUG_LOG_LEVEL); // Debug wont be able to call log_arguments or log_return
										  // And it will save a much more information than prod - time, type, associated
										  // int for level, location (function name) and, finally, message

	log.log_message("Hello, Debug");

	log.set_local_level(TRACE_LOG_LEVEL); // Every time before this function we have used trace level
										  // it saves the most information and can call any log method
										  // it collects - thread id, time, type, level, location, message

	log.log_message("Back to Trace");

	log.log_return_nothing();
}

void CustomLogLevel()
{
	LOGGER(log);

	// There is a way to create custom log level

	CREATE_LOG_LEVEL(demo, "{:m}|{:l}|{:T}"); // First you need is level name (in this case: "demo")
	// And last is format for this level (more about format keys is in LogMacros.h documentation)
	// In this case we only will save message, level and then time

	// Then we have two options
	//  1. Use it as a local level
	//  2. Use as a global one

	log.set_local_level(demo);					 // we will use it as a local
	log.log_message("This is custom log level"); // to actually see a difference we need to log something

	// As we have noticed level = 4, and this mean that we can use any log method
	// Every custom log level can use any log method
	log.log_return_nothing();
}

void CustomMessageType() {
	LOGGER(log);
	//What if we need more than information, warning or error?
	//What if we need critial error?

	CREATE_TYPE(our_type, " OUR_TYPE ", ERROR_COLOR); // Now we can create our very own message type
	//First - variable name
	//Secong - in log name
	//Third - in log color (must be ANSI color)

	log.log_any("This is custom type", our_type); // But to use our type, we need to use log_any
}

int main()
{
	logger::Logger::init(TRACE_LOG_LEVEL); // init logger with global trace log level
	// every instance of Logger will have trace log level

	NoArgsNoRet();
	ArgsRet(5);

	MessageOutput(1, 0);

	ExampleOperator t;
	CustomClassOperator(t);

	ExampleMacros t2;
	CustomClassMacros(t2);

	int a = 6;
	ArgsWithoutLogging(&a, 5);

	LogLevelsDemo();

	CustomLogLevel();

	CustomMessageType();
}
