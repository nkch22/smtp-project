/**
 * @file MimeParser.hpp
 * @brief Defines the MimeParser class for parsing MIME entities from various sources.
 *
 * This file declares the MimeParser class which provides functionality for parsing
 * MIME entities from strings, streams, and files. It supports parsing complex MIME
 * structures including multipart messages, nested entities, and various encodings.
 *
 * @section usage_example Comprehensive Usage Example
 * @code
 * #include "MIME/MimeParser.hpp"
 * #include <iostream>
 * #include <fstream>
 * 
 * // Create a parser with default configuration
 * auto parser = ISXMime::CreateMimeParser();
 * 
 * // === PARSING FROM DIFFERENT SOURCES ===
 * 
 * // Parse from a string
 * std::string emailContent = "From: sender@example.com\r\n"
 *                           "To: recipient@example.com\r\n"
 *                           "Subject: Test Email\r\n"
 *                           "Content-Type: text/plain\r\n"
 *                           "\r\n"
 *                           "This is a test email.";
 * auto entity1 = parser->ParseString(emailContent);
 * 
 * // Parse from a file
 * auto entity2 = parser->ParseFile("email.eml");
 * 
 * // Parse from a stream
 * std::ifstream emailFile("another_email.eml");
 * auto entity3 = parser->ParseStream(emailFile);
 * 
 * // === ACCESSING PARSED CONTENT ===
 * 
 * // Get headers
 * auto& headers = entity1->GetHeader();
 * std::string from = headers.GetField("From");
 * std::string to = headers.GetField("To");
 * std::string subject = headers.GetField("Subject");
 * 
 * // Get content type
 * auto contentType = headers.GetContentType();
 * std::string mimeType = contentType.GetMimeType();
 * std::string charset = contentType.GetParameter("charset");
 * 
 * // Get body content
 * auto& body = entity1->GetBody();
 * std::string textContent = body.GetText();
 * 
 * // === WORKING WITH MULTIPART MESSAGES ===
 * 
 * // Check if this is a multipart message
 * if (contentType.IsMultipart()) {
 *     // Parse the multipart message into its constituent parts
 *     auto parts = parser->ParseMultipart(*entity1);
 *     
 *     // Process each part
 *     for (const auto& part : parts) {
 *         auto& partHeaders = part->GetHeader();
 *         auto partContentType = partHeaders.GetContentType();
 *         
 *         // Handle different part types
 *         if (partContentType.GetMimeType() == "text/plain") {
 *             std::cout << "Text part: " << part->GetBody().GetText() << std::endl;
 *         } else if (partContentType.GetMimeType() == "text/html") {
 *             std::cout << "HTML part: " << part->GetBody().GetText() << std::endl;
 *         } else if (partContentType.GetMimeType().find("image/") == 0) {
 *             // Handle image attachment
 *             auto& binaryData = part->GetBody().GetBinary();
 *             std::cout << "Image attachment: " << binaryData.size() << " bytes" << std::endl;
 *         }
 *     }
 * }
 * 
 * // === CUSTOMIZING PARSER BEHAVIOR ===
 * 
 * // Create a custom configuration
 * auto config = ISXMime::MimeParserConfig::CreateDefault();
 * config.SetStrictMode(true); // Be strict about RFC compliance
 * config.SetMaxLineLength(16 * 1024); // Allow longer lines (16KB)
 * config.SetMaxPartCount(200); // Allow more parts in multipart messages
 * 
 * // Create a parser with custom configuration
 * auto strictParser = std::make_shared<ISXMime::MimeParser>(config);
 * 
 * // Parse with the strict configuration
 * auto entity4 = strictParser->ParseFile("complex_email.eml");
 * @endcode
 *
 * @section advanced_example Advanced Usage with Nested Multiparts
 * @code
 * #include "MIME/MimeParser.hpp"
 * #include <iostream>
 * #include <stack>
 * 
 * void processEntity(const std::shared_ptr<ISXMime::MimeEntity>& entity, int depth = 0) {
 *     std::string indent(depth * 2, ' ');
 *     auto& headers = entity->GetHeader();
 *     auto contentType = headers.GetContentType();
 *     
 *     // Print entity information
 *     std::cout << indent << "Content-Type: " << contentType.ToString() << std::endl;
 *     
 *     if (contentType.IsMultipart()) {
 *         // Process multipart entity
 *         auto parser = ISXMime::CreateMimeParser();
 *         auto parts = parser->ParseMultipart(*entity);
 *         
 *         std::cout << indent << "Found " << parts.size() << " parts" << std::endl;
 *         
 *         // Recursively process each part
 *         for (const auto& part : parts) {
 *             processEntity(part, depth + 1);
 *         }
 *     } else {
 *         // Process leaf entity
 *         std::string disposition = headers.GetField("Content-Disposition");
 *         
 *         if (disposition.find("attachment") != std::string::npos) {
 *             // Handle attachment
 *             std::cout << indent << "Attachment: " 
 *                       << headers.GetContentDisposition().GetParameter("filename") 
 *                       << " (" << entity->GetBody().GetBinary().size() << " bytes)" 
 *                       << std::endl;
 *         } else if (contentType.GetMimeType() == "text/plain" || 
 *                   contentType.GetMimeType() == "text/html") {
 *             // Handle text content
 *             std::cout << indent << "Text content (" 
 *                       << contentType.GetMimeType() << "): " 
 *                       << entity->GetBody().GetText().substr(0, 40) << "..." 
 *                       << std::endl;
 *         } else {
 *             // Handle other content
 *             std::cout << indent << "Binary content: " 
 *                       << entity->GetBody().GetBinary().size() << " bytes" 
 *                       << std::endl;
 *         }
 *     }
 * }
 * 
 * // Parse and process a complex email
 * auto parser = ISXMime::CreateMimeParser();
 * auto entity = parser->ParseFile("complex_nested_email.eml");
 * processEntity(entity);
 * @endcode
 */

