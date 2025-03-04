#include "MimeBuilder.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../Encoding/EncoderFactory.hpp"
#include "Body.hpp"
#include "MimeUtils.hpp"

namespace ISXMime
{

MimeBuilder::MimeBuilder() = default;

MimeBuilder::~MimeBuilder() = default;

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

MimeBuilder& MimeBuilder::TextBody(const std::string& text)
{
	m_text_body = text;
	return *this;
}

MimeBuilder& MimeBuilder::HtmlBody(const std::string& html)
{
	m_html_body = html;
	return *this;
}

MimeBuilder& MimeBuilder::AddAttachment(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file) return *this;

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
	attachment.data = file_content;
	attachment.mime_type = content_type.Str();

	m_attachments.push_back(attachment);

	return *this;
}

MimeBuilder& MimeBuilder::AddAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
										const std::string& mimeType)
{
	Attachment attachment;
	attachment.file_name = fileName;
	attachment.data = data;
	attachment.mime_type = mimeType;

	m_attachments.push_back(attachment);

	return *this;
}

std::shared_ptr<MimeEntity> MimeBuilder::Build()
{
	if (m_from.empty() || m_to.empty()) throw std::runtime_error("From and To are required fields");

	if (m_text_body.empty() && m_html_body.empty()) throw std::runtime_error("Either text or HTML body is required");

	auto entity = std::make_shared<MimeEntity>();

	AddStandardHeaders(entity);

	if (!m_attachments.empty() || (!m_text_body.empty() && !m_html_body.empty()))
	{
		// this is a multipart message
		if (!m_text_body.empty() && !m_html_body.empty())
		{
			// multipart/alternative for text and HTML
			std::string boundary = MimeUtils::GenerateMimeBoundary();

			ContentType content_type = ContentType::MultipartAlternative();
			content_type.AddParameter("boundary", boundary);
			entity->GetHeader().SetContentType(content_type);

			std::stringstream body_stream;
			body_stream << "This is a MIME multipart message.\r\n\r\n";

			body_stream << "--" << boundary << "\r\n";
			body_stream << "Content-Type: text/plain; charset=UTF-8\r\n";
			body_stream << "Content-Transfer-Encoding: quoted-printable\r\n\r\n";

			auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
			std::vector<uint8_t> text_data(m_text_body.begin(), m_text_body.end());
			body_stream << qp_encoder->Encode(text_data) << "\r\n\r\n";

			body_stream << "--" << boundary << "\r\n";
			body_stream << "Content-Type: text/html; charset=UTF-8\r\n";
			body_stream << "Content-Transfer-Encoding: quoted-printable\r\n\r\n";

			std::vector<uint8_t> html_data(m_html_body.begin(), m_html_body.end());
			body_stream << qp_encoder->Encode(html_data) << "\r\n\r\n";

			body_stream << "--" << boundary << "--\r\n";

			if (!m_attachments.empty())
			{
				std::string mixed_boundary = MimeUtils::GenerateMimeBoundary();
				ContentType mixed_type = ContentType::MultipartMixed();
				mixed_type.AddParameter("boundary", mixed_boundary);
				entity->GetHeader().SetContentType(mixed_type);

				std::stringstream mixed_stream;
				mixed_stream << "This is a MIME multipart message.\r\n\r\n";

				mixed_stream << "--" << mixed_boundary << "\r\n";
				mixed_stream << "Content-Type: multipart/alternative; boundary=\"" << boundary << "\"\r\n\r\n";
				mixed_stream << body_stream.str() << "\r\n";

				for (const auto& attachment : m_attachments)
				{
					mixed_stream << "--" << mixed_boundary << "\r\n";
					mixed_stream << "Content-Type: " << attachment.mime_type << "\r\n";
					mixed_stream << "Content-Transfer-Encoding: base64\r\n";

					std::string encoded_filename = MimeUtils::EncodeFilename(attachment.file_name);
					if (encoded_filename == attachment.file_name)
					{
						mixed_stream << "Content-Disposition: attachment; filename=\"" << attachment.file_name
									 << "\"\r\n\r\n";
					}
					else
					{
						mixed_stream << "Content-Disposition: attachment; "
									 << MimeUtils::EncodeParameterValue("filename", attachment.file_name) << "\r\n\r\n";
					}

					auto base64_encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
					mixed_stream << base64_encoder->Encode(attachment.data) << "\r\n\r\n";
				}

				mixed_stream << "--" << mixed_boundary << "--\r\n";

				entity->GetBody().Set(mixed_stream.str());
			}
			else
			{
				entity->GetBody().Set(body_stream.str());
			}
		}
		else
		{
			std::string boundary = MimeUtils::GenerateMimeBoundary();
			ContentType content_type = ContentType::MultipartMixed();
			content_type.AddParameter("boundary", boundary);
			entity->GetHeader().SetContentType(content_type);

			std::stringstream body_stream;
			body_stream << "This is a MIME multipart message.\r\n\r\n";

			body_stream << "--" << boundary << "\r\n";

			if (!m_text_body.empty())
			{
				body_stream << "Content-Type: text/plain; charset=UTF-8\r\n";
				body_stream << "Content-Transfer-Encoding: quoted-printable\r\n\r\n";

				auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
				std::vector<uint8_t> text_data(m_text_body.begin(), m_text_body.end());
				body_stream << qp_encoder->Encode(text_data) << "\r\n\r\n";
			}
			else
			{
				body_stream << "Content-Type: text/html; charset=UTF-8\r\n";
				body_stream << "Content-Transfer-Encoding: quoted-printable\r\n\r\n";

				auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
				std::vector<uint8_t> html_data(m_html_body.begin(), m_html_body.end());
				body_stream << qp_encoder->Encode(html_data) << "\r\n\r\n";
			}

			for (const auto& attachment : m_attachments)
			{
				body_stream << "--" << boundary << "\r\n";
				body_stream << "Content-Type: " << attachment.mime_type << "\r\n";
				body_stream << "Content-Transfer-Encoding: base64\r\n";

				std::string encoded_filename = MimeUtils::EncodeFilename(attachment.file_name);
				if (encoded_filename == attachment.file_name)
				{
					body_stream << "Content-Disposition: attachment; filename=\"" << attachment.file_name
								<< "\"\r\n\r\n";
				}
				else
				{
					body_stream << "Content-Disposition: attachment; "
								<< MimeUtils::EncodeParameterValue("filename", attachment.file_name) << "\r\n\r\n";
				}

				auto base64_encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
				body_stream << base64_encoder->Encode(attachment.data) << "\r\n\r\n";
			}

			body_stream << "--" << boundary << "--\r\n";

			entity->GetBody().Set(body_stream.str());
		}
	}
	else
	{
		if (!m_text_body.empty())
		{
			entity->GetHeader().SetContentType(ContentType::TextPlain());
			entity->GetHeader().SetContentTransferEncoding(
				ContentTransferEncoding(ContentTransferEncoding::QUOTED_PRINTABLE));

			auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
			std::vector<uint8_t> text_data(m_text_body.begin(), m_text_body.end());
			entity->GetBody().Set(qp_encoder->Encode(text_data));
		}
		else
		{
			entity->GetHeader().SetContentType(ContentType::TextHtml());
			entity->GetHeader().SetContentTransferEncoding(
				ContentTransferEncoding(ContentTransferEncoding::QUOTED_PRINTABLE));

			auto qp_encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
			std::vector<uint8_t> html_data(m_html_body.begin(), m_html_body.end());
			entity->GetBody().Set(qp_encoder->Encode(html_data));
		}
	}

	return entity;
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
	std::ofstream file(filePath, std::ios::binary);
	if (!file) return false;

	file << ToString();
	return file.good();
}

