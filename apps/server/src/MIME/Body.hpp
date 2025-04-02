/**
 * @file Body.hpp
 * @brief Defines the Body class for handling MIME body content and multipart structures.
 *
 * This file declares the Body class which provides functionality for handling MIME body
 * content, including single-part plain bodies as well as complex multipart structures.
 * It supports various multipart types like mixed, alternative, and related, and provides
 * methods for working with MIME parts, boundaries, and content.
 * 
 * The Body class is a fundamental component of the MIME architecture, responsible for
 * storing and manipulating the actual content of messages, separate from their headers.
 */

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

/**
 * @enum MultipartType
 * @brief Defines the types of multipart MIME structures.
 *
 * This enum represents the different multipart body types as defined in the MIME standards.
 * Each type has specific semantics for how the contained parts should be interpreted.
 * The multipart type affects how email clients and other MIME-aware applications
 * display and process the content.
 */
enum class MultipartType
{
	NONE,		 ///< Not a multipart body
	MIXED,		 ///< Independent body parts with different content types (RFC 2046)
	ALTERNATIVE, ///< Different representations of the same content (RFC 2046)
	RELATED,	 ///< Related body parts that reference each other (RFC 2387)
	DIGEST,		 ///< Collection of messages, default content-type is message/rfc822 (RFC 2046)
	FORM,		 ///< Form data submission (RFC 7578)
	REPORT,		 ///< Delivery status reports and similar (RFC 6522)
	SIGNED,		 ///< Cryptographically signed content (RFC 1847)
	ENCRYPTED	 ///< Encrypted content (RFC 1847)
};

/**
 * @class Body
 * @brief Represents the body part of a MIME entity.
 *
 * The Body class handles the content portion of a MIME entity, supporting both
 * simple text content and complex multipart structures. It provides methods for
 * manipulating the body content, managing multipart boundaries, and accessing
 * individual parts within multipart bodies.
 * 
 * The class uses the PIMPL idiom to hide implementation details, which allows
 * for ABI compatibility and reduces compilation dependencies. It manages both
 * textual and binary content, and provides specialized methods for working with
 * common MIME types like text/plain and text/html.
 * 
 * For multipart bodies, it maintains the boundary string, preamble, epilogue,
 * and a collection of child parts, each represented as a MimeEntity.
 */
class Body
{
public:
	/**
	 * @brief Default constructor.
	 *
	 * Creates an empty Body object with no content or parts. The multipart type
	 * is initialized to NONE and no boundary is set. This constructor creates a
	 * minimal body that can be populated later with content or parts.
	 */
	Body();

	/**
	 * @brief Destructor.
	 *
	 * Properly cleans up resources used by the Body object, including all
	 * implementation details managed by the PIMPL idiom.
	 */
	~Body();

	/**
	 * @brief Deleted copy constructor.
	 *
	 * Body objects cannot be copied to prevent unintended duplication of potentially
	 * large content. If you need a copy of a Body, you must explicitly create a new
	 * Body and populate it with the content of the original.
	 */
	Body(const Body&) = delete;

	/**
	 * @brief Deleted copy assignment operator.
	 *
	 * Body objects cannot be copied to prevent unintended duplication of potentially
	 * large content. If you need a copy of a Body, you must explicitly create a new
	 * Body and populate it with the content of the original.
	 */
	Body& operator=(const Body&) = delete;

	/**
	 * @brief Move constructor.
	 *
	 * Allows efficient transfer of resources from one Body object to another.
	 * After the move operation, the source object is left in a valid but unspecified state.
	 * All resources (content, parts, etc.) are transferred to the new object.
	 */
	Body(Body&&) noexcept;

	/**
	 * @brief Move assignment operator.
	 *
	 * Allows efficient transfer of resources from one Body object to another.
	 * After the move operation, the source object is left in a valid but unspecified state.
	 * All resources (content, parts, etc.) are transferred to the target object.
	 */
	Body& operator=(Body&&) noexcept;

public:
	/**
	 * @brief Sets the content of the body.
	 * @param content String content to set.
	 *
	 * Sets the plain text or raw content of the body. For multipart bodies,
	 * this typically contains the entire multipart structure including boundaries.
	 * 
	 * Note that simply setting content directly for multipart bodies may not
	 * properly set up the internal structures. For building multipart bodies,
	 * consider using the CreateMultipartBody() method instead.
	 */
	void Set(const std::string& content);