#pragma once

#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Body.hpp"
#include "ContentType.hpp"
#include "Header.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

/**
 * @class MimeParserError
 * @brief Exception class for MIME parsing errors.
 *
 * This exception is thrown when the parser encounters errors during
 * the parsing process, such as malformed MIME content, invalid headers,
 * or boundary issues.
 */
class MimeParserError : public MimeException
{
public:
	/**
	 * @brief Constructs a new MimeParserError with the given message.
	 * @param message Description of the error that occurred.
	 */
	explicit MimeParserError(const std::string& message) : MimeException(message) {}
};

// Forward declaration for configuration class
class MimeParserConfig;

/**
 * @class MimeParser
 * @brief Parser for MIME entities from various sources.
 *
 * MimeParser provides functionality to parse MIME entities from strings,
 * streams, and files. It handles complex MIME structures including multipart
 * messages, nested entities, and various content encodings. The parser can
 * be configured to control its behavior regarding RFC compliance, size limits,
 * and error handling.
 */
class MimeParser
{
public:
	/**
	 * @brief Default constructor.
	 *
	 * Creates a parser with default configuration settings.
	 */
	MimeParser();
	
	/**
	 * @brief Constructor with custom configuration.
	 * @param config Configuration settings for the parser.
	 *
	 * Creates a parser with the specified configuration settings,
	 * allowing customization of parsing behavior.
	 */
	explicit MimeParser(const MimeParserConfig& config);
	
	/**
	 * @brief Destructor.
	 *
	 * Ensures proper cleanup of the implementation.
	 */
	~MimeParser();

public:
	/**
	 * @brief Parses a MIME entity from a string.
	 * @param mimeContent String containing the MIME content.
	 * @return Shared pointer to the parsed MimeEntity.
	 * @throw MimeParserError If the content cannot be parsed successfully.
	 *
	 * This method parses a complete MIME entity from a string, including
	 * headers and body. It handles both simple and multipart entities.
	 */
	std::shared_ptr<MimeEntity> ParseString(const std::string& mimeContent);
	
