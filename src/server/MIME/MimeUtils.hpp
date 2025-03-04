#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ContentType.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

class MimeUtils
{
public:
	static std::string GenerateRandomBoundary();

	static std::string GetFileExtensionForMimeType(const ContentType& contentType);

	static ContentType GetMimeTypeForFileExtension(const std::string& extension);

	static const std::map<std::string, std::string>& GetCommonMimeTypes();

	static std::string ConvertToQuotedPrintableHeader(const std::string& text);

	static std::shared_ptr<MimeEntity> CreateBasicEmail(const std::string& from, const std::string& to,
														const std::string& subject, const std::string& body,
														bool isHtml = false);

	static std::shared_ptr<MimeEntity> CreateMultipartEmail(const std::string& from, const std::string& to,
															const std::string& subject, const std::string& textBody,
															const std::string& htmlBody);

	static std::string GetContentTypeDescription(const ContentType& contentType);

	static std::string NormalizeHeaderFieldName(const std::string& name);

	static bool CaseInsensitiveCompare(const std::string& a, const std::string& b);
};

} // namespace ISXMime
