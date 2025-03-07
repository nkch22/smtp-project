#include "MimeParser.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stack>

#include "MimeUtils.hpp"
#include "server/Encoding/EncoderFactory.hpp"

namespace ISXMime
{

class MimeParser::Impl
{
public:
	MimeParserConfig config;
	size_t current_recursion_depth = 0;
};

MimeParserConfig::MimeParserConfig() :
	m_strict_mode(false), m_max_header_size(static_cast<long>(16) * 1024), // 16KB
	m_max_line_length(static_cast<long>(8) * 1024),						   // 8KB
	m_max_part_count(100), m_recursion_limit(20), m_ignore_invalid_headers(true)
{
}

void MimeParserConfig::SetStrictMode(bool strict)
{
	m_strict_mode = strict;
}

bool MimeParserConfig::IsStrictMode() const
{
	return m_strict_mode;
}

void MimeParserConfig::SetMaxHeaderSize(size_t size)
{
	m_max_header_size = size;
}

size_t MimeParserConfig::GetMaxHeaderSize() const
{
	return m_max_header_size;
}

void MimeParserConfig::SetMaxLineLength(size_t length)
{
	m_max_line_length = length;
}

size_t MimeParserConfig::GetMaxLineLength() const
{
	return m_max_line_length;
}

void MimeParserConfig::SetMaxPartCount(size_t count)
{
	m_max_part_count = count;
}

size_t MimeParserConfig::GetMaxPartCount() const
{
	return m_max_part_count;
}

void MimeParserConfig::SetRecursionLimit(size_t limit)
{
	m_recursion_limit = limit;
}

size_t MimeParserConfig::GetRecursionLimit() const
{
	return m_recursion_limit;
}

void MimeParserConfig::SetIgnoreInvalidHeaders(bool ignore)
{
	m_ignore_invalid_headers = ignore;
}

bool MimeParserConfig::ShouldIgnoreInvalidHeaders() const
{
	return m_ignore_invalid_headers;
}

MimeParserConfig MimeParserConfig::CreateDefault()
{
	return {};
}

MimeParser::MimeParser() : m_p_impl(std::make_unique<Impl>())
{
	m_p_impl->config = MimeParserConfig::CreateDefault();
}

MimeParser::MimeParser(const MimeParserConfig& config) : m_p_impl(std::make_unique<Impl>())
{
	m_p_impl->config = config;
}

MimeParser::~MimeParser() = default;

void MimeParser::SetConfig(const MimeParserConfig& config)
{
	m_p_impl->config = config;
}

const MimeParserConfig& MimeParser::GetConfig() const
{
	return m_p_impl->config;
}

std::shared_ptr<MimeEntity> MimeParser::ParseString(const std::string& mimeContent)
{
	std::istringstream stream(mimeContent);
	return ParseStream(stream);
}

std::shared_ptr<MimeEntity> MimeParser::ParseFile(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) throw MimeParserError("Failed to open file: " + filePath);

	return ParseStream(file);
}

std::shared_ptr<MimeEntity> MimeParser::ParseStream(std::istream& stream)
{
	try
	{
		auto entity = std::make_shared<MimeEntity>();

		Header header = ParseHeader(stream);
		entity->GetHeader() = header;

		std::string body_content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

		Body body;
		body.Set(body_content);
		entity->GetBody().Set(body_content);

		std::cout << "ParseStream read body content (" << body_content.size() << " bytes):\n";
		if (body_content.size() < 100)
		{
			std::cout << "'" << body_content << "'" << std::endl;
		}
		else
		{
			std::cout << "'" << body_content.substr(0, 100) << "..." << body_content.substr(body_content.size() - 20)
					  << "'" << std::endl;
		}

		return entity;
	}
	catch (const std::exception& e)
	{
		throw MimeParserError(std::string("Error while parsing MIME content: ") + e.what());
	}
}