	/**
	 * @brief Parses a MIME entity from a stream.
	 * @param stream Input stream containing the MIME content.
	 * @return Shared pointer to the parsed MimeEntity.
	 * @throw MimeParserError If the content cannot be parsed successfully.
	 *
	 * This method parses a complete MIME entity from an input stream, which
	 * can be useful for parsing large emails without loading them entirely
	 * into memory.
	 */
	std::shared_ptr<MimeEntity> ParseStream(std::istream& stream);
	
	/**
	 * @brief Parses a MIME entity from a file.
	 * @param filePath Path to the file containing the MIME content.
	 * @return Shared pointer to the parsed MimeEntity.
	 * @throw MimeParserError If the file cannot be opened or the content cannot be parsed.
	 *
	 * This method opens the specified file and parses its contents as a MIME entity.
	 * It's a convenience wrapper around ParseStream().
	 */
	std::shared_ptr<MimeEntity> ParseFile(const std::string& filePath);

public:
	/**
	 * @brief Parses a multipart MIME entity into its constituent parts.
	 * @param entity Multipart MIME entity to parse.
	 * @return Vector of shared pointers to the parsed MIME entities.
	 * @throw MimeParserError If the entity is not a multipart entity or cannot be parsed.
	 *
	 * This method extracts the boundary from the entity's Content-Type header
	 * and uses it to split the body into separate parts, then parses each part
	 * into a MimeEntity.
	 */
	std::vector<std::shared_ptr<MimeEntity>> ParseMultipart(const MimeEntity& entity);
	
	/**
	 * @brief Parses a single part of a multipart MIME message.
	 * @param part String containing the part content.
	 * @param boundary Boundary string used in the multipart message.
	 * @return Shared pointer to the parsed MimeEntity.
	 * @throw MimeParserError If the part cannot be parsed successfully.
	 *
	 * This method parses a single part extracted from a multipart MIME message.
	 * The boundary parameter is used for context in error messages.
	 */
	std::shared_ptr<MimeEntity> ParsePart(const std::string& part, const std::string& boundary);

	/**
	 * @brief Parses MIME headers from a stream.
	 * @param stream Input stream positioned at the start of headers.
	 * @return Parsed Header object.
	 * @throw MimeParserError If the headers cannot be parsed successfully.
	 *
	 * This method reads header fields from the stream until an empty line
	 * (indicating the end of headers) is encountered. It handles header
	 * field folding and basic validation according to RFC 5322.
	 */
	Header ParseHeader(std::istream& stream);
	
	/**
	 * @brief Parses a single header field line.
	 * @param line String containing the header field line.
	 * @return Optional pair of field name and value, or nullopt if the line is invalid.
	 *
	 * This method parses a single header field line into its name and value
	 * components, handling folded header fields according to RFC 5322.
	 */
	std::optional<std::pair<std::string, std::string>> ParseHeaderField(const std::string& line);

	/**
	 * @brief Parses the body of a MIME entity from a stream.
	 * @param stream Input stream positioned at the start of the body.
	 * @param header Headers of the MIME entity.
	 * @return Parsed Body object.
	 * @throw MimeParserError If the body cannot be parsed successfully.
	 *
	 * This method reads the body content from the stream, applying the
	 * appropriate handling based on the Content-Type and Content-Transfer-Encoding
	 * headers. For multipart entities, it reads until the end of the entity.
	 */
	Body ParseBody(std::istream& stream, const Header& header);
	
	/**
	 * @brief Parses a body with boundary from a stream.
	 * @param stream Input stream positioned at the start of the body.
	 * @param boundary Boundary string for the multipart content.
	 * @return Parsed Body object.
	 * @throw MimeParserError If the body cannot be parsed successfully.
	 *
	 * This method reads a multipart body from the stream, stopping when
	 * the specified boundary is encountered. It's used for parsing nested
	 * multipart entities.
	 */
	Body ParseBodyWithBoundary(std::istream& stream, const std::string& boundary);

public:
	/**
	 * @brief Sets the parser configuration.
	 * @param config New configuration settings.
	 *
	 * This method allows updating the parser's configuration after creation.
	 * The new settings will be applied to subsequent parsing operations.
	 */
	void SetConfig(const MimeParserConfig& config);
	
