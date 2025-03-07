#include "MimeBuilder.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "../Encoding/EncoderFactory.hpp"
#include "Body.hpp"
#include "MimeUtils.hpp"

namespace ISXMime
{

MimeBuilder::MimeBuilder() : m_email_type(EmailType::Simple), m_type_manually_set(false) {}

MimeBuilder MimeBuilder::Create()
{
	return {};
}

MimeBuilder MimeBuilder::CreateTextEmail()
{
	auto builder = Create();
	builder.SetEmailType(EmailType::Simple);
	return builder;
}

MimeBuilder MimeBuilder::CreateHtmlEmail()
{
	auto builder = Create();
	builder.SetEmailType(EmailType::Simple);
	return builder;
}

MimeBuilder MimeBuilder::CreateMultipartEmail()
{
	auto builder = Create();
	builder.SetEmailType(EmailType::Alternative);
	return builder;
}

MimeBuilder& MimeBuilder::SetEmailType(EmailType type)
{
	m_email_type = type;
	m_type_manually_set = true;
	return *this;
}

EmailType MimeBuilder::GetEmailType() const
{
	return m_email_type;
}

void MimeBuilder::UpdateEmailType()
{
	if (m_type_manually_set) return;

	bool has_inline_attachments = false;
	for (const auto& attachment : m_attachments)
	{
		if (attachment.is_inline)
		{
			has_inline_attachments = true;
			break;
		}
	}

	// determine email type based on content
	if (!m_attachments.empty())
	{
		if (has_inline_attachments)
			m_email_type = EmailType::Related;
		else
			m_email_type = EmailType::Mixed;
	}
	else if (m_content.text_body.has_value() && m_content.html_body.has_value())
	{
		m_email_type = EmailType::Alternative;
	}
	else
	{
		m_email_type = EmailType::Simple;
	}
}

MimeBuilder& MimeBuilder::From(const std::string& from)
{
	m_from = from;
	return *this;
}

MimeBuilder& MimeBuilder::To(const std::string& to)
{
	m_to = to;
	return *this;
}

MimeBuilder& MimeBuilder::Subject(const std::string& subject)
{
	m_subject = subject;
	return *this;
}

MimeBuilder& MimeBuilder::Cc(const std::string& cc)
{
	m_cc = cc;
	return *this;
}

MimeBuilder& MimeBuilder::Bcc(const std::string& bcc)
{
	m_bcc = bcc;
	return *this;
}

MimeBuilder& MimeBuilder::ReplyTo(const std::string& replyTo)
{
	m_reply_to = replyTo;
	return *this;
}

MimeBuilder& MimeBuilder::AddCustomHeader(const std::string& name, const std::string& value)
{
	if (!name.empty())
	{
		m_custom_headers[name] = value;
	}
	return *this;
}

// modified content methods to update email type automatically
MimeBuilder& MimeBuilder::TextBody(const std::string& text, const std::string& charset)
{
	m_content.text_body = text;
	m_content.text_charset = charset;
	UpdateEmailType();
	return *this;
}

MimeBuilder& MimeBuilder::HtmlBody(const std::string& html, const std::string& charset)
{
	m_content.html_body = html;
	m_content.html_charset = charset;
	UpdateEmailType();
	return *this;
}

MimeBuilder& MimeBuilder::AddAttachment(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file) throw std::runtime_error("Failed to open attachment file: " + filePath);

	std::string file_name = filePath;
	size_t last_slash = filePath.find_last_of("/\\");
	if (last_slash != std::string::npos) file_name = filePath.substr(last_slash + 1);

	std::vector<uint8_t> file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string extension;
	size_t last_dot = file_name.find_last_of('.');
	if (last_dot != std::string::npos) extension = file_name.substr(last_dot + 1);

	ContentType content_type = MimeUtils::GetMimeTypeForFileExtension(extension);

	Attachment attachment;
	attachment.file_name = file_name;
	attachment.data = std::move(file_content);
	attachment.mime_type = content_type.Str();
	attachment.is_inline = false;

	m_attachments.push_back(std::move(attachment));
	UpdateEmailType();
	return *this;
}

MimeBuilder& MimeBuilder::AddAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
										const std::string& mimeType)
{
	Attachment attachment;
	attachment.file_name = fileName;
	attachment.data = data;
	attachment.mime_type = mimeType;
	attachment.is_inline = false;

	m_attachments.push_back(std::move(attachment));
	UpdateEmailType();
	return *this;
}

