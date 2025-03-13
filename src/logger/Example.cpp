#include <sstream>

#include "Include/Logger.h"

namespace example
{
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
		buff << obj.a; // Buffer has default operator for int, see Buffer Documentation page for more

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
	LOGGER_GET_PRIVATE(example::ExampleMacros) // you have to use this macros, when you want to log private members
									  // to make this class fully loggable you have to use one more macros, but you cant
									  // use it inside namespace

	int a;
	double b;

public:
	ExampleMacros() : a{5}, b{2.6} {}
	~ExampleMacros() {}
};


void NoArgsNoRet()
{
	logger::Logger log;	  // creates logger variable
	log.log_func_start(); // saves function start without arguments

	// some logic, that not need to be logged

	log.log_return_nothing(); // saves function end
}
int ArgsRet(int a)
{
	logger::Logger log;
	log.log_arguments(a); // saves function start with a parameter (might be more parameters)

	int b = a++; // some logic, that not need to be logged

	log.log_return(b); // saves function return with b output
	return b;
}

int LocalLevel(int a)
{
	logger::Logger log;
	log.set_local_level(
		logger::LOG_LEVEL_DEBUG); // set local level to debug (no input parameters or return will be saved)
	// Global log level won't be affected

	log.log_arguments(a); // because of debug log level will be replaced with log_func_start()

	int b = a++; // some logic, that not need to be logged

	log.log_return(b); // because of debug log level will be replaced with log_return_nothing()
	return b;
}

int MessageOutput(int a, int b)
{
	logger::Logger log;
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
	logger::Logger log;
	log.log_arguments(obj); // if you have overloaded operator, just pass it to the method
	// any type, that is not in default buffer operators, need to have overloaded one
	//  if dont and you want to log it, method will throw exception

	log.log_return_nothing();
}

void CustomClassMacros(ExampleMacros& obj)
{
	logger::Logger log;
	log.log_arguments(obj); //works petty much the same as overloaded operator, but you cant make you own overloading

	log.log_return_nothing();
}

void ArgsWithoutLogging(int*, int b)
{
	logger::Logger log;

	log.log_arguments(b); // you choose what to save
	// if you dont want to log any parameters, than use log_func_start()

	log.log_return_nothing();
}
} // namespace example


class Test
{
	LOGGER_GET_PRIVATE(Test)

	const int a = 7;
};
MAKE_LOGGABLE(Test, a)

void test(Test h) {
	logger::Logger log;
	log.log_arguments(h);
}

using namespace example;

MAKE_LOGGABLE(ExampleMacros, a, b)
// This macros generates operator<< overloading for ExampleMarcos class with a and b members


int main()
{
	logger::Logger::init(logger::LOG_LEVEL_TRACE); // init logger with global trace log level
	// every instance of Logger will have trace log level

	NoArgsNoRet();
	ArgsRet(5);

	LocalLevel(5);

	MessageOutput(1, 0);

	ExampleOperator t;
	CustomClassOperator(t);

	ExampleMacros t2;
	CustomClassMacros(t2);

	int a = 6;
	ArgsWithoutLogging(&a, 5);

	test(Test{});
}
