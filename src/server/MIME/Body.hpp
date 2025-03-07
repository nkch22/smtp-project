#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace ISXMime
{
class MimeEntity;
class ContentType;

enum class MultipartType
{
	NONE,
	MIXED,
	ALTERNATIVE,
	RELATED,
	DIGEST,
	FORM,
	REPORT,
	SIGNED,
	ENCRYPTED
};

class Body
{
public:
	Body();
	~Body();

	Body(const Body&) = delete;
	Body& operator=(const Body&) = delete;
	Body(Body&&) noexcept;
	Body& operator=(Body&&) noexcept;

public:
	void Set(const std::string& content);
	const std::string& Content() const;
	std::string& Content();

	bool Load(const std::string& filename);
	bool Save(const std::string& filename) const;

public:
	void SetPreamble(const std::string& preamble);
	const std::string& Preamble() const;

	void SetEpilogue(const std::string& epilogue);
	const std::string& Epilogue() const;

	void SetBoundary(const std::string& boundary);
	const std::string& Boundary() const;
	std::string GenerateRandomBoundary();

public:
	std::vector<std::shared_ptr<MimeEntity>>& Parts();
	const std::vector<std::shared_ptr<MimeEntity>>& Parts() const;

	void AddPart(const std::shared_ptr<MimeEntity>& part);
	bool RemovePart(size_t index);
	std::shared_ptr<MimeEntity> GetPart(size_t index) const;
	size_t PartCount() const;

	void SetMultipartType(MultipartType type);
	MultipartType GetMultipartType() const;
	bool IsMultipart() const;

public:
	std::shared_ptr<MimeEntity> GetPartByContentType(const std::string& contentType) const;
	std::shared_ptr<MimeEntity> GetPartByContentId(const std::string& contentId) const;
	std::vector<std::shared_ptr<MimeEntity>> GetPartsByContentType(const std::string& contentType) const;

	std::vector<std::shared_ptr<MimeEntity>> FindParts(const std::function<bool(const MimeEntity&)>& predicate) const;

	std::vector<std::shared_ptr<MimeEntity>> FindPartsRecursive(
		const std::function<bool(const MimeEntity&)>& predicate) const;

public:
	std::shared_ptr<MimeEntity> GetTextPart() const;
	std::shared_ptr<MimeEntity> GetHtmlPart() const;
	std::vector<std::shared_ptr<MimeEntity>> GetAttachments() const;
	std::vector<std::shared_ptr<MimeEntity>> GetInlineAttachments() const;

public:
	void CreateMultipartBody(const std::vector<std::shared_ptr<MimeEntity>>& parts,
							 MultipartType type = MultipartType::MIXED);

	std::vector<uint8_t> GetBinaryContent() const;
	void SetBinaryContent(const std::vector<uint8_t>& data);

public:
	friend std::ostream& operator<<(std::ostream& os, const Body& body);

private:
	void CollectPartsRecursive(const std::shared_ptr<MimeEntity>& entity,
							   const std::function<bool(const MimeEntity&)>& predicate,
							   std::vector<std::shared_ptr<MimeEntity>>& result) const;

	std::string BuildMultipartContent() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_p_impl;
};

std::ostream& operator<<(std::ostream& os, const Body& body);

namespace BodyHelpers
{
std::string MultipartTypeToString(MultipartType type);
MultipartType StringToMultipartType(const std::string& typeStr);

ContentType CreateContentType(MultipartType type, const std::string& boundary);

MultipartType ExtractMultipartType(const ContentType& contentType);
} // namespace BodyHelpers

} // namespace ISXMime
