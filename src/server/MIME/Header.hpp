/**
 * @file Header.hpp
 * @brief Defines the Header class for managing MIME message headers.
 *
 * This file declares the Header class which provides functionality for working with
 * MIME message headers, including common headers like Content-Type and Content-Transfer-Encoding,
 * as well as general header field management. It supports case-insensitive header access,
 * encoded header values according to RFC 2047, and standard header field operations.
 * 
 * Headers are a fundamental component of MIME messages, containing metadata about the
 * message content, such as its type, encoding, and other properties.
 */

#pragma once

#include <map>
#include <ostream>
#include <sstream>
#include <string>

#include "ContentTransferEncoding.hpp"
#include "ContentType.hpp"

namespace ISXMime
{
/**
 * @class Header
 * @brief Represents the header section of a MIME entity.
 *
 * The Header class manages a collection of header fields in a MIME message, providing
 * methods for adding, retrieving, and manipulating header values. It offers special
 * handling for important headers like Content-Type and Content-Transfer-Encoding,
 * and supports both case-sensitive and case-insensitive header lookup.
 * 
 * According to RFC 5322 and RFC 2045, headers consist of field names and values
 * in the format "Field-Name: field value". This class helps maintain the proper
 * format and provides encoding/decoding capabilities for internationalized headers.
 */
class Header
{
public:
	/**
	 * @brief Performs case-insensitive string comparison.
	 * @param a First string to compare.
	 * @param b Second string to compare.
	 * @return True if the strings are equal (ignoring case), false otherwise.
	 * 
	 * This static utility method compares two strings, ignoring the case of characters.
	 * It's useful for implementing case-insensitive header field name comparisons as
	 * required by MIME standards.
	 */
	static bool CaseInsensitiveCompare(const std::string& a, const std::string& b);

public:
	/**
	 * @brief Default constructor.
	 *
	 * Creates an empty Header object with default Content-Type (text/plain) and
	 * Content-Transfer-Encoding (7bit). These defaults are chosen according to
	 * the MIME specifications, which state that these are the assumed values
	 * when the respective headers are not explicitly provided.
	 */
	Header();
	
	/**
	 * @brief Destructor.
	 *
	 * Properly cleans up resources used by the Header object.
	 */
	~Header();

public:
	/**
	 * @brief Gets the Content-Type header as a mutable object.
	 * @return Reference to the ContentType object.
	 *
	 * Provides access to the specialized ContentType object for direct manipulation.
	 * Changes to this object will be reflected in the header fields when appropriate
	 * methods are called. This approach allows for easier manipulation of the
	 * structured Content-Type header.
	 */
	ContentType& GetContentType();
	
	/**
	 * @brief Gets the Content-Type header as a const object.
	 * @return Const reference to the ContentType object.
	 *
	 * Provides read-only access to the ContentType object.
	 */
	const ContentType& GetContentType() const;
	
	/**
	 * @brief Sets the Content-Type header.
	 * @param ct ContentType object to set.
	 *
	 * Updates both the internal ContentType object and the corresponding
	 * header field value. This ensures that the ContentType specialized object
	 * and the raw header field stay synchronized.
	 */
	void SetContentType(const ContentType& ct);

public:
	/**
	 * @brief Gets the Content-Transfer-Encoding header as a mutable object.
	 * @return Reference to the ContentTransferEncoding object.
	 *
	 * Provides access to the specialized ContentTransferEncoding object for direct
	 * manipulation. Changes to this object will be reflected in the header fields
	 * when appropriate methods are called.
	 */
	ContentTransferEncoding& GetContentTransferEncoding();
	
	/**
	 * @brief Gets the Content-Transfer-Encoding header as a const object.
	 * @return Const reference to the ContentTransferEncoding object.
	 *
	 * Provides read-only access to the ContentTransferEncoding object.
	 */
	const ContentTransferEncoding& GetContentTransferEncoding() const;
	
	/**
	 * @brief Sets the Content-Transfer-Encoding header.
	 * @param cte ContentTransferEncoding object to set.
	 *
	 * Updates both the internal ContentTransferEncoding object and the corresponding
	 * header field value. This ensures that the ContentTransferEncoding specialized
	 * object and the raw header field stay synchronized.
	 */
	void SetContentTransferEncoding(const ContentTransferEncoding& cte);

public:
	/**
	 * @brief Checks if a header field exists.
	 * @param name Case-sensitive name of the header field to check.
	 * @return True if the field exists, false otherwise.
	 *
	 * This method performs a case-sensitive lookup for the specified header field.
	 * For case-insensitive lookup, use HasFieldCaseInsensitive() instead.
	 */
	bool HasField(const std::string& name) const;
	
