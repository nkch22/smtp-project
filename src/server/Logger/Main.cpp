#include "Include/Logger.h"

int main()
{
	logger::ThreadMap map;

	std::cout << map.get(std::this_thread::get_id()) << "\n";

	std::thread thr{[&map]
					{
						map.add();
						std::cout << map.get(std::this_thread::get_id()) << "\n";
					}};
	thr.join();
}