	/**
	 * @brief Gets the current parser configuration.
	 * @return Reference to the current configuration.
	 *
	 * This method provides access to the parser's current configuration,
	 * allowing inspection and modification of individual settings.
	 */
	const MimeParserConfig& GetConfig() const;

public:
	/**
	 * @brief Decodes an encoded header field.
	 * @param field Encoded header field value.
	 * @return Decoded string.
	 *
	 * This method decodes header fields encoded according to RFC 2047,
	 * which allows non-ASCII characters in header fields. It handles
	 * both "=?ISO-8859-1?Q?...?=" (quoted-printable) and "=?UTF-8?B?...?="
	 * (base64) encodings.
	 */
	std::string DecodeHeaderField(const std::string& field) const;
	
	/**
	 * @brief Decodes an encoded body.
	 * @param body Encoded body content.
	 * @param encoding Content-Transfer-Encoding value.
	 * @return Decoded binary data.
	 *
	 * This method decodes body content based on the specified encoding.
	 * It supports standard encodings such as "base64", "quoted-printable",
	 * "7bit", "8bit", and "binary".
	 */
	std::vector<uint8_t> DecodeBody(const std::string& body, const std::string& encoding) const;

private:
	/**
	 * @brief Reads a line from a stream.
	 * @param stream Input stream to read from.
	 * @return Line read from the stream, with CRLF removed.
	 * @throw MimeParserError If the line exceeds the maximum allowed length.
	 *
	 * This helper method reads a line from the stream, handling both CRLF
	 * and LF line endings. It enforces the maximum line length specified
	 * in the configuration.
	 */
	std::string ReadLine(std::istream& stream) const;
	
	/**
	 * @brief Reads content from a stream until a boundary is encountered.
	 * @param stream Input stream to read from.
	 * @param boundary Boundary string to stop at.
	 * @return Content read from the stream.
	 * @throw MimeParserError If the boundary is not found or content exceeds limits.
	 *
	 * This helper method reads content from the stream until the specified
	 * boundary is encountered. It's used for extracting parts from multipart
	 * entities.
	 */
	std::string ReadUntilBoundary(std::istream& stream, const std::string& boundary) const;
	
	/**
	 * @brief Checks if a line is an end boundary.
	 * @param line Line to check.
	 * @param boundary Boundary string.
	 * @return True if the line is an end boundary, false otherwise.
	 *
	 * This helper method checks if a line matches the end boundary format
	 * (--boundary--). End boundaries indicate the end of a multipart entity.
	 */
	bool IsEndBoundary(const std::string& line, const std::string& boundary) const;
	
	/**
	 * @brief Checks if a line is a boundary.
	 * @param line Line to check.
	 * @param boundary Boundary string.
	 * @return True if the line is a boundary, false otherwise.
	 *
	 * This helper method checks if a line matches the boundary format
	 * (--boundary). Boundaries separate parts in a multipart entity.
	 */
	bool IsBoundary(const std::string& line, const std::string& boundary) const;
	
	/**
	 * @brief Extracts the boundary from a Content-Type header.
	 * @param contentType Content-Type header.
	 * @return Boundary string.
	 * @throw MimeParserError If the boundary parameter is missing or invalid.
	 *
	 * This helper method extracts the boundary parameter from a multipart
	 * Content-Type header. The boundary is needed to parse multipart entities.
	 */
	std::string ExtractBoundary(const ContentType& contentType) const;

private:
	/**
	 * @brief Private implementation class.
	 *
	 * This class holds the actual implementation details of the MimeParser,
	 * following the pimpl idiom to hide implementation details and reduce
	 * compilation dependencies.
	 */
	class Impl;
	
	/**
	 * @brief Pointer to the implementation.
	 *
	 * This unique pointer holds the implementation of the MimeParser,
	 * following the pimpl idiom.
	 */
	std::unique_ptr<Impl> m_p_impl;
};

/**
 * @class MimeParserConfig
 * @brief Configuration settings for the MimeParser.
 *
 * This class holds configuration settings that control the behavior
 * of the MimeParser, such as size limits, recursion limits, and
 * error handling behavior.
 */
