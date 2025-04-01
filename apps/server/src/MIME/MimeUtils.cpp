#include "MimeUtils.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <unordered_map>

#include "../Encoding/EncoderFactory.hpp"
#include "Body.hpp"

namespace ISXMime
{

std::string MimeUtils::GenerateMimeBoundary()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << "----=_NextPart_" << std::hex << time << "_";
	ss << GenerateRandomString(16);

	return ss.str();
}

std::string MimeUtils::GenerateRandomString(size_t length)
{
	static const char CHARSET[] = "0123456789"
								  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
								  "abcdefghijklmnopqrstuvwxyz";

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<size_t> distribution(0, sizeof(CHARSET) - 2);

	std::string result;
	result.reserve(length);

	for (size_t i = 0; i < length; ++i) result += CHARSET[distribution(generator)];

	return result;
}

std::string MimeUtils::EncodeFilename(const std::string& filename, const std::string& charset)
{
	bool needs_encoding = false;
	for (unsigned char c : filename)
	{
		if (c > 127)
		{
			needs_encoding = true;
			break;
		}
	}

	if (!needs_encoding)
	{
		return filename;
	}

	// =?charset?encoding?encoded-text?=
	// Base64 encoding for filenames as they often contain non-ASCII chars
	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
	std::vector<uint8_t> data(filename.begin(), filename.end());
	std::string encoded = encoder->Encode(data);

	encoded.erase(std::remove(encoded.begin(), encoded.end(), '\r'), encoded.end());
	encoded.erase(std::remove(encoded.begin(), encoded.end(), '\n'), encoded.end());

	return "=?" + charset + "?B?" + encoded + "?=";
}

std::string MimeUtils::DecodeFilename(const std::string& encoded_filename)
{
	if (encoded_filename.size() > 7 && encoded_filename.substr(0, 2) == "=?"
		&& encoded_filename.substr(encoded_filename.size() - 2) == "?=")
	{
		// parse the encoded-word: =?charset?encoding?encoded-text?=
		size_t charset_end = encoded_filename.find('?', 2);
		if (charset_end != std::string::npos)
		{
			size_t encoding_end = encoded_filename.find('?', charset_end + 1);
			if (encoding_end != std::string::npos)
			{
				std::string charset = encoded_filename.substr(2, charset_end - 2);
				char encoding_type = std::toupper(encoded_filename[charset_end + 1]);
				std::string encoded_text =
					encoded_filename.substr(encoding_end + 1, encoded_filename.size() - encoding_end - 3);

				if (encoding_type == 'B')
				{
					// Base64 encoding
					auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
					auto data = encoder->Decode(encoded_text);
					return std::string(data.begin(), data.end());
				}
				else if (encoding_type == 'Q')
				{
					// Q-encoding
					std::replace(encoded_text.begin(), encoded_text.end(), '_', ' ');
					auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
					auto data = encoder->Decode(encoded_text);
					return std::string(data.begin(), data.end());
				}
			}
		}
	}

	return encoded_filename;
}

std::string MimeUtils::EncodeParameterValue(const std::string& name, const std::string& value,
											const std::string& charset)
{
	// check if encoding is needed
	bool needs_encoding = false;
	for (unsigned char c : value)
	{
		if (c > 127)
		{
			needs_encoding = true;
			break;
		}
	}

	if (!needs_encoding) return name + "=\"" + value + "\"";

	// name*=charset'language'encoded-value
	std::string encoded;
	encoded.reserve(value.size() * 3); // pessimistic estimation

	for (unsigned char c : value)
	{
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '!' || c == '*'
			|| c == '-' || c == '.' || c == '_')
		{
			// these chars are safe per RFC 2231
			encoded += c;
		}
		else
		{
			// percent-encode everything else
			encoded += '%';
			encoded += "0123456789ABCDEF"[(c >> 4) & 0xF];
			encoded += "0123456789ABCDEF"[c & 0xF];
		}
	}

	return name + "*=" + charset + "''" + encoded;
}

std::string MimeUtils::DecodeBase64(const std::string& encoded_text)
{
	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("base64");
	if (!encoder) throw MimeException("Base64 encoder not available");

	std::vector<uint8_t> decoded = encoder->Decode(encoded_text);

	return {decoded.begin(), decoded.end()};
}

std::string MimeUtils::DecodeQuotedPrintable(const std::string& encoded_text)
{
	auto encoder = ISXEncoding::EncoderFactory::CreateEncoder("quoted-printable");
	if (!encoder) throw MimeException("Quoted-Printable encoder not available");

	std::vector<uint8_t> decoded = encoder->Decode(encoded_text);

	return {decoded.begin(), decoded.end()};
}

