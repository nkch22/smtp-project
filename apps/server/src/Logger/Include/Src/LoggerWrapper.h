#pragma once
#include <string>
#include <source_location>
#include "LogLevels.h"

namespace temp_wrap
{

void wrap_return(const std::string&, const std::string&, const logger::LogLevel, const logger::Format&);

} // namespace temp_wrap