class MimeParserConfig
{
public:
	/**
	 * @brief Default constructor.
	 *
	 * Creates a configuration with default settings.
	 */
	MimeParserConfig();

	/**
	 * @brief Sets the strict mode flag.
	 * @param strict True to enable strict RFC compliance, false for lenient parsing.
	 *
	 * In strict mode, the parser enforces strict RFC compliance and throws
	 * exceptions for any violations. In non-strict mode, the parser attempts
	 * to recover from minor issues.
	 */
	void SetStrictMode(bool strict);
	
	/**
	 * @brief Checks if strict mode is enabled.
	 * @return True if strict mode is enabled, false otherwise.
	 */
	bool IsStrictMode() const;

	/**
	 * @brief Sets the maximum header size.
	 * @param size Maximum size in bytes.
	 *
	 * This setting limits the total size of headers to prevent excessive
	 * memory usage when parsing malicious or malformed messages.
	 */
	void SetMaxHeaderSize(size_t size);
	
	/**
	 * @brief Gets the maximum header size.
	 * @return Maximum header size in bytes.
	 */
	size_t GetMaxHeaderSize() const;

	/**
	 * @brief Sets the maximum line length.
	 * @param length Maximum length in bytes.
	 *
	 * This setting limits the length of individual lines to prevent
	 * excessive memory usage when parsing malicious or malformed messages.
	 */
	void SetMaxLineLength(size_t length);
	
	/**
	 * @brief Gets the maximum line length.
	 * @return Maximum line length in bytes.
	 */
	size_t GetMaxLineLength() const;

	/**
	 * @brief Sets the maximum number of parts in a multipart message.
	 * @param count Maximum number of parts.
	 *
	 * This setting limits the number of parts that can be extracted from
	 * a multipart message to prevent excessive resource usage.
	 */
	void SetMaxPartCount(size_t count);
	
	/**
	 * @brief Gets the maximum number of parts.
	 * @return Maximum number of parts.
	 */
	size_t GetMaxPartCount() const;

	/**
	 * @brief Sets the recursion limit for nested multipart entities.
	 * @param limit Maximum recursion depth.
	 *
	 * This setting limits the depth of nested multipart entities to
	 * prevent stack overflow and excessive resource usage.
	 */
	void SetRecursionLimit(size_t limit);
	
	/**
	 * @brief Gets the recursion limit.
	 * @return Maximum recursion depth.
	 */
	size_t GetRecursionLimit() const;

	/**
	 * @brief Sets whether to ignore invalid headers.
	 * @param ignore True to ignore invalid headers, false to throw exceptions.
	 *
	 * This setting controls whether the parser should ignore invalid header
	 * fields or throw exceptions. In strict mode, this setting is ignored
	 * and exceptions are always thrown for invalid headers.
	 */
	void SetIgnoreInvalidHeaders(bool ignore);
	
	/**
	 * @brief Checks if invalid headers should be ignored.
	 * @return True if invalid headers should be ignored, false otherwise.
	 */
	bool ShouldIgnoreInvalidHeaders() const;

	/**
	 * @brief Creates a configuration with default settings.
	 * @return Default MimeParserConfig.
	 *
	 * This static factory method creates a configuration with reasonable
	 * default settings suitable for most parsing scenarios.
	 */
	static MimeParserConfig CreateDefault();

private:
	bool m_strict_mode;            ///< Whether to enforce strict RFC compliance
	size_t m_max_header_size;      ///< Maximum size of all headers combined
	size_t m_max_line_length;      ///< Maximum length of a single line
	size_t m_max_part_count;       ///< Maximum number of parts in a multipart message
	size_t m_recursion_limit;      ///< Maximum depth of nested multipart entities
	bool m_ignore_invalid_headers; ///< Whether to ignore invalid headers
};

/**
 * @brief Creates a MimeParser with default configuration.
 * @return Shared pointer to a new MimeParser instance.
 *
 * Factory function that creates a new MimeParser with default configuration.
 * This is the recommended way to create a parser for most use cases.
 */
std::shared_ptr<MimeParser> CreateMimeParser();

} // namespace ISXMime
