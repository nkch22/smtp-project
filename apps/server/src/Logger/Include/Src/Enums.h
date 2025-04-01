#pragma once

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

} // namespace logger