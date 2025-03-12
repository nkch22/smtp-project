/**
 * @file MimeBuilder.hpp
 * @brief Defines the MimeBuilder class for creating MIME entities with a fluent interface.
 *
 * This file declares the MimeBuilder class which provides a builder pattern interface
 * for creating MIME entities for emails, supporting various email types including
 * simple text, HTML, multipart/alternative, multipart/mixed, and multipart/related.
 *
 * @section usage_example Comprehensive Usage Example
 * @code
 * #include "MIME/MimeBuilder.hpp"
 * #include <iostream>
 *
 * auto mimeBuilder = ISXMime::MimeBuilder::Create();
 *
 * // === REQUIRED COMPONENTS ===
 * // The From and To headers are required for a valid email per RFC 5322
 * mimeBuilder.From("John Doe <sender@example.com>")
 *            .To("Jane Smith <recipient@example.com>");
 *
 * // At least one body component (text or HTML) is required
 * mimeBuilder.TextBody("This is the plain text version of the email.");
 *
 * // === OPTIONAL COMPONENTS (but commonly used) ===
 * // Subject is technically optional but practically essential
 * mimeBuilder.Subject("Comprehensive MIME Builder Example");
 *
 * // Optional recipients
 * mimeBuilder.Cc("cc-recipient@example.com")
 *            .Bcc("bcc-recipient@example.com")
 *            .ReplyTo("reply@example.com");
 *
 * // Optional custom headers
 * mimeBuilder.AddCustomHeader("X-Priority", "1")
 *            .AddCustomHeader("X-Mailer", "ISXMime Library");
 *
 * // Optional HTML alternative (adding this will automatically create multipart/alternative)
 * mimeBuilder.HtmlBody("<html><body>"
 *                      "<h1>Rich HTML Email</h1>"
 *                      "<p>This is the <b>HTML version</b> of the email.</p>"
 *                      "<p>Here's our company logo:</p>"
 *                      "<img src=\"cid:logo\" alt=\"Company Logo\">"
 *                      "</body></html>");
 *
 * // Optional attachments (adding these will automatically create multipart/mixed)
 * mimeBuilder.AddAttachment("document.pdf")
 *            .AddAttachment("spreadsheet.xlsx");
 *
 * // Optional inline attachments (adding these with HTML will create multipart/related)
 * mimeBuilder.AddInlineAttachment("logo.png", "logo");
 *
 * // === BUILDING THE EMAIL ===
 * // Validate and build the complete MIME entity
 * // This will throw std::runtime_error if required components are missing
 * auto email = mimeBuilder.Build();
 *
 * // Output options
 * mimeBuilder.SaveToFile("email.eml");       // Save to file
 * std::string emailStr = mimeBuilder.ToString(); // Get as string for sending
 * @endcode
 *
 * @section chained_example Concise Method Chaining Example
 * @code
 * #include "MIME/MimeBuilder.hpp"
 *
 * // Create and build an email in a single expression with method chaining
 * auto email = ISXMime::MimeBuilder::Create()
 *     .From("sender@example.com")
 *     .To("recipient@example.com")
 *     .Subject("Meeting Tomorrow")
 *     .TextBody("Hi Jane, let's meet tomorrow at 2 PM to discuss the project.")
 *     .HtmlBody("<html><body>"
 *               "<h3>Meeting Tomorrow</h3>"
 *               "<p>Hi Jane,</p>"
 *               "<p>Let's meet tomorrow at <strong>2 PM</strong> to discuss the project.</p>"
 *               "<p>I've attached the <a href=\"cid:agenda\">agenda</a>.</p>"
 *               "</body></html>")
 *     .AddAttachment("project_specs.pdf")
 *     .AddInlineAttachment("meeting_agenda.pdf", "agenda")
 *     .Build();
 *
 * // Or directly save without storing the intermediate object
 * ISXMime::MimeBuilder::Create()
 *     .From("notifications@example.com")
 *     .To("user@example.com")
 *     .Subject("Your Account Statement")
 *     .TextBody("Your monthly account statement is attached.")
 *     .AddAttachment("statement.pdf")
 *     .SaveToFile("account_statement.eml");
 * @endcode
 *
 * Required components for a valid email:
 * - From header (RFC 5322 requirement)
 * - To header (RFC 5322 requirement)
 * - At least one body part (text or HTML)
 *
 * Optional components:
 * - Subject header (technically optional but practically essential)
 * - Cc, Bcc, Reply-To headers
 * - Custom headers (X-headers)
 * - HTML alternative body (creates multipart/alternative)
 * - Attachments (creates multipart/mixed)
 * - Inline attachments (creates multipart/related)
 *
 * The MimeBuilder::Build() method will validate that all required components
 * are present and throw std::runtime_error if any are missing. The email type
 * and structure are automatically determined based on the components added,
 * though they can be manually specified using SetEmailType() if needed.
 */

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