MimeBuilder& MimeBuilder::AddInlineAttachment(const std::string& filePath, const std::string& contentId)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file) throw std::runtime_error("Failed to open inline attachment file: " + filePath);

	std::string file_name = filePath;
	size_t last_slash = filePath.find_last_of("/\\");
	if (last_slash != std::string::npos) file_name = filePath.substr(last_slash + 1);

	std::vector<uint8_t> file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string extension;
	size_t last_dot = file_name.find_last_of('.');
	if (last_dot != std::string::npos) extension = file_name.substr(last_dot + 1);

	ContentType content_type = MimeUtils::GetMimeTypeForFileExtension(extension);

	Attachment attachment;
	attachment.file_name = file_name;
	attachment.data = std::move(file_content);
	attachment.mime_type = content_type.Str();
	attachment.content_id = contentId;
	attachment.is_inline = true;

	m_attachments.push_back(std::move(attachment));
	UpdateEmailType();
	return *this;
}

MimeBuilder& MimeBuilder::AddInlineAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
											  const std::string& contentId, const std::string& mimeType)
{
	Attachment attachment;
	attachment.file_name = fileName;
	attachment.data = data;
	attachment.mime_type = mimeType;
	attachment.content_id = contentId;
	attachment.is_inline = true;

	m_attachments.push_back(std::move(attachment));
	UpdateEmailType();
	return *this;
}

void MimeBuilder::ValidateRequiredFields() const
{
	if (m_from.empty()) throw std::runtime_error("From field is required");

	if (m_to.empty()) throw std::runtime_error("To field is required");

	bool has_content = m_content.text_body.has_value() || m_content.html_body.has_value();
	if (!has_content && m_attachments.empty())
		throw std::runtime_error("Email must have either a text body, HTML body, or attachments");
}

std::shared_ptr<MimeEntity> MimeBuilder::Build()
{
	ValidateRequiredFields();
	// final update of email type before building
	UpdateEmailType();
	return BuildBasedOnType();
}

std::shared_ptr<MimeEntity> MimeBuilder::BuildBasedOnType()
{
	switch (m_email_type)
	{
	case EmailType::Simple:
		if (m_content.html_body.has_value())
			return BuildHtmlOnlyEmail();
		else
			return BuildTextOnlyEmail();

	case EmailType::Alternative:
		return BuildMultipartAlternativeEmail();

	case EmailType::Mixed:
	{
		std::shared_ptr<MimeEntity> content_entity;

		if (m_content.text_body.has_value() && m_content.html_body.has_value())
			content_entity = BuildMultipartAlternativeEmail();
		else if (m_content.html_body.has_value())
			content_entity = BuildHtmlOnlyEmail();
		else
			content_entity = BuildTextOnlyEmail();

		return BuildMultipartMixedEmail(content_entity);
	}

	case EmailType::Related:
	{
		std::shared_ptr<MimeEntity> content_entity;

		if (m_content.text_body.has_value() && m_content.html_body.has_value())
			content_entity = BuildMultipartAlternativeEmail();
		else if (m_content.html_body.has_value())
			content_entity = BuildHtmlOnlyEmail();
		else
			content_entity = BuildTextOnlyEmail();

		return BuildMultipartRelatedEmail(content_entity);
	}

	default:
		throw std::runtime_error("Unknown email type");
	}
}

std::shared_ptr<MimeEntity> MimeBuilder::BuildMultipartRelatedEmail(std::shared_ptr<MimeEntity> contentEntity)
{
	auto entity = std::make_shared<MimeEntity>();
	AddStandardHeaders(entity);

	// create multipart/related content type with boundary
	ContentType content_type = ContentType("multipart", "related");
	std::string boundary = MimeUtils::GenerateMimeBoundary();
	content_type.AddParameter("boundary", boundary);
	entity->GetHeader().SetContentType(content_type);

	// start building the body with preamble
	std::stringstream body_stream;
	body_stream << "This is a MIME multipart message.\r\n\r\n";

	// add the content entity as the first part
	body_stream << "--" << boundary << "\r\n";
	body_stream << *contentEntity << "\r\n";

	// add inline attachments
	for (const auto& attachment : m_attachments)
	{
		if (attachment.is_inline)
		{
			body_stream << "--" << boundary << "\r\n";
			auto attachment_entity = CreateAttachmentEntity(attachment);
			body_stream << *attachment_entity << "\r\n";
		}
	}

	// add regular attachments
	for (const auto& attachment : m_attachments)
	{
		if (!attachment.is_inline)
		{
			body_stream << "--" << boundary << "\r\n";
			auto attachment_entity = CreateAttachmentEntity(attachment);
			body_stream << *attachment_entity << "\r\n";
		}
	}

	body_stream << "--" << boundary << "--\r\n";

	entity->GetBody().Set(body_stream.str());

	return entity;
}