	/**
	 * @brief Gets the content of the body as a const reference.
	 * @return Const reference to the body content.
	 *
	 * Provides read-only access to the body content. This is useful for
	 * examining the content without modifying it. For multipart bodies,
	 * this will return the fully formatted multipart content including
	 * all boundaries and parts.
	 */
	const std::string& Content() const;

	/**
	 * @brief Gets the content of the body as a mutable reference.
	 * @return Mutable reference to the body content.
	 *
	 * Provides read-write access to the body content for direct manipulation.
	 * Use with caution for multipart bodies, as manual modifications could
	 * invalidate the structure. For structured modifications of multipart
	 * bodies, consider using the specialized methods provided by this class.
	 */
	std::string& Content();

	/**
	 * @brief Loads body content from a file.
	 * @param filename Path to the file to load.
	 * @return True if successful, false if the file couldn't be opened or read.
	 * @throw std::runtime_error If the file path is invalid or inaccessible.
	 *
	 * Reads the entire content of the specified file and sets it as the body content.
	 * This method is useful for loading message templates, attachments, or other
	 * file-based content. For large files, be aware of memory considerations.
	 */
	bool Load(const std::string& filename);

	/**
	 * @brief Saves body content to a file.
	 * @param filename Path to save the content to.
	 * @return True if successful, false if the file couldn't be opened or written.
	 * @throw std::runtime_error If the file path is invalid or inaccessible.
	 *
	 * Writes the current body content to the specified file. This is useful for
	 * debugging, archiving, or exporting body content. For multipart bodies,
	 * this will save the fully formatted multipart content.
	 */
	bool Save(const std::string& filename) const;

public:
	/**
	 * @brief Sets the preamble text for multipart bodies.
	 * @param preamble Preamble text to set.
	 *
	 * The preamble appears before the first boundary in a multipart body and is typically
	 * ignored by MIME-compliant clients. It's often used for compatibility messages for
	 * non-MIME clients, such as "This is a MIME-formatted message."
	 * 
	 * This text is only relevant for multipart bodies and is ignored for single-part bodies.
	 */
	void SetPreamble(const std::string& preamble);

	/**
	 * @brief Gets the preamble text.
	 * @return The current preamble text.
	 *
	 * Returns the preamble text set for this body. If no preamble has been set,
	 * returns an empty string. This is primarily used for multipart bodies.
	 */
	const std::string& Preamble() const;

	/**
	 * @brief Sets the epilogue text for multipart bodies.
	 * @param epilogue Epilogue text to set.
	 *
	 * The epilogue appears after the final boundary in a multipart body and is typically
	 * ignored by MIME-compliant clients. Like the preamble, it's mainly used for
	 * compatibility with non-MIME clients or to include additional information that
	 * is not part of the formal message structure.
	 * 
	 * This text is only relevant for multipart bodies and is ignored for single-part bodies.
	 */
	void SetEpilogue(const std::string& epilogue);

	/**
	 * @brief Gets the epilogue text.
	 * @return The current epilogue text.
	 *
	 * Returns the epilogue text set for this body. If no epilogue has been set,
	 * returns an empty string. This is primarily used for multipart bodies.
	 */
	const std::string& Epilogue() const;

	/**
	 * @brief Sets the boundary string for multipart bodies.
	 * @param boundary Boundary string to set.
	 *
	 * The boundary string delimits parts in a multipart body. It must not appear within
	 * any of the parts. According to RFC 2046, a boundary should be no longer than 70
	 * characters and must consist of characters from a limited set.
	 * 
	 * Rather than setting a boundary manually, consider using GenerateRandomBoundary()
	 * to create a compliant and unique boundary string.
	 */
	void SetBoundary(const std::string& boundary);

