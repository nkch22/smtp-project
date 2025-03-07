#include "Body.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>

#include "ContentType.hpp"
#include "Header.hpp"
#include "MimeEntity.hpp"
#include "MimeUtils.hpp"

namespace ISXMime
{

class Body::Impl
{
public:
	Impl() = default;

	std::string content;
	std::string preamble;
	std::string epilogue;
	std::string boundary;
	std::vector<std::shared_ptr<MimeEntity>> parts;
	MultipartType multipart_type{};
	std::vector<uint8_t> binary_content;
};

namespace BodyHelpers
{
std::string MultipartTypeToString(MultipartType type)
{
	switch (type)
	{
	case MultipartType::MIXED:
		return "mixed";
	case MultipartType::ALTERNATIVE:
		return "alternative";
	case MultipartType::RELATED:
		return "related";
	case MultipartType::DIGEST:
		return "digest";
	case MultipartType::FORM:
		return "form-data";
	case MultipartType::REPORT:
		return "report";
	case MultipartType::SIGNED:
		return "signed";
	case MultipartType::ENCRYPTED:
		return "encrypted";
	case MultipartType::NONE:
	default:
		return "";
	}
}

MultipartType StringToMultipartType(const std::string& typeStr)
{
	if (typeStr == "mixed") return MultipartType::MIXED;
	if (typeStr == "alternative") return MultipartType::ALTERNATIVE;
	if (typeStr == "related") return MultipartType::RELATED;
	if (typeStr == "digest") return MultipartType::DIGEST;
	if (typeStr == "form-data") return MultipartType::FORM;
	if (typeStr == "report") return MultipartType::REPORT;
	if (typeStr == "signed") return MultipartType::SIGNED;
	if (typeStr == "encrypted") return MultipartType::ENCRYPTED;
	return MultipartType::NONE;
}

ContentType CreateContentType(MultipartType type, const std::string& boundary)
{
	std::string type_str = MultipartTypeToString(type);
	if (type_str.empty()) return ContentType::TextPlain();

	ContentType content_type("multipart", type_str);
	content_type.AddParameter("boundary", boundary);
	return content_type;
}

MultipartType ExtractMultipartType(const ContentType& contentType)
{
	if (contentType.Type() != "multipart") return MultipartType::NONE;

	return StringToMultipartType(contentType.Subtype());
}
} // namespace BodyHelpers

Body::Body() : m_p_impl(std::make_unique<Impl>()) {}
Body::~Body() = default;

Body::Body(Body&&) noexcept = default;
Body& Body::operator=(Body&&) noexcept = default;

void Body::Set(const std::string& content)
{
	m_p_impl->content = content;
}

const std::string& Body::Content() const
{
	return m_p_impl->content;
}

std::string& Body::Content()
{
	return m_p_impl->content;
}

bool Body::Load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file) return false;

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	m_p_impl->content = content;
	return true;
}

bool Body::Save(const std::string& filename) const
{
	std::ofstream file(filename, std::ios::binary);
	if (!file) return false;

	file << m_p_impl->content;
	return file.good();
}

void Body::SetPreamble(const std::string& preamble)
{
	m_p_impl->preamble = preamble;
}

const std::string& Body::Preamble() const
{
	return m_p_impl->preamble;
}

void Body::SetEpilogue(const std::string& epilogue)
{
	m_p_impl->epilogue = epilogue;
}

const std::string& Body::Epilogue() const
{
	return m_p_impl->epilogue;
}

void Body::SetBoundary(const std::string& boundary)
{
	m_p_impl->boundary = boundary;
}

const std::string& Body::Boundary() const
{
	return m_p_impl->boundary;
}

std::string Body::GenerateRandomBoundary()
{
	// random boundary
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> dist(0, 35);

	const char* charset = "abcdefghijklmnopqrstuvwxyz0123456789";
	std::string boundary = "----=_NextPart_";

	// timestamp for uniqueness
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::stringstream time_str;
	time_str << std::hex << now_c;
	boundary += time_str.str() + "_";

	// random characters
	for (int i = 0; i < 16; ++i) boundary += charset[dist(generator)];

	m_p_impl->boundary = boundary;
	return boundary;
}

