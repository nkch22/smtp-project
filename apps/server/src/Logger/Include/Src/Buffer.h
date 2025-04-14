#pragma once

#include "SharedInclude.h"
#include "BufferWrapper.h"

/*!
 *	@file Buffer.h
 *	@brief Interface for logger serialization
 *
 *	This file contains serialization interface
 */

namespace logger
{
/*! @class Buffer
*	@brief Interface for logger serialization
* 
*	It is used to convert different types into std::string
*/
class Buffer
{
private:
	std::string m_real_buff;

public:
	/*! @brief Copy constructor*/
	Buffer(const Buffer&); 

	/*! @brief Default constructor*/
	Buffer(); 

	/*! @brief Default parameterized constructor*/
	Buffer(const std::string&); 

	/*! @brief Trivial destructor */
	~Buffer() = default;
	
	/*! @brief String getter*/
	std::string get() const;
	
	/*! @brief String cleaner*/
	void clear();
	
	Buffer& operator<<(const std::string&);
	Buffer& operator<<(const char*);
	Buffer& operator<<(const char);

	Buffer& operator<<(const int);
	Buffer& operator<<(const unsigned int);
	Buffer& operator<<(const double);
	Buffer& operator<<(const bool);

	/*! @brief Default overloaded serialization operator
	*	
	*	@warning If overloaded operator is not found, it will log warning
	*
	*	Supported types: std::string, char*, char, int, usigned int, double, bool
	* 
	*	To avoid warning with custom class use macros or overload operator manually
	* 
	*	@sa MAKE_LOGGABLE(Type, ...)
	*/
	template<typename T>
	Buffer& operator<<(const T&)
	{
		temp_wrap::wrap_warning("Can't find overloaded operator", FUNCTION_NAME);
		return *this;
	}

	/*! @brief Assignment operator*/
	void operator=(const Buffer&);
};

} // namespace logger
