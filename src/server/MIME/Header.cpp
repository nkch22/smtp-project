#include "Header.hpp"

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>

#include "../Encoding/EncoderFactory.hpp"

namespace ISXMime
{

Header::Header() : m_content_type(ContentType::TextPlain()), m_content_transfer(ContentTransferEncoding::SEVEN_BIT) {}

Header::~Header() = default;

ContentType& Header::GetContentType()
{
	return m_content_type;
}

const ContentType& Header::GetContentType() const
{
	return m_content_type;
}

void Header::SetContentType(const ContentType& ct)
{
	m_content_type = ct;
	AddField("Content-Type", ct.Str());
}

ContentTransferEncoding& Header::GetContentTransferEncoding()
{
	return m_content_transfer;
}

const ContentTransferEncoding& Header::GetContentTransferEncoding() const
{
	return m_content_transfer;
}

void Header::SetContentTransferEncoding(const ContentTransferEncoding& cte)
{
	m_content_transfer = cte;
	AddField("Content-Transfer-Encoding", cte.Str());
}

bool Header::HasField(const std::string& name) const
{
	return m_fields.find(name) != m_fields.end();
}

bool Header::HasFieldCaseInsensitive(const std::string& name) const
{
	std::string lower_name = name;
	std::ranges::transform(lower_name, lower_name.begin(), [](unsigned char c) { return std::tolower(c); });

	for (const auto& field : m_fields)
	{
		std::string field_name_lower = field.first;
		std::ranges::transform(field_name_lower, field_name_lower.begin(),
							   [](unsigned char c) { return std::tolower(c); });

		if (field_name_lower == lower_name) return true;
	}

	return false;
}

void Header::AddField(const std::string& name, const std::string& value)
{
	if (name.empty()) return;

	m_fields[name] = value;

	if (Header::CaseInsensitiveCompare(name, "Content-Type"))
		m_content_type = ContentType(value);

	else if (Header::CaseInsensitiveCompare(name, "Content-Transfer-Encoding"))
		m_content_transfer = ContentTransferEncoding(value);
}

std::string Header::GetField(const std::string& name) const
{
	auto it = m_fields.find(name);
	if (it != m_fields.end()) return it->second;

	return "";
}

std::string Header::GetFieldCaseInsensitive(const std::string& name) const
{
	std::string lower_name = name;
	std::ranges::transform(lower_name, lower_name.begin(), [](unsigned char c) { return std::tolower(c); });

	for (const auto& field : m_fields)
	{
		std::string field_name_lower = field.first;
		std::ranges::transform(field_name_lower, field_name_lower.begin(),
							   [](unsigned char c) { return std::tolower(c); });

		if (field_name_lower == lower_name) return field.second;
	}

	return "";
}

bool Header::CaseInsensitiveCompare(const std::string& a, const std::string& b)
{
	if (a.size() != b.size()) return false;

	return std::equal(a.begin(), a.end(), b.begin(),
					  [](unsigned char a, unsigned char b) { return std::tolower(a) == std::tolower(b); });
}

std::ostream& operator<<(std::ostream& os, const Header& header)
{
	// output all header fields in the format "Field-Name: value\r\n"
	for (const auto& field : header.m_fields) os << field.first << ": " << field.second << "\r\n";

	// end of headers is indicated by an empty line
	os << "\r\n";

	return os;
}

std::string Header::EncodeFieldValue(const std::string& value, const std::string& charset) const
{
	// check if encoding is needed - only encode if there are non-ASCII characters
	bool needs_encoding = false;
	for (unsigned char c : value)
	{
		if (c == 0 || c > 127 || c == '=')
		{
			needs_encoding = true;
			break;
		}
	}

	if (!needs_encoding)
	{
		return value;
	}

	size_t non_ascii_count = 0;
	for (unsigned char c : value)
		if (c > 127) non_ascii_count++;

	// if more than 30% of characters are non-ASCII, use Base64
	bool use_base64 = (non_ascii_count > value.size() * 0.3);

	std::string encoding_type = use_base64 ? "B" : "Q";
	std::string encoded;

	if (use_base64)
	{
		// Base64 encoding
		auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
		std::vector<uint8_t> data(value.begin(), value.end());
		encoded = encoder->Encode(data);

		// Remove any line breaks that base64 encoder might have added
		encoded.erase(std::remove(encoded.begin(), encoded.end(), '\r'), encoded.end());
		encoded.erase(std::remove(encoded.begin(), encoded.end(), '\n'), encoded.end());
	}
	else
	{
		// Q-encoding (similar to quoted-printable but with spaces encoded as '_')
		auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
		std::vector<uint8_t> data(value.begin(), value.end());
		encoded = encoder->Encode(data);

		size_t pos = 0;
		while ((pos = encoded.find("=\r\n", pos)) != std::string::npos) encoded.erase(pos, 3);

		// spaces are encoded as underscore (not as =20)
		pos = 0;
		while ((pos = encoded.find("=20", pos)) != std::string::npos) encoded.replace(pos, 3, "_");
	}

	// =?charset?encoding?encoded-text?=
	return "=?" + charset + "?" + encoding_type + "?" + encoded + "?=";
}

std::string Header::DecodeFieldValue(const std::string& encoded_value) const
{
	std::regex encoded_word_regex("=\\?([^?]*)\\?([bqBQ])\\?([^?]*)\\?=");
	std::string result = encoded_value;

	// Find and process all encoded-word sections
	std::string::const_iterator search_start(result.cbegin());
	std::smatch match;

	while (std::regex_search(search_start, result.cend(), match, encoded_word_regex))
	{
		// Get the offset from the beginning of the string
		size_t match_pos = std::distance(result.cbegin(), match[0].first);
		size_t match_length = match[0].length();

		std::string charset = match[1].str();
		char encoding_type = std::toupper(match[2].str()[0]);
		std::string encoded_text = match[3].str();

		std::string decoded;
		if (encoding_type == 'B')
		{
			// Base64 encoding
			auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
			auto data = encoder->Decode(encoded_text);
			decoded = std::string(data.begin(), data.end());
		}
		else if (encoding_type == 'Q')
		{
			// Q-encoding (variant of quoted-printable)
			// convert underscore back to space
			std::replace(encoded_text.begin(), encoded_text.end(), '_', ' ');

			auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
			auto data = encoder->Decode(encoded_text);
			decoded = std::string(data.begin(), data.end());
		}

		result.replace(match_pos, match_length, decoded);

		search_start = result.cbegin() + match_pos + decoded.length();

		if (search_start >= result.cend()) break;
	}

	return result;
}

const std::map<std::string, std::string>& Header::GetAllFields() const
{
	return m_fields;
}

} // namespace ISXMime