/**
 * @enum EmailType
 * @brief Defines the types of email structures that can be created.
 *
 * This enum represents the different structural formats that emails can take.
 * The type determines how the MIME hierarchy is constructed and which Content-Type
 * headers are used. The builder will automatically select an appropriate type
 * based on the content added, but it can also be manually specified.
 */
enum class EmailType
{
	SIMPLE,		 ///< Single part (text or html only) - For basic emails with just one content type
	ALTERNATIVE, ///< Multipart/alternative (text and html versions) - For providing both plain text and HTML versions
				 ///< of the same content
	MIXED,	///< Multipart/mixed (with attachments) - For emails with attachments alongside the main content
	RELATED ///< Multipart/related (with inline attachments) - For HTML emails with embedded images or other resources
};

/**
 * @class MimeBuilder
 * @brief Builder class for creating MIME entities with a fluent interface.
 *
 * MimeBuilder provides a convenient way to build MIME entities for emails
 * with support for various email types, headers, body content, and attachments.
 * It follows the builder pattern with a fluent interface for easy chaining of
 * method calls.
 */
class MimeBuilder
{
public:
	/**
	 * @brief Sets the sender of the email.
	 * @param from Email address of the sender.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * This sets the "From" header of the email. According to RFC 5322,
	 * the From header contains the address of the sender of the message.
	 * The format should follow the "name <email@domain.com>" pattern or
	 * just "email@domain.com".
	 */
	MimeBuilder& From(const std::string& from);

	/**
	 * @brief Sets the recipient of the email.
	 * @param to Email address of the recipient.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * This sets the "To" header of the email. Multiple recipients can be
	 * specified by separating addresses with commas. Both "name <email@domain.com>"
	 * and simple "email@domain.com" formats are supported.
	 */
	MimeBuilder& To(const std::string& to);

	/**
	 * @brief Sets the subject of the email.
	 * @param subject Subject line of the email.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * The subject will be automatically encoded if it contains non-ASCII characters,
	 * following the MIME encoded-word syntax (RFC 2047).
	 */
	MimeBuilder& Subject(const std::string& subject);

	/**
	 * @brief Sets the CC recipients of the email.
	 * @param cc Email addresses for CC recipients.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * CC (Carbon Copy) recipients receive a copy of the email, and their
	 * addresses are visible to all other recipients.
	 */
	MimeBuilder& Cc(const std::string& cc);

	/**
	 * @brief Sets the BCC recipients of the email.
	 * @param bcc Email addresses for BCC recipients.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * BCC (Blind Carbon Copy) recipients receive a copy of the email, but their
	 * addresses are not visible to other recipients. The BCC header is typically
	 * stripped by the mail server before delivery.
	 */
	MimeBuilder& Bcc(const std::string& bcc);

	/**
	 * @brief Sets the Reply-To address of the email.
	 * @param replyTo Email address for replies.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * The Reply-To header indicates the address(es) to which the sender prefers
	 * replies to be sent, which may be different from the From address.
	 */
	MimeBuilder& ReplyTo(const std::string& replyTo);

	/**
	 * @brief Adds a custom header to the email.
	 * @param name Name of the header.
	 * @param value Value of the header.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * This allows adding any custom header to the email. Standard headers
	 * (From, To, Subject, etc.) should be set using their dedicated methods.
	 * Header names are case-insensitive according to the RFC, but this method
	 * preserves the case provided.
	 */
	MimeBuilder& AddCustomHeader(const std::string& name, const std::string& value);

public:
	/**
	 * @brief Sets the plain text body of the email.
	 * @param text Plain text content.
	 * @param charset Character set of the text content (default: UTF-8).
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * This method sets the plain text version of the email body. If both
	 * text and HTML bodies are provided, the builder will automatically create
	 * a multipart/alternative structure. The text is encoded using quoted-printable
	 * encoding to ensure safe transport.
	 */
	MimeBuilder& TextBody(const std::string& text, const std::string& charset = "UTF-8");