std::string MimeUtils::GetFileExtensionForMimeType(const ContentType& contentType)
{
	static const std::unordered_map<std::string, std::string> MIME_TO_EXT = {{"text/plain", "txt"},
																			 {"text/html", "html"},
																			 {"text/css", "css"},
																			 {"text/javascript", "js"},
																			 {"image/jpeg", "jpg"},
																			 {"image/png", "png"},
																			 {"image/gif", "gif"},
																			 {"image/bmp", "bmp"},
																			 {"image/webp", "webp"},
																			 {"audio/mpeg", "mp3"},
																			 {"audio/wav", "wav"},
																			 {"video/mp4", "mp4"},
																			 {"application/pdf", "pdf"},
																			 {"application/zip", "zip"},
																			 {"application/json", "json"},
																			 {"application/xml", "xml"},
																			 {"application/octet-stream", "bin"}};

	auto it = MIME_TO_EXT.find(contentType.Str());

	if (it != MIME_TO_EXT.end()) return it->second;

	return "bin";
}

ContentType MimeUtils::GetMimeTypeForFileExtension(const std::string& extension)
{
	std::string ext = extension;
	std::ranges::transform(ext, ext.begin(), [](unsigned char c) { return std::tolower(c); });

	static const std::unordered_map<std::string, std::string> EXT_TO_MIME = {
		{"txt", "text/plain"},
		{"html", "text/html"},
		{"htm", "text/html"},
		{"css", "text/css"},
		{"js", "text/javascript"},
		{"jpg", "image/jpeg"},
		{"jpeg", "image/jpeg"},
		{"png", "image/png"},
		{"gif", "image/gif"},
		{"bmp", "image/bmp"},
		{"webp", "image/webp"},
		{"mp3", "audio/mpeg"},
		{"wav", "audio/wav"},
		{"mp4", "video/mp4"},
		{"pdf", "application/pdf"},
		{"zip", "application/zip"},
		{"json", "application/json"},
		{"xml", "application/xml"},
		{"doc", "application/msword"},
		{"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
		{"xls", "application/vnd.ms-excel"},
		{"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
		{"ppt", "application/vnd.ms-powerpoint"},
		{"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"}};

	auto it = EXT_TO_MIME.find(ext);

	if (it != EXT_TO_MIME.end()) return ContentType(it->second);

	return ContentType("application/octet-stream");
}

const std::map<std::string, std::string>& MimeUtils::GetCommonMimeTypes()
{
	static const std::map<std::string, std::string> COMMON_MIME_TYPES = {{"text/plain", "Plain text"},
																		 {"text/html", "HTML document"},
																		 {"text/css", "Cascading Style Sheet"},
																		 {"text/javascript", "JavaScript code"},
																		 {"image/jpeg", "JPEG image"},
																		 {"image/png", "PNG image"},
																		 {"image/gif", "GIF image"},
																		 {"image/bmp", "Bitmap image"},
																		 {"image/webp", "WebP image"},
																		 {"audio/mpeg", "MP3 audio"},
																		 {"audio/wav", "WAV audio"},
																		 {"video/mp4", "MP4 video"},
																		 {"application/pdf", "PDF document"},
																		 {"application/zip", "ZIP archive"},
																		 {"application/json", "JSON data"},
																		 {"application/xml", "XML data"},
																		 {"application/octet-stream", "Binary data"},
																		 {"multipart/mixed", "MIME multipart content"},
																		 {"multipart/alternative",
																		  "MIME alternative versions"},
																		 {"multipart/related", "MIME related content"},
																		 {"multipart/form-data", "MIME form data"}};

	return COMMON_MIME_TYPES;
}

bool MimeUtils::CaseInsensitiveCompare(const std::string& a, const std::string& b)
{
	if (a.size() != b.size()) return false;

	return std::equal(a.begin(), a.end(), b.begin(),
					  [](char a_char, char b_char) { return std::tolower(a_char) == std::tolower(b_char); });
}

std::string MimeUtils::GetContentTypeDescription(const ContentType& contentType)
{
	const std::string& type = contentType.Type();
	const std::string& subtype = contentType.Subtype();

	if (type == "text")
	{
		if (subtype == "plain") return "Plain Text";
		if (subtype == "html") return "HTML Document";
		if (subtype == "xml") return "XML Document";
		if (subtype == "rtf") return "Rich Text Format";
		if (subtype == "css") return "CSS Stylesheet";
		if (subtype == "javascript") return "JavaScript Code";
		return "Text Document";
	}
	if (type == "image")
	{
		if (subtype == "jpeg" || subtype == "jpg") return "JPEG Image";
		if (subtype == "png") return "PNG Image";
		if (subtype == "gif") return "GIF Image";
		if (subtype == "bmp") return "Bitmap Image";
		if (subtype == "tiff") return "TIFF Image";
		if (subtype == "svg+xml") return "SVG Vector Image";
		return "Image";
	}
	if (type == "audio")
	{
		if (subtype == "mpeg" || subtype == "mp3") return "MP3 Audio";
		if (subtype == "wav") return "WAV Audio";
		if (subtype == "ogg") return "OGG Audio";
		if (subtype == "midi") return "MIDI Audio";
		return "Audio";
	}
	if (type == "video")
	{
		if (subtype == "mp4") return "MP4 Video";
		if (subtype == "mpeg") return "MPEG Video";
		if (subtype == "quicktime") return "QuickTime Video";
		if (subtype == "x-msvideo" || subtype == "avi") return "AVI Video";
		if (subtype == "webm") return "WebM Video";
		return "Video";
	}
	if (type == "application")
	{
		if (subtype == "pdf") return "PDF Document";
		if (subtype == "msword") return "Word Document";
		if (subtype == "vnd.ms-excel") return "Excel Spreadsheet";
		if (subtype == "vnd.ms-powerpoint") return "PowerPoint Presentation";
		if (subtype == "zip") return "ZIP Archive";
		if (subtype == "x-rar-compressed") return "RAR Archive";
		if (subtype == "json") return "JSON Data";
		if (subtype == "xml") return "XML Data";
		return "Application Data";
	}
	if (type == "multipart")
	{
		if (subtype == "alternative") return "Alternative Content Formats";
		if (subtype == "mixed") return "Mixed Content";
		if (subtype == "related") return "Related Content";
		if (subtype == "form-data") return "Form Data";
		return "Multi-part Content";
	}

	return contentType.Str();
}

std::string MimeUtils::NormalizeHeaderFieldName(const std::string& name)
{
	if (name.empty()) return name;

	std::string lower_name;
	lower_name.reserve(name.size());
	std::ranges::transform(name, std::back_inserter(lower_name), [](unsigned char c) { return std::tolower(c); });

	static const std::map<std::string, std::string> STANDARD_HEADERS = {{"content-type", "Content-Type"},
																		{"content-transfer-encoding",
																		 "Content-Transfer-Encoding"},
																		{"content-disposition", "Content-Disposition"},
																		{"content-id", "Content-ID"},
																		{"content-description", "Content-Description"},
																		{"content-language", "Content-Language"},
																		{"content-location", "Content-Location"},
																		{"from", "From"},
																		{"to", "To"},
																		{"cc", "Cc"},
																		{"bcc", "Bcc"},
																		{"subject", "Subject"},
																		{"date", "Date"},
																		{"message-id", "Message-ID"},
																		{"in-reply-to", "In-Reply-To"},
																		{"references", "References"},
																		{"reply-to", "Reply-To"},
																		{"mime-version", "MIME-Version"},
																		{"user-agent", "User-Agent"},
																		{"x-mailer", "X-Mailer"}};

	auto it = STANDARD_HEADERS.find(lower_name);
	if (it != STANDARD_HEADERS.end()) return it->second;

	std::string result;
	result.reserve(name.size());
	bool capitalize_next = true;

	for (char c : name)
	{
		if (c == '-')
		{
			result.push_back(c);
			capitalize_next = true;
		}
		else if (capitalize_next)
		{
			result.push_back(std::toupper(c));
			capitalize_next = false;
		}
		else
		{
			result.push_back(std::tolower(c));
		}
	}

	return result;
}

std::string MimeUtils::ConvertToQuotedPrintableHeader(const std::string& text)
{
	bool needs_encoding = false;
	for (char c : text)
	{
		if (static_cast<unsigned char>(c) > 127)
		{
			needs_encoding = true;
			break;
		}
	}

	if (!needs_encoding)
	{
		return text;
	}

	std::stringstream result;
	result << "=?UTF-8?Q?";

	for (unsigned char c : text)
	{
		if (c == ' ')
		{
			result << '_';
		}
		else if (c > 127 || c == '=' || c == '?' || c == '_' || c == '\t' || c < 32)
		{
			result << '=' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
		}
		else
		{
			result << c;
		}
	}

	result << "?=";
	return result.str();
}

} // namespace ISXMime
