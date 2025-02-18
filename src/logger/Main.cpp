#include "Logger.h"

void func() {
	LOG_START();
	LOG_RETURN_NOTHING();
}

int add(int a, int b) {
	LOG_START_ARGS(a, b);
	LOG_RETURN(a + b);
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

	log.get().set_level(LOG_LEVEL_PROD);
}