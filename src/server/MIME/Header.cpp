#include "Header.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

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

} // namespace ISXMime
