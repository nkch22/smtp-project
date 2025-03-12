#pragma once
/*! @class Buffer
 *  @brief Logger class, that is used to transform variables into string
 *
 *	This class is used to transform different variables into std::stiring for futher saving by Logger
 */


#include "SharedInclude.h"

namespace logger
{

class Buffer
{
private:
	std::string* m_real_buff;

public:
	Buffer();
	/*! @fn Buffer()
	 *	@brief Default empty constructor
	 *
	 *	Calls Buffer(const std::string&) with empty string
	 * */

	Buffer(const std::string&);
	/*! @fn Buffer(const std::string&)
	 *	@brief Parameterized constructor
	 *
	 *	Copies given string in created string buffer
	 */

	~Buffer();
	/*! @fn ~Buffer()
	 *	@brief Default destructor
	 *
	 *	Deletes string buffer
	 */

	std::string get() const;
	/*! @fn get()
	 *	@brief String buffer getter
	 *
	 *	@return string buffer copy
	 */

	void clear();
	/*! @fn clear()
	 *	@brief String buffer cleaner
	 *
	 *	Clears string buffer without deletion
	 */

	Buffer& operator<<(const std::string&);
	Buffer& operator<<(const char*);
	Buffer& operator<<(const char);

	Buffer& operator<<(const int);
	Buffer& operator<<(const unsigned int);
	Buffer& operator<<(const double);
	Buffer& operator<<(const bool);

	template<typename T>
	Buffer& operator<<(const T&)
	{
		throw std::runtime_error("Can't find overloaded operator");
	}
	/*! @fn operator<<(const std::string&)
	 * @brief Default overloaded operator<< with const string&.
	 * Spesifies how buffer writes given variables into string buffer
	 *
	 *	@attention If operator<< is not found for given variable in default Buffer operators <<, it will use template
	 *	@attention operator<<(const T&).
	 *
	 *	@return Buffer
	 */
	/*! @fn operator<<(const char*)
		@brief Default overloaded operator<< with const char*
	*/
	/*! @fn operator<<(const int)
		@brief Default overloaded operator<< with const int&
	*/
	/*! @fn operator<<(const unsigned int)
		@brief Default overloaded operator<< with const unsigned int&
	*/
	/*! @fn operator<<(const double)
		@brief Default overloaded operator<< with const double&
	*/
	/*! @fn operator<<(const bool)
		@brief Default overloaded operator<< with const bool&
	*/
	/*! @fn operator<<(const T&)
	 *	@brief Default template operator<<
	 *
	 *	@attention It will be called, if none of default overloaded operators << can be used
	 *	@attention If not properly overloaded in given variable, it will throw std::exception
	 *
	 *  @important Overload example:
	 *  @important	friend Buffer& operator<<(Buffer&, const YOUR_CLASS &)
	 *
	 *	@throw std::exception;
	 *
	 *  @return Buffer
	 */
};

template<typename T>
struct RealLoggerSerializer;

template<>
struct RealLoggerSerializer<int>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const int&);
};

template<>
struct RealLoggerSerializer<double>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const double&);
};

template<>
struct RealLoggerSerializer<char>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const char&);
};

template<>
struct RealLoggerSerializer<char*>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const char*&);
};

template<>
struct RealLoggerSerializer<unsigned int>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const unsigned int&);
};

template<>
struct RealLoggerSerializer<bool>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const bool&);
};

template<>
struct RealLoggerSerializer<std::string>
{
	static logger::Buffer& serialize(logger::Buffer& buff, const std::string&);
};

template<typename T>
struct LoggerSerializer
{
	static Buffer& serialize(Buffer& buff, const T& obj) { return RealLoggerSerializer<T>::serialize(buff, obj); }
};

} // namespace logger