#include "ThreadMap.h"

using namespace logger;

ThreadMap::ThreadMap()
{
	map.emplace(std::this_thread::get_id(), "MAIN");
}

std::string ThreadMap::get(const std::thread::id& id)
{
	return map[id];
}

void ThreadMap::add(const std::thread::id& id)
{
	map.try_emplace(id, "THR" + std::to_string(map.size()));
}