std::shared_ptr<MimeEntity> MimeBuilder::BuildTextOnlyEmail()
{
	auto entity = std::make_shared<MimeEntity>();
	AddStandardHeaders(entity);

	entity->GetHeader().SetContentType(ContentType::TextPlain());

	if (m_content.text_charset.has_value())
		entity->GetHeader().GetContentType().AddParameter("charset", *m_content.text_charset);
	else
		entity->GetHeader().GetContentType().AddParameter("charset", "UTF-8");

	entity->GetHeader().SetContentTransferEncoding(ContentTransferEncoding(ContentTransferEncoding::QUOTED_PRINTABLE));

	auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
	std::vector<uint8_t> text_data(m_content.text_body->begin(), m_content.text_body->end());
	entity->GetBody().Set(qp_encoder->Encode(text_data));

	return entity;
}

std::shared_ptr<MimeEntity> MimeBuilder::BuildHtmlOnlyEmail()
{
	auto entity = std::make_shared<MimeEntity>();
	AddStandardHeaders(entity);

	entity->GetHeader().SetContentType(ContentType::TextHtml());

	if (m_content.html_charset.has_value())
		entity->GetHeader().GetContentType().AddParameter("charset", *m_content.html_charset);
	else
		entity->GetHeader().GetContentType().AddParameter("charset", "UTF-8");

	entity->GetHeader().SetContentTransferEncoding(ContentTransferEncoding(ContentTransferEncoding::QUOTED_PRINTABLE));

	auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
	std::vector<uint8_t> html_data(m_content.html_body->begin(), m_content.html_body->end());
	entity->GetBody().Set(qp_encoder->Encode(html_data));

	return entity;
}

std::shared_ptr<MimeEntity> MimeBuilder::BuildMultipartAlternativeEmail()
{
	auto entity = std::make_shared<MimeEntity>();
	AddStandardHeaders(entity);

	// create multipart/alternative content type with boundary
	ContentType content_type = ContentType::MultipartAlternative();
	std::string boundary = MimeUtils::GenerateMimeBoundary();
	content_type.AddParameter("boundary", boundary);
	entity->GetHeader().SetContentType(content_type);

	// start building the body with preamble
	std::stringstream body_stream;
	body_stream << "This is a MIME multipart message.\r\n\r\n";

	// add text part
	if (m_content.text_body.has_value())
	{
		body_stream << "--" << boundary << "\r\n";
		body_stream << "Content-Type: text/plain; charset=";
		body_stream << (m_content.text_charset.has_value() ? *m_content.text_charset : "UTF-8") << "\r\n";
		body_stream << "Content-Transfer-Encoding: quoted-printable\r\n\r\n";

		auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
		std::vector<uint8_t> text_data(m_content.text_body->begin(), m_content.text_body->end());
		body_stream << qp_encoder->Encode(text_data) << "\r\n\r\n";
	}

	// add HTML part
	if (m_content.html_body.has_value())
	{
		body_stream << "--" << boundary << "\r\n";
		body_stream << "Content-Type: text/html; charset=";
		body_stream << (m_content.html_charset.has_value() ? *m_content.html_charset : "UTF-8") << "\r\n";
		body_stream << "Content-Transfer-Encoding: quoted-printable\r\n\r\n";

		auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
		std::vector<uint8_t> html_data(m_content.html_body->begin(), m_content.html_body->end());
		body_stream << qp_encoder->Encode(html_data) << "\r\n\r\n";
	}

	body_stream << "--" << boundary << "--\r\n";

	entity->GetBody().Set(body_stream.str());

	return entity;
}