	/**
	 * @brief Sets the HTML body of the email.
	 * @param html HTML content.
	 * @param charset Character set of the HTML content (default: UTF-8).
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * This method sets the HTML version of the email body. If both text and HTML
	 * bodies are provided, the builder will automatically create a multipart/alternative
	 * structure. The HTML is encoded using quoted-printable encoding.
	 *
	 * Modern email clients prefer HTML content, but it's good practice to always
	 * include a plain text alternative for accessibility and compatibility.
	 */
	MimeBuilder& HtmlBody(const std::string& html, const std::string& charset = "UTF-8");

public:
	/**
	 * @brief Adds a file attachment to the email.
	 * @param filePath Path to the file to attach.
	 * @return Reference to this MimeBuilder for method chaining.
	 * @throw std::runtime_error If the file cannot be read.
	 *
	 * This method reads a file from disk and adds it as an attachment to the email.
	 * The file's MIME type is automatically detected based on the file extension.
	 * Attachments are encoded using Base64 encoding for safe binary transport.
	 *
	 * Adding attachments automatically converts the email to multipart/mixed
	 * structure if not already set to a multipart type.
	 */
	MimeBuilder& AddAttachment(const std::string& filePath);

	/**
	 * @brief Adds an attachment to the email from memory.
	 * @param fileName Name to use for the attachment.
	 * @param data Binary data for the attachment.
	 * @param mimeType MIME type of the attachment (default: application/octet-stream).
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * This method adds binary data from memory as an attachment. This is useful
	 * for dynamically generated content that doesn't exist as a file on disk.
	 * The provided filename is used in the Content-Disposition header.
	 */
	MimeBuilder& AddAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
							   const std::string& mimeType = "application/octet-stream");

	/**
	 * @brief Adds an inline attachment from a file.
	 * @param filePath Path to the file to attach.
	 * @param contentId Content ID to reference the attachment in HTML.
	 * @return Reference to this MimeBuilder for method chaining.
	 * @throw std::runtime_error If the file cannot be read.
	 *
	 * Inline attachments are typically used for embedding images in HTML emails.
	 * The contentId parameter is used to reference the attachment from the HTML
	 * using the "cid:" URI scheme, e.g., <img src="cid:image1">.
	 *
	 * Adding inline attachments automatically converts the email to multipart/related
	 * structure if an HTML body is present.
	 */
	MimeBuilder& AddInlineAttachment(const std::string& filePath, const std::string& contentId);

	/**
	 * @brief Adds an inline attachment from memory.
	 * @param fileName Name to use for the attachment.
	 * @param data Binary data for the attachment.
	 * @param contentId Content ID to reference the attachment in HTML.
	 * @param mimeType MIME type of the attachment (default: application/octet-stream).
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * Similar to AddInlineAttachment(filePath, contentId), but accepts binary data
	 * directly rather than reading from a file. This is useful for dynamically generated
	 * content like charts or graphs that are embedded in the HTML.
	 */
	MimeBuilder& AddInlineAttachment(const std::string& fileName, const std::vector<uint8_t>& data,
									 const std::string& contentId,
									 const std::string& mimeType = "application/octet-stream");

public:
	/**
	 * @brief Explicitly sets the email type.
	 * @param type Type of email to create.
	 * @return Reference to this MimeBuilder for method chaining.
	 *
	 * By default, the builder automatically determines the appropriate email type
	 * based on the content added (text only, HTML only, with attachments, etc.).
	 * This method allows manually overriding that behavior when needed.
	 *
	 * Manual setting may be necessary in complex scenarios where the automatic
	 * determination doesn't produce the desired structure.
	 */
	MimeBuilder& SetEmailType(EmailType type);

	/**
	 * @brief Gets the current email type.
	 * @return Current email type.
	 *
	 * Useful for checking what type the builder has determined based on the
	 * current content or what was manually set.
	 */
	EmailType GetEmailType() const;

public:
	/**
	 * @brief Builds the MIME entity with the configured settings.
	 * @return Shared pointer to the constructed MimeEntity.
	 * @throw std::runtime_error If required fields are missing.
	 *
	 * This method finalizes the email construction process and returns a fully
	 * constructed MIME entity representing the email. The structure of the entity
	 * depends on the email type, which is either explicitly set or automatically
	 * determined based on content.
	 *
	 * The build process:
	 * 1. Validates required fields (From, To, etc.)
	 * 2. Determines the appropriate email type if not manually set
	 * 3. Constructs the appropriate MIME hierarchy based on the type
	 * 4. Adds all headers, content, and attachments
	 * 5. Sets appropriate Content-Type headers and boundaries
	 * 6. Returns the complete entity structure
	 */
	std::shared_ptr<MimeEntity> Build();

	/**
	 * @brief Converts the MIME entity to a string.
	 * @return String representation of the MIME entity.
	 * @throw std::runtime_error If required fields are missing.
	 *
	 * This is a convenience method that builds the MIME entity and then
	 * converts it to a string representation suitable for sending or saving.
	 * The string contains the complete email with all headers and properly
	 * formatted content according to MIME standards.
	 */
	std::string ToString();

	/**
	 * @brief Saves the MIME entity to a file.
	 * @param filePath Path to save the MIME entity to.
	 * @return True if successful, false otherwise.
	 * @throw std::runtime_error If required fields are missing.
	 *
	 * This is a convenience method that builds the MIME entity and saves it
	 * to a file. This can be useful for debugging or for creating email templates.
	 */
	bool SaveToFile(const std::string& filePath);

