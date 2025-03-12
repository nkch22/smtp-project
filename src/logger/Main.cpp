#include "Include/Logger.h"

class Test
{
	const int a = 1;
	const double b = 2;

	MAKE_PRIVATE_LOGGER(Test)
	
//public:
//	Test(int _a = 5, double _b = 10.6) : a{_a}, b{_b} {}
};

MAKE_LOGGABLE(Test, Test::a, Test::b)

//LOGGER_DEFINE_SERIALIZER(Test, Test::a, Test::b)
//
//LOGGER_DEFINE(Test)


void func(Test t, int a) {
	logger::Logger log;
	log.log_arguments(t, a);
}

int main() {
	logger::Logger::init(logger::LOG_LEVEL_TRACE);

	logger::Logger log;

	func(Test{}, 5);
	
	Test t;
	log.log_return(t);

}