std::shared_ptr<MimeEntity> MimeBuilder::CreateAttachmentEntity(const Attachment& attachment)
{
	auto entity = std::make_shared<MimeEntity>();

	entity->GetHeader().SetContentType(ContentType(attachment.mime_type));

	entity->GetHeader().SetContentTransferEncoding(ContentTransferEncoding(ContentTransferEncoding::BASE64));

	// Handle filename encoding and Content-Disposition
	std::string encoded_filename = MimeUtils::EncodeFilename(attachment.file_name);
	std::string disposition = attachment.is_inline ? "inline" : "attachment";

	if (encoded_filename == attachment.file_name)
	{
		entity->GetHeader().AddField("Content-Disposition",
									 disposition + "; filename=\"" + attachment.file_name + "\"");
	}
	else
	{
		entity->GetHeader().AddField("Content-Disposition",
									 disposition + "; "
										 + MimeUtils::EncodeParameterValue("filename", attachment.file_name));
	}

	// add Content-ID for inline attachments
	if (attachment.is_inline && attachment.content_id.has_value())
	{
		std::string content_id = *attachment.content_id;
		if (content_id.front() != '<') content_id = "<" + content_id;
		if (content_id.back() != '>') content_id += ">";
		entity->GetHeader().AddField("Content-ID", content_id);
	}

	// encode the data
	auto base64_encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
	entity->GetBody().Set(base64_encoder->Encode(attachment.data));

	return entity;
}

std::shared_ptr<MimeEntity> MimeBuilder::BuildMultipartMixedEmail(std::shared_ptr<MimeEntity> contentEntity)
{
	auto entity = std::make_shared<MimeEntity>();
	AddStandardHeaders(entity);

	// create multipart/mixed content type with boundary
	ContentType content_type = ContentType::MultipartMixed();
	std::string boundary = MimeUtils::GenerateMimeBoundary();
	content_type.AddParameter("boundary", boundary);
	entity->GetHeader().SetContentType(content_type);

	// start building the body with preamble
	std::stringstream body_stream;
	body_stream << "This is a MIME multipart message.\r\n\r\n";

	// add the content part
	body_stream << "--" << boundary << "\r\n";

	// add content headers
	body_stream << "Content-Type: " << contentEntity->GetHeader().GetContentType().Str() << "\r\n";
	body_stream << "Content-Transfer-Encoding: " << contentEntity->GetHeader().GetContentTransferEncoding().Str()
				<< "\r\n\r\n";

	// add content body
	body_stream << contentEntity->GetBody().Content() << "\r\n\r\n";

	// add all attachments
	for (const auto& attachment : m_attachments)
	{
		auto attachment_entity = CreateAttachmentEntity(attachment);

		body_stream << "--" << boundary << "\r\n";
		body_stream << attachment_entity->GetHeader();
		body_stream << attachment_entity->GetBody().Content() << "\r\n\r\n";
	}

	body_stream << "--" << boundary << "--\r\n";

	entity->GetBody().Set(body_stream.str());

	return entity;
}

void MimeBuilder::AddStandardHeaders(std::shared_ptr<MimeEntity> entity)
{
	entity->GetHeader().AddField("From", m_from);
	entity->GetHeader().AddField("To", m_to);

	if (!m_subject.empty()) entity->GetHeader().AddField("Subject", entity->GetHeader().EncodeFieldValue(m_subject));

	if (!m_cc.empty()) entity->GetHeader().AddField("Cc", m_cc);
	if (!m_bcc.empty()) entity->GetHeader().AddField("Bcc", m_bcc);
	if (!m_reply_to.empty()) entity->GetHeader().AddField("Reply-To", m_reply_to);

	for (const auto& header_pair : m_custom_headers)
		entity->GetHeader().AddField(header_pair.first, header_pair.second);

	entity->GetHeader().AddField("MIME-Version", "1.0");

	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	char time_buffer[128];
	std::strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S %z", std::localtime(&time));
	entity->GetHeader().AddField("Date", time_buffer);

	std::stringstream msg_id;
	msg_id << "<" << std::hex << time << "." << reinterpret_cast<uintptr_t>(this) << "@"
		   << (m_from.find('@') != std::string::npos ? m_from.substr(m_from.find('@') + 1) : "localhost") << ">";
	entity->GetHeader().AddField("Message-ID", msg_id.str());
}

std::string MimeBuilder::ToString()
{
	std::shared_ptr<MimeEntity> entity = Build();
	std::ostringstream oss;
	oss << *entity;
	return oss.str();
}

bool MimeBuilder::SaveToFile(const std::string& filePath)
{
	try
	{
		std::ofstream file(filePath, std::ios::binary);
		if (!file) return false;

		file << ToString();
		return file.good();
	}
	catch (const std::exception&)
	{
		return false;
	}
}

} // namespace ISXMime
