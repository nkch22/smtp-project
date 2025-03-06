#pragma once
#include "RealLogger.h"

namespace temp_wrap
{

void save_return(const std::string& str, const std::source_location& m_location, const logger::LogLevels local)
{
	RealLogger::get_instance()->real_save({"returned: " + str}, INFORMATION, m_location, local);
}

} // namespace temp_wrap