#include "Logger.h"

#include <csignal>

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
	Logger::init(LOG_LEVEL_TRACE, "", 1, true, false);

	lv3(3,5);
	lv3(3, 5);
	lv3(3, 5);
	lv3(3, 5);
	Logger::set_flush(true);
	lv3(3, 5);

	Logger::set_output_dir("qe");
	Logger::stop_config();
}