	/**
	 * @brief Gets the boundary string.
	 * @return The current boundary string.
	 *
	 * Returns the boundary string set for this body. If no boundary has been set,
	 * returns an empty string. This is primarily used for multipart bodies.
	 */
	const std::string& Boundary() const;

	/**
	 * @brief Generates a random boundary string.
	 * @return The generated boundary string.
	 *
	 * Creates a unique boundary string using a timestamp and random characters.
	 * This boundary is also set internally for the body. The generated boundary
	 * follows the recommendations in RFC 2046 for boundary strings.
	 * 
	 * This is the preferred method for setting a boundary rather than manually
	 * creating and setting one.
	 */
	std::string GenerateRandomBoundary();

public:
	/**
	 * @brief Gets a mutable reference to the parts vector.
	 * @return Reference to the vector of MIME entity parts.
	 *
	 * Provides direct access to modify the parts of a multipart body.
	 * This allows adding, removing, or modifying parts directly.
	 * 
	 * Note that after modifying parts directly, you may need to call 
	 * BuildMultipartContent() to update the raw content string.
	 */
	std::vector<std::shared_ptr<MimeEntity>>& Parts();

	/**
	 * @brief Gets a const reference to the parts vector.
	 * @return Const reference to the vector of MIME entity parts.
	 *
	 * Provides read-only access to the parts of a multipart body.
	 * This is useful for iterating through parts without modifying them.
	 */
	const std::vector<std::shared_ptr<MimeEntity>>& Parts() const;

	/**
	 * @brief Adds a part to a multipart body.
	 * @param part Shared pointer to the MimeEntity to add as a part.
	 *
	 * Appends a new part to the existing parts collection. This method
	 * only affects the internal parts collection; to update the raw content
	 * string, you may need to call BuildMultipartContent().
	 * 
	 * If this is the first part added and no multipart type has been set,
	 * consider calling SetMultipartType() to define how the parts should be interpreted.
	 */
	void AddPart(const std::shared_ptr<MimeEntity>& part);

	/**
	 * @brief Removes a part at the specified index.
	 * @param index Zero-based index of the part to remove.
	 * @return True if successful, false if the index is out of range.
	 *
	 * Removes the part at the given index from the parts collection.
	 * This method only affects the internal parts collection; to update
	 * the raw content string, you may need to call BuildMultipartContent().
	 */
	bool RemovePart(size_t index);

	/**
	 * @brief Gets a part at the specified index.
	 * @param index Zero-based index of the part to retrieve.
	 * @return Shared pointer to the MimeEntity, or nullptr if index is out of range.
	 *
	 * Retrieves a specific part from the parts collection. This method
	 * provides read-only access to the part.
	 */
	std::shared_ptr<MimeEntity> GetPart(size_t index) const;

	/**
	 * @brief Gets the number of parts in the multipart body.
	 * @return Number of parts currently in the body.
	 *
	 * Returns the size of the parts collection. For non-multipart bodies,
	 * this will typically return 0.
	 */
	size_t PartCount() const;

	/**
	 * @brief Sets the multipart type of the body.
	 * @param type The MultipartType to set.
	 *
	 * Defines how the parts in a multipart body should be interpreted by clients.
	 * This affects the Content-Type header when the MIME entity is built.
	 * For example, setting MultipartType::ALTERNATIVE indicates that each part
	 * represents an alternative version of the same content (e.g., text and HTML).
	 * 
	 * Setting a multipart type doesn't automatically convert the body to a
	 * multipart structure; you still need to add parts and build the content.
	 */
	void SetMultipartType(MultipartType type);

	/**
	 * @brief Gets the current multipart type.
	 * @return The current MultipartType of the body.
	 *
	 * Returns the current multipart type setting. The default is NONE for
	 * single-part bodies.
	 */
	MultipartType GetMultipartType() const;

