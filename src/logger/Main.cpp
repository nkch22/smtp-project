#include "Logger.h"

using namespace logger;

void lv1() {
	Logger log;
	log.save_func_start();

	log.set_local_level(LOG_LEVEL_PROD);

	log.save_func_start();

	log.save_return_nothing();
}

void lv2() {
	Logger log;

	log.set_local_level(LOG_LEVEL_DEBUG);

	log.save_func_start();
	log.save_return_nothing();
}

int lv3(int a, int b) {
	Logger log;

	log.set_local_level(LOG_LEVEL_TRACE);

	log.save_arguments(a,b);

	log.save_return(a+b);
	return a + b;
}

int lv_glob(int a, int b) {
	Logger log;

	log.save_arguments(a, b);

	log.save_return(a + b);
	return a + b;
}

int main() {

	//MainLogger log{LOG_LEVEL_TRACE};
	Logger::init(LOG_LEVEL_TRACE);



}

