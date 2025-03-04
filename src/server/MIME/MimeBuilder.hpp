#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ContentType.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

class MimeBuilder
{
public:
	MimeBuilder();
	~MimeBuilder();

public:
	MimeBuilder& From(const std::string& from);
	MimeBuilder& To(const std::string& to);
	MimeBuilder& Subject(const std::string& subject);
	MimeBuilder& Cc(const std::string& cc);
	MimeBuilder& Bcc(const std::string& bcc);
	MimeBuilder& ReplyTo(const std::string& replyTo);

public:
	MimeBuilder& TextBody(const std::string& text);
	MimeBuilder& HtmlBody(const std::string& html);

	MimeBuilder& AddAttachment(const std::string& filePath);
	MimeBuilder& AddAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
							   const std::string& mimeType = "application/octet-stream");

public:
	std::shared_ptr<MimeEntity> Build();
	std::string ToString();
	bool SaveToFile(const std::string& filePath);

private:
	std::string m_from;
	std::string m_to;
	std::string m_subject;
	std::string m_cc;
	std::string m_bcc;
	std::string m_reply_to;

	std::string m_text_body;
	std::string m_html_body;

private:
	struct Attachment
	{
		std::string file_name;
		std::vector<uint8_t> data;
		std::string mime_type;
	};

	std::vector<Attachment> m_attachments;

private:
	void AddStandardHeaders(std::shared_ptr<MimeEntity> entity);
};

} // namespace ISXMime
