#pragma once
#include <thread>
#include <unordered_map>
#include <string>
#include <mutex>

/*! @file ThreadMap.h
*	@brief ThreadMap class header
*/

namespace logger_inner
{
	/*!@class ThreadMap
	*	@brief Thread id register
	* 
	*	This class is used to replace random numeric thread id with short strings
	*
	*	Example: 895983495 -> MAIN, 789895645 -> THR1, 0 -> OUT
	*/
class ThreadMap
{
private:
	std::unordered_map<std::thread::id, std::string> map;
	std::mutex mutex;

public:
	/*! @brief Default constructor*/
	ThreadMap();
	/*! @brief Trivial destructor*/
	~ThreadMap() = default;

	/*! @brief Adds thread id
	* 
	*	Adds thread id into map with associated name
	*/
	void add(const std::thread::id& id = std::this_thread::get_id());

	/*! @brief Gets thread name
	* 
	 *	Returns associated name to thread id
	 */
	std::string get(const std::thread::id&);
};

} // namespace logger