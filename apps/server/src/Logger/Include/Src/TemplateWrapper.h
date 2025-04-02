#pragma once
#include "RealLogger.h"

namespace temp_wrap
{

void log_return(const std::string& str, const std::source_location& m_location, const logger::LogLevels local);

} // namespace temp_wrap
