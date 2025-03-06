#include "Logger.h"

int main()
{
	logger::Logger::init(logger::LOG_LEVEL_DEBUG);

	std::vector<std::thread> vec;

	for (int i = 0; i < 1000; i++)
	{
		vec.emplace_back(
			[]
			{
				logger::Logger log;
				log.save_func_start();

				log.save_message("hello world");
				log.save_message("logger");
				log.save_message("test");

				log.save_return_nothing();
			});
	}

	for (auto& thr : vec)
	{
		thr.join();
	}
}