	/**
	 * @brief Checks if the body is a multipart body.
	 * @return True if the body is multipart and has at least one part, false otherwise.
	 *
	 * This method provides a convenient way to determine if the body
	 * is structured as a multipart body with at least one part.
	 */
	bool IsMultipart() const;

public:
	/**
	 * @brief Finds a part by its Content-Type.
	 * @param contentType Content-Type string to search for.
	 * @return Shared pointer to the first matching MimeEntity, or nullptr if not found.
	 *
	 * Searches for a part with an exact Content-Type match. This is useful
	 * for finding specific types of content within a multipart body, such as
	 * "text/plain" or "image/jpeg".
	 * 
	 * This method only searches the top level of parts and doesn't recurse into
	 * nested multipart structures. For recursive searching, use FindPartsRecursive().
	 */
	std::shared_ptr<MimeEntity> GetPartByContentType(const std::string& contentType) const;

	/**
	 * @brief Finds a part by its Content-ID.
	 * @param contentId Content-ID string to search for.
	 * @return Shared pointer to the matching MimeEntity, or nullptr if not found.
	 *
	 * Searches for a part with a matching Content-ID header, used for finding
	 * inline attachments referenced by HTML content. Content-IDs are typically
	 * formatted like "<id@domain>" and referenced in HTML using the "cid:" URI scheme.
	 * 
	 * This method only searches the top level of parts and doesn't recurse into
	 * nested multipart structures.
	 */
	std::shared_ptr<MimeEntity> GetPartByContentId(const std::string& contentId) const;

	/**
	 * @brief Finds all parts with a specific Content-Type.
	 * @param contentType Content-Type string to search for.
	 * @return Vector of shared pointers to matching MimeEntity objects.
	 *
	 * Returns all parts that have the exact specified Content-Type. This is useful
	 * when a multipart body may contain multiple parts of the same type.
	 * 
	 * This method only searches the top level of parts and doesn't recurse into
	 * nested multipart structures. For recursive searching, use FindPartsRecursive().
	 */
	std::vector<std::shared_ptr<MimeEntity>> GetPartsByContentType(const std::string& contentType) const;

	/**
	 * @brief Finds parts that match a predicate function.
	 * @param predicate Function that takes a MimeEntity reference and returns a boolean.
	 * @return Vector of shared pointers to matching MimeEntity objects.
	 *
	 * Allows for custom filtering of parts based on any criteria defined by the predicate.
	 * The predicate function should return true for parts that match the desired criteria.
	 * 
	 * This method only searches the top level of parts and doesn't recurse into
	 * nested multipart structures. For recursive searching, use FindPartsRecursive().
	 * 
	 * Example usage:
	 * @code
	 * auto imageParts = body.FindParts([](const MimeEntity& entity) {
	 *     return entity.GetHeader().GetContentType().Type().starts_with("image/");
	 * });
	 * @endcode
	 */
	std::vector<std::shared_ptr<MimeEntity>> FindParts(const std::function<bool(const MimeEntity&)>& predicate) const;

	/**
	 * @brief Recursively finds parts that match a predicate function.
	 * @param predicate Function that takes a MimeEntity reference and returns a boolean.
	 * @return Vector of shared pointers to matching MimeEntity objects.
	 *
	 * Searches through nested multipart structures for parts matching the predicate.
	 * This is useful for finding specific content in complex MIME structures where
	 * parts may be nested within other multipart containers.
	 * 
	 * Example usage:
	 * @code
	 * auto jpegParts = body.FindPartsRecursive([](const MimeEntity& entity) {
	 *     return entity.GetHeader().GetContentType().Type() == "image/jpeg";
	 * });
	 * @endcode
	 */
	std::vector<std::shared_ptr<MimeEntity>> FindPartsRecursive(
		const std::function<bool(const MimeEntity&)>& predicate) const;

public:
	/**
	 * @brief Gets the plain text part of the message.
	 * @return Shared pointer to the text/plain MimeEntity, or nullptr if not found.
	 *
	 * Searches recursively for a part with Content-Type text/plain. This is particularly
	 * useful for multipart/alternative bodies where both text and HTML versions are
	 * provided, and you want to extract just the plain text version.
	 * 
	 * For simple bodies with direct text content (not multipart), this method will
	 * return nullptr since there are no parts to search.
	 */
	std::shared_ptr<MimeEntity> GetTextPart() const;

