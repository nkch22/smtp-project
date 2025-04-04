#include "BufferWrapper.h"
#include "LoggerWrapper.h"
#include "RealLogger.h"

using namespace logger_inner;

void temp_wrap::wrap_return(const std::string& str, const std::string& m_location,
							const logger::LogLevel local, const logger::Format& ft)
{
	RealLogger::get_instance()->save_to_queue({"returned: " + str}, logger::INFORMATION, m_location, local, ft);
}

void temp_wrap::wrap_warning(const std::string& str, const std::string& loc)
{
	auto buff = RealLogger::get_instance();
	buff->save_to_queue(str, logger::WARNING, loc, buff->real_get_level(), buff->real_get_level().get_format());
}