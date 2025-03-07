#pragma once

#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "ContentType.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

class MimeUtils
{
public:
	static std::string GetFileExtensionForMimeType(const ContentType& contentType);

	static ContentType GetMimeTypeForFileExtension(const std::string& extension);

	static const std::map<std::string, std::string>& GetCommonMimeTypes();

	static std::string ConvertToQuotedPrintableHeader(const std::string& text);

	static std::string GetContentTypeDescription(const ContentType& contentType);

	static std::string NormalizeHeaderFieldName(const std::string& name);

	static bool CaseInsensitiveCompare(const std::string& a, const std::string& b);

	static std::string GenerateMimeBoundary();
	static std::string EncodeFilename(const std::string& filename, const std::string& charset = "UTF-8");
	static std::string DecodeFilename(const std::string& encoded_filename);

	static std::string EncodeParameterValue(const std::string& name, const std::string& value,
											const std::string& charset = "UTF-8");

	static std::string DecodeBase64(const std::string& encoded_text);
	static std::string DecodeQuotedPrintable(const std::string& encoded_text);

private:
	static std::string GenerateRandomString(size_t length);
};

} // namespace ISXMime
