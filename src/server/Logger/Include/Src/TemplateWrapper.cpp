#include "TemplateWrapper.h"


void temp_wrap::log_return(const std::string& str, const std::source_location& m_location,
								const logger::LogLevels local)
{
	RealLogger::get_instance()->save_to_queue({"returned: " + str}, logger::INFORMATION, m_location, local);
}