	/**
	 * @brief Adds or updates a header field.
	 * @param name Name of the header field.
	 * @param value Value of the header field.
	 *
	 * If the header field already exists, its value is updated. Otherwise, a new
	 * field is added. The header name is normalized using MimeUtils::NormalizeHeaderFieldName()
	 * to ensure consistent casing and formatting.
	 * 
	 * If the field is Content-Type or Content-Transfer-Encoding, the corresponding
	 * specialized objects are also updated to maintain synchronization.
	 */
	void AddField(const std::string& name, const std::string& value);
	
	/**
	 * @brief Gets the value of a header field.
	 * @param name Case-sensitive name of the header field to retrieve.
	 * @return Value of the header field, or an empty string if the field doesn't exist.
	 *
	 * This method performs a case-sensitive lookup for the specified header field.
	 * For case-insensitive lookup, use GetFieldCaseInsensitive() instead.
	 */
	std::string GetField(const std::string& name) const;

	/**
	 * @brief Checks if a header field exists, ignoring case.
	 * @param name Case-insensitive name of the header field to check.
	 * @return True if the field exists, false otherwise.
	 *
	 * This method performs a case-insensitive lookup for the specified header field,
	 * which is more compliant with the RFC standards that specify header field names
	 * are case-insensitive.
	 */
	bool HasFieldCaseInsensitive(const std::string& name) const;
	
	/**
	 * @brief Gets the value of a header field, ignoring case.
	 * @param name Case-insensitive name of the header field to retrieve.
	 * @return Value of the header field, or an empty string if the field doesn't exist.
	 *
	 * This method performs a case-insensitive lookup for the specified header field,
	 * which is more compliant with the RFC standards that specify header field names
	 * are case-insensitive.
	 */
	std::string GetFieldCaseInsensitive(const std::string& name) const;
	
	/**
	 * @brief Gets all header fields.
	 * @return Const reference to the map of header fields.
	 *
	 * Provides read-only access to the complete collection of header fields.
	 * This is useful for operations that need to process all headers, such as
	 * writing the complete header section to a stream.
	 */
	const std::map<std::string, std::string>& GetAllFields() const;

public:
	/**
	 * @brief Encodes a header field value according to RFC 2047.
	 * @param value The value to encode.
	 * @param charset Character set of the value (default: UTF-8).
	 * @return The encoded header field value.
	 *
	 * This method encodes non-ASCII characters in header field values using
	 * either Q-encoding or Base64 encoding, as specified in RFC 2047. The encoding
	 * method is chosen based on the content: Base64 for highly non-ASCII content,
	 * Q-encoding for mostly ASCII content with some non-ASCII characters.
	 * 
	 * The resulting encoded-word syntax is: =?charset?encoding?encoded-text?=
	 * This is used for header fields that might contain non-ASCII characters,
	 * such as Subject, From, To, etc.
	 */
	std::string EncodeFieldValue(const std::string& value, const std::string& charset = "UTF-8") const;
	
	/**
	 * @brief Decodes an RFC 2047 encoded header field value.
	 * @param encoded_value The encoded header field value.
	 * @return The decoded header field value.
	 *
	 * This method decodes header field values that were encoded using the
	 * encoded-word syntax of RFC 2047. It supports both Q-encoding and Base64
	 * encoding methods, and handles multiple encoded-word sections in a single
	 * header field value.
	 */
	std::string DecodeFieldValue(const std::string& encoded_value) const;

public:
	/**
	 * @brief Normalizes a header field name.
	 * @param lower_name Field name to normalize (modified in place).
	 *
	 * This method normalizes header field names by ensuring consistent casing
	 * and formatting. This is for internal use; public code should use the
	 * MimeUtils::NormalizeHeaderFieldName() function instead.
	 * 
	 * @deprecated This method is deprecated and should be removed.
	 * Use MimeUtils::NormalizeHeaderFieldName() instead.
	 */
	void NormalizeFieldName(std::string& lower_name);

public:
	/**
	 * @brief Stream insertion operator for Header objects.
	 * @param os Output stream to write to.
	 * @param header Header object to output.
	 * @return Reference to the output stream.
	 *
	 * Formats the header fields appropriately for output according to the
	 * RFC 5322 format, with each field on a separate line followed by CRLF.
	 * The end of the header section is indicated by an empty line (CRLF).
	 * 
	 * This operator is useful for serializing a Header object for transmission
	 * or storage.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Header& header);

private:
	std::map<std::string, std::string> m_fields;       ///< Container for all header fields
	class ContentType m_content_type;                  ///< Specialized handler for Content-Type header
	class ContentTransferEncoding m_content_transfer;  ///< Specialized handler for Content-Transfer-Encoding header
};
} // namespace ISXMime