	/**
	 * @brief Gets the HTML part of the message.
	 * @return Shared pointer to the text/html MimeEntity, or nullptr if not found.
	 *
	 * Searches recursively for a part with Content-Type text/html. This is particularly
	 * useful for multipart/alternative bodies where both text and HTML versions are
	 * provided, and you want to extract just the HTML version.
	 * 
	 * Modern email clients typically prefer to display the HTML part when available.
	 * For simple bodies with direct content (not multipart), this method will
	 * return nullptr since there are no parts to search.
	 */
	std::shared_ptr<MimeEntity> GetHtmlPart() const;

	/**
	 * @brief Gets all attachments in the message.
	 * @return Vector of shared pointers to attachment MimeEntity objects.
	 *
	 * Finds parts with Content-Disposition: attachment or non-text parts that
	 * don't have a Content-ID. This method searches recursively through all parts.
	 * 
	 * Attachments are typically files that are meant to be saved by the recipient
	 * rather than displayed inline with the message content.
	 */
	std::vector<std::shared_ptr<MimeEntity>> GetAttachments() const;

	/**
	 * @brief Gets all inline attachments in the message.
	 * @return Vector of shared pointers to inline attachment MimeEntity objects.
	 *
	 * Finds parts with Content-Disposition: inline or parts that have a Content-ID.
	 * This method searches recursively through all parts.
	 * 
	 * Inline attachments are typically images or other resources that are meant
	 * to be displayed as part of the message content, often referenced by the HTML
	 * part using the "cid:" URI scheme.
	 */
	std::vector<std::shared_ptr<MimeEntity>> GetInlineAttachments() const;

public:
	/**
	 * @brief Creates a multipart body from a collection of parts.
	 * @param parts Vector of shared pointers to MimeEntity objects to include.
	 * @param type Multipart type to use (default: MultipartType::MIXED).
	 *
	 * Constructs a complete multipart body with appropriate boundaries and structure.
	 * This method performs the following steps:
	 * 1. Sets the multipart type
	 * 2. Generates a random boundary if none is set
	 * 3. Adds all the provided parts to the body
	 * 4. Builds the complete multipart content with proper formatting
	 * 
	 * This is the preferred way to create a multipart body rather than manually
	 * setting parts and building the content.
	 */
	void CreateMultipartBody(const std::vector<std::shared_ptr<MimeEntity>>& parts,
							 MultipartType type = MultipartType::MIXED);

	/**
	 * @brief Gets the body content as binary data.
	 * @return Vector of bytes containing the binary content.
	 *
	 * Returns the binary representation of the body content, useful for binary
	 * content types like images or attachments. If the content has been set
	 * as a string, this method will convert it to binary data.
	 * 
	 * For text-based content, this will return the UTF-8 encoded bytes.
	 */
	std::vector<uint8_t> GetBinaryContent() const;

	/**
	 * @brief Sets the body content from binary data.
	 * @param data Vector of bytes containing the binary data to set.
	 *
	 * Sets both the binary content storage and updates the string content.
	 * This method ensures that both the binary representation and the string
	 * representation of the content are kept in sync.
	 * 
	 * This is particularly useful for binary content types like images or 
	 * application data that are not primarily text-based.
	 */
	void SetBinaryContent(const std::vector<uint8_t>& data);

public:
	/**
	 * @brief Stream insertion operator for Body objects.
	 * @param os Output stream to write to.
	 * @param body Body object to output.
	 * @return Reference to the output stream.
	 *
	 * Formats the body content appropriately for output, handling both simple
	 * and multipart bodies. This operator is useful for debugging, logging,
	 * or sending MIME content to output streams.
	 * 
	 * For multipart bodies, this includes the complete structure with boundaries,
	 * preamble, epilogue, and all parts.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Body& body);

private:
	/**
	 * @brief Helper method for recursive part collection.
	 * @param entity MimeEntity to search within.
	 * @param predicate Function that determines if a part matches criteria.
	 * @param result Vector to store matching parts.
	 *
	 * Used internally by FindPartsRecursive to traverse nested multipart structures.
	 * This method recursively searches through all parts and nested multipart
	 * bodies, applying the predicate to each part and collecting matches.
	 */
	void CollectPartsRecursive(const std::shared_ptr<MimeEntity>& entity,
							   const std::function<bool(const MimeEntity&)>& predicate,
							   std::vector<std::shared_ptr<MimeEntity>>& result) const;

