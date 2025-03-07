#pragma once

#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Body.hpp"
#include "ContentType.hpp"
#include "Header.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

class MimeParserError : public MimeException
{
public:
	explicit MimeParserError(const std::string& message) : MimeException(message) {}
};

class MimeParserConfig;

class MimeParser
{
public:
	MimeParser();
	explicit MimeParser(const MimeParserConfig& config);
	~MimeParser();

public:
	std::shared_ptr<MimeEntity> ParseString(const std::string& mimeContent);
	std::shared_ptr<MimeEntity> ParseStream(std::istream& stream);
	std::shared_ptr<MimeEntity> ParseFile(const std::string& filePath);

public:
	std::vector<std::shared_ptr<MimeEntity>> ParseMultipart(const MimeEntity& entity);
	std::shared_ptr<MimeEntity> ParsePart(const std::string& part, const std::string& boundary);

	Header ParseHeader(std::istream& stream);
	std::optional<std::pair<std::string, std::string>> ParseHeaderField(const std::string& line);

	Body ParseBody(std::istream& stream, const Header& header);
	Body ParseBodyWithBoundary(std::istream& stream, const std::string& boundary);

public:
	void SetConfig(const MimeParserConfig& config);
	const MimeParserConfig& GetConfig() const;

public:
	std::string DecodeHeaderField(const std::string& field) const;
	std::vector<uint8_t> DecodeBody(const std::string& body, const std::string& encoding) const;

private:
	std::string ReadLine(std::istream& stream) const;
	std::string ReadUntilBoundary(std::istream& stream, const std::string& boundary) const;
	bool IsEndBoundary(const std::string& line, const std::string& boundary) const;
	bool IsBoundary(const std::string& line, const std::string& boundary) const;
	std::string ExtractBoundary(const ContentType& contentType) const;

private:
	class Impl;
	std::unique_ptr<Impl> m_p_impl;
};

class MimeParserConfig
{
public:
	MimeParserConfig();

	void SetStrictMode(bool strict);
	bool IsStrictMode() const;

	void SetMaxHeaderSize(size_t size);
	size_t GetMaxHeaderSize() const;

	void SetMaxLineLength(size_t length);
	size_t GetMaxLineLength() const;

	void SetMaxPartCount(size_t count);
	size_t GetMaxPartCount() const;

	void SetRecursionLimit(size_t limit);
	size_t GetRecursionLimit() const;

	void SetIgnoreInvalidHeaders(bool ignore);
	bool ShouldIgnoreInvalidHeaders() const;

	static MimeParserConfig CreateDefault();

private:
	bool m_strict_mode;
	size_t m_max_header_size;
	size_t m_max_line_length;
	size_t m_max_part_count;
	size_t m_recursion_limit;
	bool m_ignore_invalid_headers;
};

std::shared_ptr<MimeParser> CreateMimeParser();

} // namespace ISXMime
