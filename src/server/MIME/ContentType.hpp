#pragma once

#include <map>
#include <stdexcept>
#include <string>

namespace ISXMime
{

class MimeException : public std::runtime_error
{
public:
	explicit MimeException(const std::string& message) : std::runtime_error(message) {}
};

class ContentType
{
public:
	ContentType();
	ContentType(const std::string& type, const std::string& subtype);

	explicit ContentType(const std::string& fullContentType);

	~ContentType();

public:
	void SetType(const std::string& type);
	const std::string& Type() const;

	void SetSubtype(const std::string& subtype);
	const std::string& Subtype() const;

public:
	void AddParameter(const std::string& name, const std::string& value);
	std::string GetParameter(const std::string& name) const;
	bool HasParameter(const std::string& name) const;

	// Parse parameters from a string like "name=value; name2=value2"
	void ParseParameters(const std::string& paramString);

public:
	static ContentType Text();
	static ContentType TextPlain();
	static ContentType TextHtml();
	static ContentType ApplicationJson();
	static ContentType ApplicationXml();
	static ContentType MultipartMixed();
	static ContentType MultipartAlternative();
	static ContentType MultipartRelated();
	static ContentType ImageJpeg();
	static ContentType ImagePng();
	static ContentType ImageGif();

public:
	std::string Str() const;
	bool IsMultipart() const;
	bool IsValid() const;

private:
	static std::string TrimString(const std::string& str);
	static std::string GenerateRandomBoundary();

private:
	std::string m_type;
	std::string m_subtype;
	std::map<std::string, std::string> m_params;
};

} // namespace ISXMime
