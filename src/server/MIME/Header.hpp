#pragma once

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "ContentTransferEncoding.hpp"
#include "ContentType.hpp"

namespace ISXMime
{
class Header
{
public:
	static bool CaseInsensitiveCompare(const std::string& a, const std::string& b);

public:
	Header();
	~Header();

public:
	ContentType& GetContentType();
	const ContentType& GetContentType() const;
	void SetContentType(const ContentType& ct);

public:
	ContentTransferEncoding& GetContentTransferEncoding();
	const ContentTransferEncoding& GetContentTransferEncoding() const;
	void SetContentTransferEncoding(const ContentTransferEncoding& cte);

public:
	bool HasField(const std::string& name) const;
	void AddField(const std::string& name, const std::string& value);
	std::string GetField(const std::string& name) const;

	bool HasFieldCaseInsensitive(const std::string& name) const;
	std::string GetFieldCaseInsensitive(const std::string& name) const;
	
	// Return all header fields
	const std::map<std::string, std::string>& GetAllFields() const;

public:
	std::string EncodeFieldValue(const std::string& value, const std::string& charset = "UTF-8") const;
	std::string DecodeFieldValue(const std::string& encoded_value) const;

public:
	void NormalizeFieldName(std::string& lower_name);

public:
	friend std::ostream& operator<<(std::ostream& os, const Header& header);

private:
	std::map<std::string, std::string> m_fields;
	class ContentType m_content_type;
	class ContentTransferEncoding m_content_transfer;
};
} // namespace ISXMime
