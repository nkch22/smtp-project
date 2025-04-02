/**
 * @file MimeUtils.hpp
 * @brief Defines utility functions for MIME processing.
 *
 * This file declares the MimeUtils class which provides a collection of static utility
 * functions for working with MIME entities, content types, encodings, and other
 * MIME-related operations as defined in RFCs 2045-2049 and related standards.
 * 
 * The utilities include functions for MIME type mapping, header encoding/decoding,
 * boundary generation, filename encoding according to RFC 2231, and other helper
 * functions that support the MIME architecture.
 */

#pragma once

#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "ContentType.hpp"
#include "MimeEntity.hpp"

namespace ISXMime
{

/**
 * @class MimeUtils
 * @brief Provides utility functions for MIME processing.
 *
 * The MimeUtils class contains static utility methods for various MIME-related 
 * operations. These operations include mapping between file extensions and MIME types,
 * encoding and decoding MIME header values, generating unique boundary strings for
 * multipart messages, normalizing header field names, and other common tasks.
 * 
 * All methods in this class are static and can be called without instantiating
 * the class. The functionality provided supports the core MIME operations in
 * the library according to the MIME specifications in RFCs 2045-2049.
 */
class MimeUtils
{
public:
	/**
	 * @brief Gets a file extension for a given MIME type.
	 * @param contentType The ContentType object representing the MIME type.
	 * @return A string containing the file extension (without dot) for the MIME type.
	 *
	 * Maps a MIME type to a corresponding file extension. If no mapping exists for the
	 * specific MIME type, "bin" is returned as a default extension for binary data.
	 * This is useful when saving MIME content to a file and needing to choose an
	 * appropriate extension.
	 */
	static std::string GetFileExtensionForMimeType(const ContentType& contentType);

	/**
	 * @brief Gets a MIME type for a given file extension.
	 * @param extension The file extension (with or without dot).
	 * @return A ContentType object representing the MIME type for the extension.
	 *
	 * Maps a file extension to a corresponding MIME type. If no mapping exists for the
	 * specific extension, "application/octet-stream" is returned as a default MIME type
	 * for binary data. This is useful when creating MIME content from files and needing
	 * to specify an appropriate Content-Type.
	 */
	static ContentType GetMimeTypeForFileExtension(const std::string& extension);

	/**
	 * @brief Gets a map of common MIME types and their descriptions.
	 * @return A reference to a map containing MIME type strings and their human-readable descriptions.
	 *
	 * Provides a mapping between MIME type strings (e.g., "text/html") and
	 * human-readable descriptions (e.g., "HTML document"). This can be useful
	 * for displaying MIME types in a user interface or for debugging.
	 */
	static const std::map<std::string, std::string>& GetCommonMimeTypes();

	/**
	 * @brief Converts text to quoted-printable encoding for use in headers.
	 * @param text The text to encode.
	 * @return The quoted-printable encoded text.
	 *
	 * Encodes text using the quoted-printable encoding scheme as defined in RFC 2047
	 * for use in MIME headers. This ensures that non-ASCII characters and other special
	 * characters are properly represented in header values.
	 */
	static std::string ConvertToQuotedPrintableHeader(const std::string& text);

	/**
	 * @brief Gets a human-readable description of a Content-Type.
	 * @param contentType The ContentType object to describe.
	 * @return A string containing a human-readable description of the Content-Type.
	 *
	 * Provides a human-readable description of a Content-Type, which can be useful
	 * for displaying Content-Type information in a user interface or for debugging.
	 */
	static std::string GetContentTypeDescription(const ContentType& contentType);

	/**
	 * @brief Normalizes a header field name.
	 * @param name The header field name to normalize.
	 * @return The normalized header field name.
	 *
	 * Normalizes a header field name by ensuring proper capitalization according to
	 * common conventions (e.g., "content-type" becomes "Content-Type"). While header
	 * field names are case-insensitive according to the RFCs, using a consistent
	 * capitalization improves readability and interoperability.
	 */
	static std::string NormalizeHeaderFieldName(const std::string& name);

