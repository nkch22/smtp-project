#include "ContentType.hpp"

#include <algorithm>
#include <ctime>
#include <random>
#include <ranges>
#include <sstream>

namespace ISXMime
{

ContentType ContentType::Text()
{
	return ContentType("text", "*");
}

ContentType ContentType::TextPlain()
{
	return ContentType("text", "plain");
}

ContentType ContentType::TextHtml()
{
	return ContentType("text", "html");
}

ContentType ContentType::ApplicationJson()
{
	return ContentType("application", "json");
}

ContentType ContentType::ApplicationXml()
{
	return ContentType("application", "xml");
}

ContentType ContentType::MultipartMixed()
{
	ContentType ct("multipart", "mixed");
	// mixed types have boundary by default
	ct.AddParameter("boundary", GenerateRandomBoundary());
	return ct;
}

ContentType ContentType::MultipartAlternative()
{
	ContentType ct("multipart", "alternative");
	ct.AddParameter("boundary", GenerateRandomBoundary());
	return ct;
}

ContentType ContentType::MultipartRelated()
{
	ContentType ct("multipart", "related");
	ct.AddParameter("boundary", GenerateRandomBoundary());
	return ct;
}

ContentType ContentType::ImageJpeg()
{
	return ContentType("image", "jpeg");
}

ContentType ContentType::ImagePng()
{
	return ContentType("image", "png");
}

ContentType ContentType::ImageGif()
{
	return ContentType("image", "gif");
}

ContentType::ContentType() : m_type("text"), m_subtype("plain") {}

ContentType::ContentType(const std::string& type, const std::string& subtype) : m_type(type), m_subtype(subtype) {}

ContentType::ContentType(const std::string& fullContentType)
{
	// parse a content type string like "text/plain; charset=utf-8"
	std::string::size_type pos = fullContentType.find('/');
	if (pos != std::string::npos)
	{
		m_type = fullContentType.substr(0, pos);

		// find where the parameters start
		std::string::size_type param_pos = fullContentType.find(';', pos);
		if (param_pos != std::string::npos)
		{
			// extract subtype (between '/' and ';')
			m_subtype = fullContentType.substr(pos + 1, param_pos - pos - 1);

			std::string params = fullContentType.substr(param_pos + 1);
			ParseParameters(params);
		}
		else
		{
			// no parameters, just extract subtype (everything after '/')
			m_subtype = fullContentType.substr(pos + 1);
		}
	}
	else
	{
		// invalid format
		m_type = "text";
		m_subtype = "plain";
	}

	m_type = TrimString(m_type);
	m_subtype = TrimString(m_subtype);
}

ContentType::~ContentType() = default;

void ContentType::SetType(const std::string& type)
{
	m_type = type;
}

const std::string& ContentType::Type() const
{
	return m_type;
}

void ContentType::SetSubtype(const std::string& subtype)
{
	m_subtype = subtype;
}

const std::string& ContentType::Subtype() const
{
	return m_subtype;
}

void ContentType::AddParameter(const std::string& name, const std::string& value)
{
	m_params[name] = value;
}

std::string ContentType::GetParameter(const std::string& name) const
{
	auto it = m_params.find(name);
	if (it != m_params.end()) return it->second;

	return "";
}

bool ContentType::HasParameter(const std::string& name) const
{
	return m_params.find(name) != m_params.end();
}

void ContentType::ParseParameters(const std::string& paramString)
{
	std::string::size_type start = 0;
	std::string::size_type end;

	// process each "name=value" pair separated by semicolons
	while ((end = paramString.find(';', start)) != std::string::npos || start < paramString.length())
	{
		// get the current parameter pair
		std::string pair;
		if (end != std::string::npos)
		{
			pair = paramString.substr(start, end - start);
			start = end + 1;
		}
		else
		{
			pair = paramString.substr(start);
			start = paramString.length();
		}

		// split the pair into name and value
		std::string::size_type equals_pos = pair.find('=');
		if (equals_pos != std::string::npos)
		{
			std::string name = pair.substr(0, equals_pos);
			std::string value = pair.substr(equals_pos + 1);

			name = TrimString(name);
			value = TrimString(value);

			// remove quotes if present
			if (value.length() >= 2 && value.front() == '"' && value.back() == '"')
				value = value.substr(1, value.length() - 2);

			if (!name.empty()) AddParameter(name, value);
		}

		if (start >= paramString.length()) break;
	}
}

std::string ContentType::Str() const
{
	std::ostringstream oss;
	oss << m_type << "/" << m_subtype;

	for (const auto& param : m_params)
	{
		oss << "; " << param.first << "=";

		// check if we need to quote the value
		bool need_quotes = false;
		for (char c : param.second)
		{
			if ((std::isalnum(c) == 0) && c != '-' && c != '_' && c != '.')
			{
				need_quotes = true;
				break;
			}
		}

		if (need_quotes)
			oss << "\"" << param.second << "\"";
		else
			oss << param.second;
	}

	return oss.str();
}

bool ContentType::IsMultipart() const
{
	return m_type == "multipart";
}

bool ContentType::IsValid() const
{
	// type and subtype cant be empty
	if (m_type.empty() || m_subtype.empty()) return false;

	// check type for valid characters (letters, digits, -, .)
	for (char c : m_type)
	{
		if ((std::isalnum(c) == 0) && c != '-' && c != '.') return false;
	}

	// check subtype for valid characters
	for (char c : m_subtype)
	{
		if ((std::isalnum(c) == 0) && c != '-' && c != '.') return false;
	}

	return true;
}

std::string ContentType::TrimString(const std::string& str)
{
	auto wsfront = std::ranges::find_if_not(str, [](int c) { return std::isspace(c); });
	auto wsback =
		std::ranges::find_if_not(std::ranges::reverse_view(str), [](int c) { return std::isspace(c); }).base();

	return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

std::string ContentType::GenerateRandomBoundary()
{
	static const char CHARSET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	static std::mt19937 rng(std::time(nullptr));
	std::uniform_int_distribution<> dist(0, sizeof(CHARSET) - 2);

	// format: "----=_NextPart_" + 16 random chars
	std::string boundary = "----=_NextPart_";

	for (int i = 0; i < 16; ++i) boundary += CHARSET[dist(rng)];

	return boundary;
}

} // namespace ISXMime
