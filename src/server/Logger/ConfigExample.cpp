#include "Include/Logger.h"

int main() {
	logger::Logger::init(logger::LOG_LEVEL_TRACE, DEFAULT_PATH, DEFAULT_AMOUNT, true);
	//set config flag to true
	//it prevents logger from writing logs into file, because file is not initialized yet

	//start parsing JSON or other files

	logger::Logger log;

	log.log_message("Hello world!"); // you can use logger as normal

	//end parsing
	std::string new_path = "";
	logger::Logger::set_output_dir(new_path); //set output path or other properties if needed

	logger::Logger::stop_config(); //need to call after configuration
	//after call to this method every saved log messages will be flushed
}