	/**
	 * @brief Performs case-insensitive string comparison.
	 * @param a First string to compare.
	 * @param b Second string to compare.
	 * @return True if the strings are equal (ignoring case), false otherwise.
	 *
	 * Compares two strings for equality, ignoring case. This is useful for
	 * case-insensitive operations like header field name comparison, as header
	 * field names are case-insensitive according to the RFCs.
	 */
	static bool CaseInsensitiveCompare(const std::string& a, const std::string& b);

	/**
	 * @brief Generates a unique MIME boundary string.
	 * @return A string containing a unique boundary for multipart MIME messages.
	 *
	 * Creates a unique boundary string suitable for use in multipart MIME messages.
	 * The boundary is guaranteed to be unique for each call, using a combination of
	 * timestamp and random characters, and follows the format recommendations in RFC 2046.
	 */
	static std::string GenerateMimeBoundary();
	
	/**
	 * @brief Encodes a filename for use in MIME headers.
	 * @param filename The filename to encode.
	 * @param charset Character set of the filename (default: UTF-8).
	 * @return The encoded filename string.
	 *
	 * Encodes a filename according to RFC 2047 for use in MIME headers, ensuring that
	 * non-ASCII characters are properly represented. This is particularly important
	 * for filenames containing international characters.
	 */
	static std::string EncodeFilename(const std::string& filename, const std::string& charset = "UTF-8");
	
	/**
	 * @brief Decodes an encoded filename from a MIME header.
	 * @param encoded_filename The encoded filename string.
	 * @return The decoded filename.
	 *
	 * Decodes a filename that was encoded according to RFC 2047 or RFC 2231.
	 * This reverses the encoding applied by EncodeFilename() and other standard
	 * MIME encoding methods for filenames.
	 */
	static std::string DecodeFilename(const std::string& encoded_filename);

	/**
	 * @brief Encodes a parameter value for use in MIME headers.
	 * @param name The parameter name.
	 * @param value The parameter value to encode.
	 * @param charset Character set of the value (default: UTF-8).
	 * @return The encoded parameter string in the format "name=value" or "name*=charset''encoded-value".
	 *
	 * Encodes a parameter value according to RFC 2231 for use in MIME headers, ensuring that
	 * non-ASCII characters are properly represented. This is used for parameters in
	 * Content-Type, Content-Disposition, and other headers that may contain international
	 * characters or characters that need encoding.
	 */
	static std::string EncodeParameterValue(const std::string& name, const std::string& value,
											const std::string& charset = "UTF-8");

	/**
	 * @brief Decodes Base64 encoded text.
	 * @param encoded_text The Base64 encoded text.
	 * @return The decoded text.
	 * @throw MimeException If the Base64 decoder is not available.
	 *
	 * Decodes text that was encoded using the Base64 encoding scheme.
	 * This is a convenience method that uses the encoding framework to perform
	 * the actual decoding operation.
	 */
	static std::string DecodeBase64(const std::string& encoded_text);
	
	/**
	 * @brief Decodes Quoted-Printable encoded text.
	 * @param encoded_text The Quoted-Printable encoded text.
	 * @return The decoded text.
	 * @throw MimeException If the Quoted-Printable decoder is not available.
	 *
	 * Decodes text that was encoded using the Quoted-Printable encoding scheme.
	 * This is a convenience method that uses the encoding framework to perform
	 * the actual decoding operation.
	 */
	static std::string DecodeQuotedPrintable(const std::string& encoded_text);

private:
	/**
	 * @brief Generates a random string of the specified length.
	 * @param length The length of the random string to generate.
	 * @return A string containing random alphanumeric characters.
	 *
	 * Creates a random string of the specified length using alphanumeric characters.
	 * This is used internally by GenerateMimeBoundary() and other methods that need
	 * to generate random strings.
	 */
	static std::string GenerateRandomString(size_t length);
};

} // namespace ISXMime
