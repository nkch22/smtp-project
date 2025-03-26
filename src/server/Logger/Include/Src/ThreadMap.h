#pragma once
#include <thread>
#include <unordered_map>
#include <string>
#include <mutex>

namespace logger
{

class ThreadMap
{
private:
	std::unordered_map<std::thread::id, std::string> map;
	std::mutex mutex;

public:
	ThreadMap();
	~ThreadMap() = default;

	void add(const std::thread::id& id = std::this_thread::get_id());

	std::string get(const std::thread::id&);
};

} // namespace logger