std::vector<std::shared_ptr<MimeEntity>>& Body::Parts()
{
	return m_p_impl->parts;
}

const std::vector<std::shared_ptr<MimeEntity>>& Body::Parts() const
{
	return m_p_impl->parts;
}

void Body::AddPart(const std::shared_ptr<MimeEntity>& part)
{
	m_p_impl->parts.push_back(part);
}

bool Body::RemovePart(size_t index)
{
	if (index >= m_p_impl->parts.size()) return false;

	m_p_impl->parts.erase(m_p_impl->parts.begin() + index);
	return true;
}

std::shared_ptr<MimeEntity> Body::GetPart(size_t index) const
{
	if (index >= m_p_impl->parts.size()) return nullptr;

	return m_p_impl->parts[index];
}

size_t Body::PartCount() const
{
	return m_p_impl->parts.size();
}

bool Body::IsMultipart() const
{
	return m_p_impl->multipart_type != MultipartType::NONE && !m_p_impl->parts.empty();
}

std::ostream& operator<<(std::ostream& os, const Body& body)
{
	// if this is a simple (non-multipart) body - output the content
	if (!body.IsMultipart()) return os << body.Content();

	// for multipart bodies, we need to output all parts with boundaries
	if (!body.Preamble().empty()) os << body.Preamble() << "\r\n";

	// output each part with a boundary
	for (const auto& part : body.Parts())
	{
		os << "--" << body.Boundary() << "\r\n";

		std::ostringstream part_stream;
		part_stream << *part;
		os << part_stream.str() << "\r\n";
	}

	// close with the final boundary
	os << "--" << body.Boundary() << "--\r\n";

	if (!body.Epilogue().empty()) os << body.Epilogue();

	return os;
}

void Body::SetMultipartType(MultipartType type)
{
	m_p_impl->multipart_type = type;
}

MultipartType Body::GetMultipartType() const
{
	return m_p_impl->multipart_type;
}

std::shared_ptr<MimeEntity> Body::GetPartByContentType(const std::string& contentType) const
{
	for (const auto& part : m_p_impl->parts)
		if (part->GetHeader().GetContentType().Str() == contentType) return part;
	return nullptr;
}

