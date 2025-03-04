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
	if (m_type == SEVEN_BIT)
	{
		if (!IsValid7Bit(data))
			throw std::runtime_error("Data contains characters that cannot be represented in 7bit encoding");

		return {reinterpret_cast<const char*>(data.data()), data.size()};
	}

	if (m_type == EIGHT_BIT)
	{
		if (!IsValid8Bit(data))
			throw std::runtime_error("Data contains null characters that cannot be represented in 8bit encoding");

		return {reinterpret_cast<const char*>(data.data()), data.size()};
	}

	if (m_type == BINARY) return {reinterpret_cast<const char*>(data.data()), data.size()};

	if (m_type == UNKNOWN) throw std::runtime_error("Cannot encode data with unknown content transfer encoding");

	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder(TypeToString(m_type));
	if (!encoder) throw std::runtime_error("Failed to create encoder for " + std::string(TypeToString(m_type)));

	return encoder->Encode(data);
}

std::vector<uint8_t> ContentTransferEncoding::Decode(const std::string& encoded) const
{
	// For 7bit, 8bit, and binary, no decoding is needed
	if (m_type == SEVEN_BIT || m_type == EIGHT_BIT || m_type == BINARY) return {encoded.begin(), encoded.end()};

	if (m_type == UNKNOWN) throw std::runtime_error("Cannot decode data with unknown content transfer encoding");

	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder(TypeToString(m_type));
	if (!encoder) throw std::runtime_error("Failed to create decoder for " + std::string(TypeToString(m_type)));

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
	return std::ranges::all_of(data, [](uint8_t c) { return c >= 1 && c <= 127; });
}

bool ContentTransferEncoding::IsValid8Bit(const std::vector<uint8_t>& data)
{
	return std::ranges::all_of(data, [](uint8_t c) { return c != 0; });
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
