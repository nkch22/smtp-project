#pragma once
#include <string>
#include "../Macros/LoggerMacros.h"

/*! @file MessageType.h
* @brief File, that declares MessageType and some static variables of this class
*/

namespace logger_inner
{
	/*! @class MessageType
	*	@brief Class, that responsible for message type in logs
	* 
	*	It consists of short name in logs and color of this name
	*/
class MessageType
{
private:
	std::string m_name;
	std::string m_color;

public:
	/*! @brief Copy constructor*/
	MessageType(const MessageType&);
	/*! @brief Move constructor*/
	MessageType(MessageType&&);

	/*! @brief Default constructor*/
	MessageType(const std::string& name = "", const std::string& color = "");
	/*! @brief Trivial destructor*/
	~MessageType() = default;

	/*! @brief Name setter*/
	void set_name(const std::string&);
	/*! @brief Name getter*/
	const std::string& get_name() const;

	/*! @brief Color setter*/
	void set_color(const std::string&);
	/*! @brief Color getter*/
	const std::string& get_color() const;

	/*! @brief Assignment operator*/
	void operator=(const MessageType&);
};

} // namespace logger_inner


namespace logger
{
static const logger_inner::MessageType error{" E ", ERROR_COLOR};
static const logger_inner::MessageType warning{" W ", WARNING_COLOR};
static const logger_inner::MessageType info{" I ", INFORMATION_COLOR};
}