#pragma once

#include <algorithm>
#include <chrono>
#include <exception>

#include <condition_variable>
#include <thread>
#include <mutex>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include <queue>
#include <source_location>
#include <string>


namespace logger
{
enum LogLevels
{
	LOG_LEVEL_NO,
	LOG_LEVEL_PROD,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE
};

enum MessageTypes
{
	ERROR,
	WARNING,
	INFORMATION
};

}

#define DEFAULT_LEVEL logger::LOG_LEVEL_PROD
#define DEFAULT_AMOUNT 30
#define DEFAULT_PATH ""
#define DEFAULT_CONFIG false
#define DEFAULT_FLUSH true

#define DEFAULT_COLOR "\033[0m"
#define ERROR_COLOR "\033[41m"
#define WARNING_COLOR "\033[43m"
#define INFORMATION_COLOR "\033[42m"