std::shared_ptr<MimeEntity> Body::GetPartByContentId(const std::string& contentId) const
{
	for (const auto& part : m_p_impl->parts)
	{
		if (part->GetHeader().HasFieldCaseInsensitive("Content-ID"))
		{
			std::string id = part->GetHeader().GetFieldCaseInsensitive("Content-ID");
			// remove < > if present
			if (id.front() == '<' && id.back() == '>') id = id.substr(1, id.size() - 2);

			if (id == contentId) return part;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<MimeEntity>> Body::GetPartsByContentType(const std::string& contentType) const
{
	std::vector<std::shared_ptr<MimeEntity>> result;
	for (const auto& part : m_p_impl->parts)
		if (part->GetHeader().GetContentType().Str() == contentType) result.push_back(part);
	return result;
}

std::vector<std::shared_ptr<MimeEntity>> Body::FindParts(const std::function<bool(const MimeEntity&)>& predicate) const
{
	std::vector<std::shared_ptr<MimeEntity>> result;
	for (const auto& part : m_p_impl->parts)
		if (predicate(*part)) result.push_back(part);
	return result;
}

std::vector<std::shared_ptr<MimeEntity>> Body::FindPartsRecursive(
	const std::function<bool(const MimeEntity&)>& predicate) const
{
	std::vector<std::shared_ptr<MimeEntity>> result;
	for (const auto& part : m_p_impl->parts)
	{
		if (predicate(*part)) result.push_back(part);

		CollectPartsRecursive(part, predicate, result);
	}
	return result;
}

void Body::CollectPartsRecursive(const std::shared_ptr<MimeEntity>& entity,
								 const std::function<bool(const MimeEntity&)>& predicate,
								 std::vector<std::shared_ptr<MimeEntity>>& result) const
{
	// check if this entity has a multipart body
	if (entity->GetHeader().GetContentType().IsMultipart())
	{
		const auto& nested_parts = entity->GetBody().Parts();
		for (const auto& part : nested_parts)
		{
			if (predicate(*part)) result.push_back(part);

			CollectPartsRecursive(part, predicate, result);
		}
	}
}

// specialized getters for common content types
std::shared_ptr<MimeEntity> Body::GetTextPart() const
{
	// first look for text/plain specifically
	for (const auto& part : m_p_impl->parts)
	{
		const ContentType& ct = part->GetHeader().GetContentType();
		if (ct.Type() == "text" && ct.Subtype() == "plain") return part;
	}

	// if not found, accept any text/* part
	for (const auto& part : m_p_impl->parts)
		if (part->GetHeader().GetContentType().Type() == "text") return part;

	return nullptr;
}

std::shared_ptr<MimeEntity> Body::GetHtmlPart() const
{
	for (const auto& part : m_p_impl->parts)
	{
		const ContentType& ct = part->GetHeader().GetContentType();
		if (ct.Type() == "text" && ct.Subtype() == "html") return part;
	}
	return nullptr;
}

std::vector<std::shared_ptr<MimeEntity>> Body::GetAttachments() const
{
	std::vector<std::shared_ptr<MimeEntity>> result;
	for (const auto& part : m_p_impl->parts)
	{
		// check for Content-Disposition: attachment
		if (part->GetHeader().HasFieldCaseInsensitive("Content-Disposition"))
		{
			std::string disposition = part->GetHeader().GetFieldCaseInsensitive("Content-Disposition");
			if (disposition.find("attachment") != std::string::npos) result.push_back(part);
		}
		// also consider non-text parts as attachments if they don't have a Content-ID
		else if (part->GetHeader().GetContentType().Type() != "text"
				 && !part->GetHeader().HasFieldCaseInsensitive("Content-ID"))
		{
			result.push_back(part);
		}
	}
	return result;
}

std::vector<std::shared_ptr<MimeEntity>> Body::GetInlineAttachments() const
{
	std::vector<std::shared_ptr<MimeEntity>> result;
	for (const auto& part : m_p_impl->parts)
	{
		// check for Content-Disposition: inline or Content-ID presence
		bool is_inline = false;

		if (part->GetHeader().HasFieldCaseInsensitive("Content-Disposition"))
		{
			std::string disposition = part->GetHeader().GetFieldCaseInsensitive("Content-Disposition");
			if (disposition.find("inline") != std::string::npos) is_inline = true;
		}

		// presence of Content-ID also suggests inline attachment
		if (part->GetHeader().HasFieldCaseInsensitive("Content-ID")) is_inline = true;

		if (is_inline) result.push_back(part);
	}
	return result;
}

void Body::CreateMultipartBody(const std::vector<std::shared_ptr<MimeEntity>>& parts, MultipartType type)
{
	m_p_impl->parts = parts;
	m_p_impl->multipart_type = type;

	if (m_p_impl->boundary.empty()) m_p_impl->boundary = GenerateRandomBoundary();

	if (m_p_impl->preamble.empty()) m_p_impl->preamble = "This is a MIME multipart message.";

	m_p_impl->content = BuildMultipartContent();
}

std::string Body::BuildMultipartContent() const
{
	std::stringstream content;

	if (!m_p_impl->preamble.empty()) content << m_p_impl->preamble << "\r\n\r\n";

	for (const auto& part : m_p_impl->parts)
	{
		content << "--" << m_p_impl->boundary << "\r\n";
		content << *part << "\r\n";
	}

	content << "--" << m_p_impl->boundary << "--\r\n";

	if (!m_p_impl->epilogue.empty()) content << m_p_impl->epilogue << "\r\n";

	return content.str();
}

std::vector<uint8_t> Body::GetBinaryContent() const
{
	if (!m_p_impl->binary_content.empty()) return m_p_impl->binary_content;

	// convert string content to bytes if binary content is empty
	return {m_p_impl->content.begin(), m_p_impl->content.end()};
}

void Body::SetBinaryContent(const std::vector<uint8_t>& data)
{
	m_p_impl->binary_content = data;
	m_p_impl->content = std::string(data.begin(), data.end());
}

} // namespace ISXMime