Header MimeParser::ParseHeader(std::istream& stream)
{
	Header header;
	std::string line;
	std::string current_field;
	size_t header_size = 0;

	while (std::getline(stream, line))
	{
		// CRLF to LF conversion
		if (!line.empty() && line.back() == '\r') line.pop_back();

		// size constraints
		header_size += line.size() + 2; // +2 for CRLF
		if (header_size > m_p_impl->config.GetMaxHeaderSize())
			throw MimeParserError("Header size exceeds maximum allowed size");

		// empty line indicates end of headers
		if (line.empty()) break;

		// folded header (starts with whitespace)
		if (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
		{
			if (!current_field.empty())
				current_field += " " + line.substr(1);
			else if (m_p_impl->config.IsStrictMode())
				throw MimeParserError("Invalid header folding: no current header field");
		}
		else
		{
			if (!current_field.empty())
			{
				auto field_pair = ParseHeaderField(current_field);
				if (field_pair.has_value()) header.AddField(field_pair->first, field_pair->second);

				current_field.clear();
			}

			current_field = line;
		}
	}

	if (!current_field.empty())
	{
		auto field_pair = ParseHeaderField(current_field);
		if (field_pair.has_value()) header.AddField(field_pair->first, field_pair->second);
	}

	return header;
}

std::optional<std::pair<std::string, std::string>> MimeParser::ParseHeaderField(const std::string& line)
{
	size_t colon_pos = line.find(':');
	if (colon_pos == std::string::npos)
	{
		if (m_p_impl->config.ShouldIgnoreInvalidHeaders()) return std::nullopt;
		throw MimeParserError("Invalid header field: missing colon");
	}

	// split into name and value
	std::string name = line.substr(0, colon_pos);
	std::string value;

	if (colon_pos + 1 < line.size())
	{
		value = line.substr(colon_pos + 1);

		value.erase(0, value.find_first_not_of(" \t"));
		value.erase(value.find_last_not_of(" \t") + 1);
	}

	if (MimeUtils::CaseInsensitiveCompare(name, "Content-Type"))
	{
		ContentType ct(value);
		return std::make_pair(name, value);
	}
	if (MimeUtils::CaseInsensitiveCompare(name, "Content-Transfer-Encoding"))
	{
		ContentTransferEncoding cte(value);
		return std::make_pair(name, value);
	}

	return std::make_pair(name, value);
}

Body MimeParser::ParseBody(std::istream& stream, const Header& header)
{
	Body body;

	const ContentType& content_type = header.GetContentType();

	if (content_type.IsMultipart())
	{
		std::string boundary = ExtractBoundary(content_type);
		if (boundary.empty()) throw MimeParserError("Multipart content missing boundary parameter");

		body = ParseBodyWithBoundary(stream, boundary);
	}
	else
	{
		std::string content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

		const ContentTransferEncoding& encoding = header.GetContentTransferEncoding();

		std::string encoding_mechanism = encoding.Str();
		if (encoding_mechanism != "7bit" && encoding_mechanism != "8bit" && encoding_mechanism != "binary")
		{
			if (encoding_mechanism == "quoted-printable")
			{
				std::string decoded = MimeUtils::DecodeQuotedPrintable(content);
				body.Set(decoded);
			}
			else if (encoding_mechanism == "base64")
			{
				std::string decoded = MimeUtils::DecodeBase64(content);
				body.Set(decoded);
			}
			else
			{
				std::vector<uint8_t> decoded = DecodeBody(content, encoding_mechanism);
				body.Set(std::string(decoded.begin(), decoded.end()));
			}
		}
		else
		{
			body.Set(content);
		}
	}

	return body;
}

Body MimeParser::ParseBodyWithBoundary(std::istream& stream, const std::string& boundary)
{
	std::string delimiter = "--" + boundary;
	std::string end_delimiter = delimiter + "--";
	std::string line;
	std::string part_content;
	std::vector<std::shared_ptr<MimeEntity>> parts;
	size_t part_count = 0;
	bool found_boundary = false;
	std::string preamble;

	// read preamble up to the first boundary
	std::stringstream preamble_stream;
	while (std::getline(stream, line))
	{
		// remove any trailing CR
		if (!line.empty() && line.back() == '\r') line.pop_back();

		if (line == delimiter)
		{
			found_boundary = true;
			break;
		}
		preamble_stream << line << "\r\n";
	}

	preamble = preamble_stream.str();

	if (!found_boundary)
	{
		stream.clear();
		stream.seekg(0);

		std::string full_content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

		size_t boundary_pos = full_content.find(delimiter);
		if (boundary_pos != std::string::npos)
		{
			found_boundary = true;

			preamble = full_content.substr(0, boundary_pos);

			stream.clear();
			stream.seekg(0);

			std::string skip_content = full_content.substr(0, boundary_pos);
			stream.ignore(skip_content.length());

			std::getline(stream, line);
		}
		else
		{
			throw MimeParserError("Could not find start boundary");
		}
	}

	std::string epilogue;
	bool found_end_boundary = false;
	while (std::getline(stream, line))
	{
		// CRLF to LF conversion
		if (!line.empty() && line.back() == '\r') line.pop_back();

		if (line == end_delimiter)
		{
			// end of multipart content
			if (!part_content.empty())
			{
				auto part = ParsePart(part_content, boundary);
				if (part)
				{
					parts.push_back(part);
					part_count++;

					if (part_count > m_p_impl->config.GetMaxPartCount())
					{
						if (m_p_impl->config.IsStrictMode()) throw MimeParserError("Maximum part count exceeded");
						break;
					}
				}
				part_content.clear();
			}
			found_end_boundary = true;
			break;
		}

		if (line == delimiter)
		{
			if (!part_content.empty())
			{
				auto part = ParsePart(part_content, boundary);
				if (part)
				{
					parts.push_back(part);
					part_count++;

					if (part_count > m_p_impl->config.GetMaxPartCount())
					{
						if (m_p_impl->config.IsStrictMode()) throw MimeParserError("Maximum part count exceeded");
						break;
					}
				}
				part_content.clear();
			}
		}
		else
		{
			part_content += line + "\r\n";
		}
	}

	if (!part_content.empty())
	{
		auto part = ParsePart(part_content, boundary);
		if (part) parts.push_back(part);
	}

	// read epilogue if we found the end boundary
	if (found_end_boundary)
	{
		std::stringstream epilogue_stream;
		while (std::getline(stream, line))
		{
			// CRLF to LF conversion
			if (!line.empty() && line.back() == '\r') line.pop_back();
			epilogue_stream << line << "\r\n";
		}
		epilogue = epilogue_stream.str();
	}

	Body body;

	MultipartType multipart_type = MultipartType::MIXED;

	body.SetBoundary(boundary);
	if (!preamble.empty()) body.SetPreamble(preamble);
	if (!epilogue.empty()) body.SetEpilogue(epilogue);

	for (const auto& part : parts) body.AddPart(part);

	body.SetMultipartType(multipart_type);

	std::stringstream body_stream;

	if (!preamble.empty()) body_stream << preamble;

	for (const auto& part : parts)
	{
		body_stream << "--" << boundary << "\r\n";
		body_stream << *part << "\r\n";
	}

	body_stream << "--" << boundary << "--\r\n";

	if (!epilogue.empty()) body_stream << epilogue;

	body.Set(body_stream.str());

	return body;
}

std::shared_ptr<MimeEntity> MimeParser::ParsePart(const std::string& part, const std::string& /*boundary*/)
{
	std::istringstream part_stream(part);
	return ParseStream(part_stream);
}

std::vector<std::shared_ptr<MimeEntity>> MimeParser::ParseMultipart(const MimeEntity& entity)
{
	std::vector<std::shared_ptr<MimeEntity>> parts;

	const ContentType& content_type = entity.GetHeader().GetContentType();
	if (!content_type.IsMultipart()) return parts;

	if (m_p_impl->current_recursion_depth >= m_p_impl->config.GetRecursionLimit())
	{
		if (m_p_impl->config.IsStrictMode()) throw MimeParserError("Maximum recursion depth exceeded");
		return parts;
	}

	std::string boundary = ExtractBoundary(content_type);
	if (boundary.empty()) throw MimeParserError("Multipart content missing boundary parameter");

	std::string body_content = entity.GetBody().Content();
	std::istringstream body_stream(body_content);

	m_p_impl->current_recursion_depth++;

	try
	{
		// MIME boundaries are prefixed with -- in the content
		std::string delimiter = "--" + boundary;
		std::string end_delimiter = delimiter + "--";

		// skip preamble until first boundary
		std::string line;
		std::string part_content;
		bool found_boundary = false;

		while (std::getline(body_stream, line))
		{
			// remove any trailing CR
			if (!line.empty() && line.back() == '\r') line.pop_back();

			// check for the boundary with exact comparison
			if (line == delimiter)
			{
				found_boundary = true;
				std::cout << "Found boundary!" << std::endl;
				break;
			}
		}

		if (!found_boundary)
		{
			body_stream.clear();
			body_stream.seekg(0);

			std::string full_content((std::istreambuf_iterator<char>(body_stream)), std::istreambuf_iterator<char>());

			size_t boundary_pos = full_content.find(delimiter);
			if (boundary_pos != std::string::npos)
			{
				std::cout << "Found boundary at position " << boundary_pos << std::endl;
				found_boundary = true;

				body_stream.clear();
				body_stream.seekg(0);

				std::string skip_content = full_content.substr(0, boundary_pos);
				body_stream.ignore(skip_content.length());

				std::getline(body_stream, line);
			}
			else
			{
				throw MimeParserError("Could not find start boundary");
			}
		}

		while (std::getline(body_stream, line))
		{
			// handle CRLF to LF conversion
			if (!line.empty() && line.back() == '\r') line.pop_back();

			if (line == end_delimiter)
			{
				if (!part_content.empty())
				{
					auto part = ParsePart(part_content, boundary);
					if (part) parts.push_back(part);
				}
				break;
			}

			if (line == delimiter)
			{
				// new part
				if (!part_content.empty())
				{
					auto part = ParsePart(part_content, boundary);
					if (part) parts.push_back(part);
					part_content.clear();
				}
			}
			else
			{
				part_content += line + "\r\n";
			}
		}
	}
	catch (const std::exception& e)
	{
		m_p_impl->current_recursion_depth--;
		throw MimeParserError(std::string("Error parsing multipart: ") + e.what());
	}

	m_p_impl->current_recursion_depth--;

	std::cout << "Found " << parts.size() << " parts in multipart content" << std::endl;
	return parts;
}

std::string MimeParser::ExtractBoundary(const ContentType& contentType) const
{
	if (contentType.HasParameter("boundary"))
	{
		return contentType.GetParameter("boundary");
	}
	return "";
}

std::string MimeParser::ReadLine(std::istream& stream) const
{
	std::string line;
	if (!std::getline(stream, line)) return "";

	if (!line.empty() && line.back() == '\r') line.pop_back();

	return line;
}

bool MimeParser::IsBoundary(const std::string& line, const std::string& boundary) const
{
	return line == ("--" + boundary);
}

bool MimeParser::IsEndBoundary(const std::string& line, const std::string& boundary) const
{
	return line == ("--" + boundary + "--");
}

std::string MimeParser::DecodeHeaderField(const std::string& field) const
{
	static const std::regex ENCODED_WORD_REGEX(R"(=\?([^?]+)\?([qQbB])\?([^?]*)\?=)", std::regex_constants::ECMAScript);

	std::string result = field;
	std::smatch matches;

	std::string::const_iterator search_start(field.cbegin());
	while (std::regex_search(search_start, field.cend(), matches, ENCODED_WORD_REGEX))
	{
		std::string charset = matches[1].str();
		char encoding = std::toupper(matches[2].str()[0]);
		std::string encoded_text = matches[3].str();
		std::string decoded_text;

		if (encoding == 'Q')
		{
			auto mime_utils = std::make_shared<MimeUtils>();
			decoded_text = ISXMime::MimeUtils::DecodeQuotedPrintable(encoded_text);
		}
		else if (encoding == 'B')
		{
			auto mime_utils = std::make_shared<MimeUtils>();
			decoded_text = ISXMime::MimeUtils::DecodeBase64(encoded_text);
		}

		size_t start_pos = matches[0].first - field.cbegin();
		size_t length = matches[0].length();
		result.replace(start_pos, length, decoded_text);

		search_start = matches[0].second;
	}

	return result;
}

std::vector<uint8_t> MimeParser::DecodeBody(const std::string& body, const std::string& encoding) const
{
	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder(encoding);
	if (!encoder) throw MimeParserError("Unsupported encoding mechanism: " + encoding);

	return encoder->Decode(body);
}

std::shared_ptr<MimeParser> CreateMimeParser()
{
	return std::make_shared<MimeParser>();
}

} // namespace ISXMime
