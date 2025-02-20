#include "Logger.h"

void func() {
	LOG_START();
	LOG_RETURN_NOTHING();
}

int add(int a, int b) {
	LOG_START_ARGS(a, b);
	LOG_RETURN(a + b);
}


void func1()
{
	throw std::exception();
	
}
void caller()
{
	LOG_START();
	
	try
	{
		func1();
		
	}
	catch (std::exception& e)
	{
		LOG_SAVE_ERROR("Error was caught");
	}

	LOG_RETURN_NOTHING();
}


int main() {
	MainLogger log;

	add(6, 9);
	func();

	log.get().set_level(LOG_LEVEL_DEBUG);
	add(6, 9);
	func();

	log.get().set_level(LOG_LEVEL_TRACE);
	add(6, 9);
	func();

	caller();
}