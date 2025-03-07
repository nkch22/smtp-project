#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "ContentType.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

enum class EmailType
{
	SIMPLE,		 // Single part (text or html only)
	ALTERNATIVE, // Multipart/alternative (text and html versions)
	MIXED,		 // Multipart/mixed (with attachments)
	RELATED		 // Multipart/related (with inline attachments)
};

class MimeBuilder
{
public:
	MimeBuilder& From(const std::string& from);
	MimeBuilder& To(const std::string& to);
	MimeBuilder& Subject(const std::string& subject);
	MimeBuilder& Cc(const std::string& cc);
	MimeBuilder& Bcc(const std::string& bcc);
	MimeBuilder& ReplyTo(const std::string& replyTo);
	MimeBuilder& AddCustomHeader(const std::string& name, const std::string& value);

public:
	MimeBuilder& TextBody(const std::string& text, const std::string& charset = "UTF-8");
	MimeBuilder& HtmlBody(const std::string& html, const std::string& charset = "UTF-8");

public:
	MimeBuilder& AddAttachment(const std::string& filePath);
	MimeBuilder& AddAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
							   const std::string& mimeType = "application/octet-stream");
	MimeBuilder& AddInlineAttachment(const std::string& filePath, const std::string& contentId);
	MimeBuilder& AddInlineAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
									 const std::string& contentId,
									 const std::string& mimeType = "application/octet-stream");

public:
	MimeBuilder& SetEmailType(EmailType type);
	EmailType GetEmailType() const;

public:
	std::shared_ptr<MimeEntity> Build();
	std::string ToString();
	bool SaveToFile(const std::string& filePath);

public:
	static MimeBuilder Create();

	static MimeBuilder CreateTextEmail();
	static MimeBuilder CreateHtmlEmail();
	static MimeBuilder CreateMultipartEmail();

private:
	struct EmailContent
	{
		std::optional<std::string> text_body;
		std::optional<std::string> text_charset;
		std::optional<std::string> html_body;
		std::optional<std::string> html_charset;
	};
	EmailContent m_content;

	struct Attachment
	{
		std::string file_name;
		std::vector<uint8_t> data;
		std::string mime_type;
		std::optional<std::string> content_id;
		bool is_inline;
	};
	std::vector<Attachment> m_attachments;

private:
	MimeBuilder();

private:
	std::string m_from;
	std::string m_to;
	std::string m_subject;
	std::string m_cc;
	std::string m_bcc;
	std::string m_reply_to;
	std::map<std::string, std::string> m_custom_headers;
	EmailType m_email_type;
	bool m_type_manually_set;

private:
	void AddStandardHeaders(std::shared_ptr<MimeEntity> entity);
	std::shared_ptr<MimeEntity> BuildTextOnlyEmail();
	std::shared_ptr<MimeEntity> BuildHtmlOnlyEmail();
	std::shared_ptr<MimeEntity> BuildMultipartAlternativeEmail();
	std::shared_ptr<MimeEntity> BuildMultipartMixedEmail(std::shared_ptr<MimeEntity> contentEntity);
	std::shared_ptr<MimeEntity> BuildMultipartRelatedEmail(std::shared_ptr<MimeEntity> contentEntity);
	std::shared_ptr<MimeEntity> CreateAttachmentEntity(const Attachment& attachment);
	void ValidateRequiredFields() const;

private:
	void UpdateEmailType();
	std::shared_ptr<MimeEntity> BuildBasedOnType();
};
} // namespace ISXMime
