#pragma once

#include <string>
#include <vector>

#include "../Encoding/IEncoder.hpp"

namespace ISXMime
{

class ContentTransferEncoding
{
public:
	enum Type
	{
		SEVEN_BIT,		  // 7bit ASCII (32-126)
		EIGHT_BIT,		  // 8bit data
		BINARY,			  // Binary data
		BASE64,			  // Base64 encoding
		QUOTED_PRINTABLE, // Quoted-printable encoding
		UNKNOWN
	};

public:
	ContentTransferEncoding() : m_type(SEVEN_BIT) {}
	explicit ContentTransferEncoding(Type type) : m_type(type) {}
	explicit ContentTransferEncoding(const std::string& type);

public:
	std::string Encode(const std::vector<uint8_t>& data) const;
	std::vector<uint8_t> Decode(const std::string& encoded) const;

public:
	Type GetType() const { return m_type; }
	void SetType(Type type) { m_type = type; }
	void SetType(const std::string& type);

public:
	std::string Str() const;

	bool IsBinarySafe() const;	   
	bool RequiresEncoding() const;

private:
	static bool IsValid7Bit(const std::vector<uint8_t>& data);
	static bool IsValid8Bit(const std::vector<uint8_t>& data);

private:
	static const char* TypeToString(Type type);
	static Type StringToType(const std::string& str);

private:
	Type m_type;
};

} // namespace ISXMime