	/**
	 * @brief Builds the complete multipart content string.
	 * @return Formatted multipart content with boundaries and parts.
	 *
	 * Constructs the raw content string for a multipart body including all boundaries
	 * and part content. This method assembles the preamble, each part with appropriate
	 * boundaries, and the epilogue into a properly formatted MIME multipart body.
	 * 
	 * The formatting follows the rules specified in RFC 2046 for multipart bodies.
	 */
	std::string BuildMultipartContent() const;

private:
	/**
	 * @brief Implementation class for Body.
	 *
	 * Uses the PIMPL idiom to hide implementation details and maintain ABI compatibility.
	 * This allows the implementation to change without requiring recompilation of
	 * code that uses the Body class, as long as the public interface remains the same.
	 */
	class Impl;

	/**
	 * @brief Pointer to the implementation.
	 *
	 * Unique pointer to the implementation class instance. This follows the
	 * PIMPL (Pointer to Implementation) idiom to hide implementation details.
	 */
	std::unique_ptr<Impl> m_p_impl;
};

/**
 * @brief Stream insertion operator for Body objects.
 * @param os Output stream to write to.
 * @param body Body object to output.
 * @return Reference to the output stream.
 *
 * Global operator that enables writing Body objects to output streams.
 * This provides the same functionality as the friend operator within the
 * Body class, but is available for use in any context.
 */
std::ostream& operator<<(std::ostream& os, const Body& body);

/**
 * @namespace BodyHelpers
 * @brief Contains utility functions for working with MIME body types.
 *
 * This namespace provides helper functions for converting between MultipartType enums
 * and their string representations, as well as for creating and parsing Content-Type
 * headers for multipart bodies.
 * 
 * These utilities simplify common operations when working with MIME bodies,
 * particularly multipart structures.
 */
namespace BodyHelpers
{
/**
 * @brief Converts a MultipartType to its string representation.
 * @param type MultipartType enum value to convert.
 * @return String representation of the multipart type.
 *
 * Returns the standard subtype string for the given multipart type.
 * For example, MultipartType::MIXED becomes "mixed" and
 * MultipartType::ALTERNATIVE becomes "alternative".
 * 
 * These strings are used in the Content-Type header of MIME messages.
 */
std::string MultipartTypeToString(MultipartType type);

/**
 * @brief Converts a string to its corresponding MultipartType.
 * @param typeStr String representation of the multipart type.
 * @return Corresponding MultipartType enum value.
 *
 * Parses a multipart subtype string and returns the appropriate enum value.
 * This function performs case-insensitive matching for standard multipart subtypes.
 * If the string doesn't match any known subtype, it returns MultipartType::NONE.
 * 
 * This is useful when parsing Content-Type headers from MIME messages.
 */
MultipartType StringToMultipartType(const std::string& typeStr);

/**
 * @brief Creates a ContentType object for a multipart type.
 * @param type MultipartType to create a ContentType for.
 * @param boundary Boundary string to include in the ContentType.
 * @return ContentType object with appropriate type and boundary parameter.
 *
 * Constructs a ContentType header value with the correct multipart subtype
 * and boundary parameter. The result will be a string like
 * "multipart/alternative; boundary=xyz123".
 * 
 * This is useful when creating Content-Type headers for multipart bodies.
 */
ContentType CreateContentType(MultipartType type, const std::string& boundary);

/**
 * @brief Extracts the MultipartType from a ContentType.
 * @param contentType ContentType object to extract from.
 * @return Extracted MultipartType, or MultipartType::NONE if not a multipart type.
 *
 * Parses a ContentType object and returns the corresponding MultipartType if
 * the ContentType represents a multipart body. If the ContentType is not a
 * multipart type, MultipartType::NONE is returned.
 * 
 * This is useful when analyzing MIME messages to determine their structure.
 */
MultipartType ExtractMultipartType(const ContentType& contentType);
} // namespace BodyHelpers
} // namespace ISXMime
