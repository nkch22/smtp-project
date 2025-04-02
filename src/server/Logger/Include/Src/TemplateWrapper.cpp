#include "LoggerWrapper.h"
#include "BufferWrapper.h"
#include "RealLogger.h"

using namespace logger_inner;

void temp_wrap::wrap_return(const std::string& str, const std::source_location& m_location,
								const logger::LogLevel local)
{
	RealLogger::get_instance()->save_to_queue({"returned: " + str}, logger::INFORMATION, m_location, local);
}

void temp_wrap::wrap_warning(const std::string& str, const std::source_location& loc)
{
	RealLogger::get_instance()->save_to_queue(str, logger::WARNING, loc, RealLogger::get_instance()->real_get_level());
}