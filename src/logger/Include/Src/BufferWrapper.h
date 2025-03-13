#pragma once
#include <string>
#include <source_location>
#include "Enums.h"

namespace temp_wrap
{

void wrap_warning(const std::string&, const std::source_location&);

} // namespace temp_wrap
