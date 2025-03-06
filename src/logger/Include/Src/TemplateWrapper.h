#pragma once
#include "RealLogger.h"

namespace temp_wrap
{

void log_return(const std::string& str, const std::source_location& m_location, const logger::LogLevels local)
{
	RealLogger::get_instance()->save_to_queue({"returned: " + str}, INFORMATION, m_location, local);
}

} // namespace temp_wrap