public:
	/**
	 * @brief Creates a new MimeBuilder instance.
	 * @return New MimeBuilder instance.
	 *
	 * Factory method that creates a new builder with default settings.
	 * The email type will be automatically determined based on the content
	 * added unless explicitly set.
	 */
	static MimeBuilder Create();

	/**
	 * @brief Creates a MimeBuilder instance for text-only emails.
	 * @return MimeBuilder instance with EmailType::SIMPLE.
	 *
	 * Factory method that creates a builder pre-configured for text-only emails.
	 * This is equivalent to calling Create() followed by SetEmailType(EmailType::SIMPLE),
	 * but provides more semantic clarity when the intent is known upfront.
	 */
	static MimeBuilder CreateTextEmail();

	/**
	 * @brief Creates a MimeBuilder instance for HTML-only emails.
	 * @return MimeBuilder instance with EmailType::SIMPLE.
	 *
	 * Factory method that creates a builder pre-configured for HTML-only emails.
	 * While technically using EmailType::SIMPLE (as it's still a single part),
	 * this method exists to provide semantic clarity when creating HTML emails.
	 */
	static MimeBuilder CreateHtmlEmail();

	/**
	 * @brief Creates a MimeBuilder instance for multipart emails.
	 * @return MimeBuilder instance with EmailType::ALTERNATIVE.
	 *
	 * Factory method that creates a builder pre-configured for multipart/alternative
	 * emails, which typically contain both text and HTML versions of the content.
	 * This is the recommended approach for maximum compatibility.
	 */
	static MimeBuilder CreateMultipartEmail();

private:
	/**
	 * @struct EmailContent
	 * @brief Contains the text and HTML content of the email.
	 *
	 * This structure encapsulates the content versions of the email with their
	 * respective character sets. Using std::optional allows tracking whether
	 * each version has been set.
	 */
	struct EmailContent
	{
		std::optional<std::string> text_body;	 ///< Plain text body content
		std::optional<std::string> text_charset; ///< Character set for text body
		std::optional<std::string> html_body;	 ///< HTML body content
		std::optional<std::string> html_charset; ///< Character set for HTML body
	};
	EmailContent m_content; ///< Storage for email body content

	/**
	 * @struct Attachment
	 * @brief Contains information about an email attachment.
	 *
	 * This structure holds all necessary data for an attachment, including
	 * its content, name, MIME type, and for inline attachments, a content ID.
	 * The is_inline flag determines whether the attachment should be treated
	 * as an inline attachment or a regular attachment.
	 */
	struct Attachment
	{
		std::string file_name;				   ///< Name of the attachment file
		std::vector<uint8_t> data;			   ///< Binary content of the attachment
		std::string mime_type;				   ///< MIME type of the attachment
		std::optional<std::string> content_id; ///< Content ID for inline attachments
		bool is_inline;						   ///< Whether the attachment is inline
	};
	std::vector<Attachment> m_attachments; ///< List of email attachments

private:
	/**
	 * @brief Default constructor.
	 *
	 * Initializes a new MimeBuilder with default settings.
	 * Email type is initially set to SIMPLE and will be automatically
	 * updated based on content unless manually overridden.
	 */
	MimeBuilder();

private:
	std::string m_from;									 ///< From header value
	std::string m_to;									 ///< To header value
	std::string m_subject;								 ///< Subject header value
	std::string m_cc;									 ///< CC header value
	std::string m_bcc;									 ///< BCC header value
	std::string m_reply_to;								 ///< Reply-To header value
	std::map<std::string, std::string> m_custom_headers; ///< Custom headers
	EmailType m_email_type;								 ///< Type of email to create
	bool m_type_manually_set;							 ///< Whether type was manually set

