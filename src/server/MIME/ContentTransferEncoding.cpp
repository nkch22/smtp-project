#include "ContentTransferEncoding.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "../Encoding/EncoderFactory.hpp"

namespace ISXMime
{

ContentTransferEncoding::ContentTransferEncoding(const std::string& type) : m_type(StringToType(type)) {}

void ContentTransferEncoding::SetType(const std::string& type)
{
	m_type = StringToType(type);
}

std::string ContentTransferEncoding::Str() const
{
	return TypeToString(m_type);
}

std::string ContentTransferEncoding::Encode(const std::vector<uint8_t>& data) const
{
	if (m_type == SEVEN_BIT && !IsValid7Bit(data)) return "";

	if (m_type == EIGHT_BIT && !IsValid8Bit(data)) return "";

	if (!RequiresEncoding()) return {reinterpret_cast<const char*>(data.data()), data.size()};

	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder(TypeToString(m_type));
	if (!encoder) return "";

	return encoder->Encode(data);
}

std::vector<uint8_t> ContentTransferEncoding::Decode(const std::string& encoded) const
{
	if (!RequiresEncoding()) return {encoded.begin(), encoded.end()};

	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder(TypeToString(m_type));
	if (!encoder) return {};

	return encoder->Decode(encoded);
}

bool ContentTransferEncoding::IsBinarySafe() const
{
	return m_type != SEVEN_BIT;
}

bool ContentTransferEncoding::RequiresEncoding() const
{
	return m_type == BASE64 || m_type == QUOTED_PRINTABLE;
}

bool ContentTransferEncoding::IsValid7Bit(const std::vector<uint8_t>& data)
{
	return std::ranges::all_of(data,
							   [](uint8_t c) { return (c >= 32 && c <= 126) || c == '\r' || c == '\n' || c == '\t'; });
}

bool ContentTransferEncoding::IsValid8Bit(const std::vector<uint8_t>& data)
{
	return std::ranges::all_of(data,
							   [](uint8_t c)
							   {
								   return c != 0; // 8bit allows any non-null character
							   });
}

const char* ContentTransferEncoding::TypeToString(Type type)
{
	switch (type)
	{
	case SEVEN_BIT:
		return "7bit";
	case EIGHT_BIT:
		return "8bit";
	case BINARY:
		return "binary";
	case BASE64:
		return "base64";
	case QUOTED_PRINTABLE:
		return "quoted-printable";
	default:
		return "unknown";
	}
}

ContentTransferEncoding::Type ContentTransferEncoding::StringToType(const std::string& str)
{
	std::string normalized;
	normalized.reserve(str.size());

	for (char c : str)
		if (std::isspace(c) == 0) normalized += std::tolower(c);

	if (normalized == "7bit") return SEVEN_BIT;
	if (normalized == "8bit") return EIGHT_BIT;
	if (normalized == "binary") return BINARY;
	if (normalized == "base64") return BASE64;
	if (normalized == "quoted-printable") return QUOTED_PRINTABLE;
	return UNKNOWN;
}

} // namespace ISXMime
