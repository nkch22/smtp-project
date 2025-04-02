#pragma once
#include <string>
#include <source_location>
#include "Enums.h"
#include "LogLevels.h"

namespace temp_wrap
{

void wrap_return(const std::string& str, const std::source_location& m_location, const logger::LogLevel local);

} // namespace temp_wrap
