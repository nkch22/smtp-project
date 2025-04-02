#pragma once
#include <string>
#include <source_location>
#include "Enums.h"

namespace temp_wrap
{

void wrap_return(const std::string& str, const std::source_location& m_location, const logger::LogLevels local);

} // namespace temp_wrap
