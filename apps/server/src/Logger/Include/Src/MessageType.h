#pragma once
#include <string>
#include "../Macros/LoggerMacros.h"

namespace logger_inner
{

class MessageType
{
private:
	std::string m_name;
	std::string m_color;

public:
	MessageType(const MessageType&);
	MessageType(MessageType&&);

	MessageType(const std::string& name = "", const std::string& color = "");
	~MessageType() = default;

	void set_name(const std::string&);
	const std::string& get_name() const;

	void set_color(const std::string&);
	const std::string& get_color() const;

	void operator=(const MessageType&);
};

} // namespace logger_inner


namespace logger
{
static const logger_inner::MessageType error{" E ", ERROR_COLOR};
static const logger_inner::MessageType warning{" W ", WARNING_COLOR};
static const logger_inner::MessageType info{" I ", INFORMATION_COLOR};
}