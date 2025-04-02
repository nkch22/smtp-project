#pragma once
#include <string>
#include <source_location>
#include "Enums.h"
#include "LogLevels.h"

namespace temp_wrap
{

void wrap_return(const std::string&, const std::source_location&, const logger::LogLevel, const logger::Format&);

} // namespace temp_wrap