private:
	/**
	 * @brief Adds standard email headers to a MIME entity.
	 * @param entity MIME entity to add headers to.
	 *
	 * This helper method adds all standard headers (From, To, Subject, etc.)
	 * and any custom headers to the provided entity. This is used during the
	 * build process to ensure all entities have the necessary headers.
	 */
	void AddStandardHeaders(std::shared_ptr<MimeEntity> entity);

	/**
	 * @brief Builds a text-only email.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * Creates a simple email with a plain text body. The content is encoded
	 * using quoted-printable encoding for compatibility. This is used internally
	 * by the Build method when the email type is SIMPLE and only text content is present.
	 */
	std::shared_ptr<MimeEntity> BuildTextOnlyEmail();

	/**
	 * @brief Builds an HTML-only email.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * Creates a simple email with an HTML body. The content is encoded
	 * using quoted-printable encoding for compatibility. This is used internally
	 * by the Build method when the email type is SIMPLE and only HTML content is present.
	 */
	std::shared_ptr<MimeEntity> BuildHtmlOnlyEmail();

	/**
	 * @brief Builds a multipart/alternative email with text and HTML versions.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * Creates an email with both text and HTML versions of the content.
	 * The multipart/alternative type indicates that each part is an alternative
	 * version of the same content, and the email client should choose the
	 * most appropriate one to display. Parts are typically ordered from
	 * least rich to most rich, so text comes before HTML.
	 */
	std::shared_ptr<MimeEntity> BuildMultipartAlternativeEmail();

	/**
	 * @brief Builds a multipart/mixed email with attachments.
	 * @param contentEntity MIME entity containing the email content.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * Creates an email with content and attachments. The multipart/mixed type
	 * indicates that each part is a separate piece of content that should be
	 * handled independently. This is used when attachments are present.
	 *
	 * The contentEntity parameter can be any kind of entity, including another
	 * multipart entity like multipart/alternative, allowing for complex nested
	 * structures.
	 */
	std::shared_ptr<MimeEntity> BuildMultipartMixedEmail(std::shared_ptr<MimeEntity> contentEntity);

	/**
	 * @brief Builds a multipart/related email with inline attachments.
	 * @param contentEntity MIME entity containing the email content.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * Creates an email with content and inline attachments. The multipart/related
	 * type indicates that each part is related to the main content and might be
	 * referenced from it. This is typically used for HTML emails with embedded images.
	 *
	 * The contentEntity parameter is usually an HTML entity or a multipart/alternative
	 * entity containing HTML. The inline attachments are referenced from the HTML
	 * using Content-ID references.
	 */
	std::shared_ptr<MimeEntity> BuildMultipartRelatedEmail(std::shared_ptr<MimeEntity> contentEntity);

	/**
	 * @brief Creates a MIME entity for an attachment.
	 * @param attachment Attachment information.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * Creates a MIME entity for an attachment with appropriate headers and content.
	 * The attachment is encoded using Base64 encoding for binary safety.
	 * This method sets the Content-Type, Content-Disposition, and Content-ID
	 * (for inline attachments) headers.
	 */
	std::shared_ptr<MimeEntity> CreateAttachmentEntity(const Attachment& attachment);

	/**
	 * @brief Validates that all required fields are set.
	 * @throw std::runtime_error If required fields are missing.
	 *
	 * Checks that mandatory fields like From, To, and at least one body
	 * (text or HTML) are set before building the email. This helps prevent
	 * creation of invalid emails. If any required field is missing, an
	 * exception is thrown with a descriptive message.
	 */
	void ValidateRequiredFields() const;

private:
	/**
	 * @brief Updates the email type based on content and attachments.
	 *
	 * This method automatically determines the appropriate email type based on
	 * the current content and attachments, unless the type has been manually set.
	 *
	 * The logic follows these rules:
	 * - If only text or only HTML is present -> SIMPLE
	 * - If both text and HTML are present -> ALTERNATIVE
	 * - If inline attachments are present and HTML is present -> RELATED
	 * - If regular attachments are present -> MIXED
	 *
	 * This method is called by Build() if m_type_manually_set is false.
	 */
	void UpdateEmailType();

	/**
	 * @brief Builds a MIME entity based on the current email type.
	 * @return Shared pointer to the constructed MimeEntity.
	 *
	 * This method delegates to the appropriate build method based on the
	 * current email type. It's the main entry point for the building process
	 * and is called by Build() after validating the required fields and
	 * updating the email type if necessary.
	 */
	std::shared_ptr<MimeEntity> BuildBasedOnType();
};
} // namespace ISXMime