void MimeBuilder::AddStandardHeaders(std::shared_ptr<MimeEntity> entity)
{
	// required headers
	entity->GetHeader().AddField("From", m_from);
	entity->GetHeader().AddField("To", m_to);

	if (!m_subject.empty()) entity->GetHeader().AddField("Subject", entity->GetHeader().EncodeFieldValue(m_subject));

	// optional headers
	if (!m_cc.empty()) entity->GetHeader().AddField("Cc", m_cc);

	if (!m_bcc.empty()) entity->GetHeader().AddField("Bcc", m_bcc);

	if (!m_reply_to.empty()) entity->GetHeader().AddField("Reply-To", m_reply_to);

	// standard headers
	entity->GetHeader().AddField("MIME-Version", "1.0");

	// date header
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	char time_buffer[128];
	std::strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S %z", std::localtime(&time));
	entity->GetHeader().AddField("Date", time_buffer);

	// message-ID header
	std::stringstream msg_id;
	msg_id << "<" << std::hex << time << "." << reinterpret_cast<uintptr_t>(this) << "@"
		   << (m_from.find('@') != std::string::npos ? m_from.substr(m_from.find('@') + 1) : "localhost") << ">";

	entity->GetHeader().AddField("Message-ID", msg_id.str());
}

} // namespace ISXMime
