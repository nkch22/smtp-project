/**
 * @file ContentTransferEncoding.hpp
 * @brief Defines the ContentTransferEncoding class for handling MIME Content-Transfer-Encoding headers.
 *
 * This file declares the ContentTransferEncoding class which provides functionality for working
 * with MIME Content-Transfer-Encoding headers as defined in RFC 2045. The Content-Transfer-Encoding
 * header field specifies the encoding mechanism used to represent the body of a MIME entity.
 * 
 * The ContentTransferEncoding class supports the standard encoding types defined in RFC 2045,
 * including 7bit, 8bit, binary, base64, and quoted-printable, and provides methods for
 * encoding and decoding content according to these standards.
 */

#pragma once

#include <string>
#include <vector>

#include "../Encoding/IEncoder.hpp"

namespace ISXMime
{

/**
 * @class ContentTransferEncoding
 * @brief Represents a MIME Content-Transfer-Encoding header field.
 *
 * The ContentTransferEncoding class encapsulates a MIME Content-Transfer-Encoding header,
 * which specifies the encoding used to transform the body content for transport through
 * email systems. This class provides methods for encoding and decoding content using
 * the specified encoding type.
 * 
 * As defined in RFC 2045, there are five standard encoding types: 7bit, 8bit, binary,
 * base64, and quoted-printable. Each serves different purposes:
 * - 7bit: US-ASCII data with lines less than 1000 characters (default for text parts)
 * - 8bit: Like 7bit, but allows 8-bit characters (not all mail systems support this)
 * - binary: Unrestricted binary data (rarely supported in email)
 * - base64: Efficient encoding for binary data, resulting in ~33% size increase
 * - quoted-printable: Readable encoding for mostly ASCII text with some special characters
 */
class ContentTransferEncoding
{
public:
	/**
     * @enum Type
     * @brief Defines the types of content transfer encodings.
     *
     * This enum represents the different encoding methods defined in RFC 2045,
     * each with specific characteristics and use cases.
     */
	enum Type
	{
		SEVEN_BIT,       ///< 7bit encoding (ASCII characters 32-126, CR, LF, TAB)
		EIGHT_BIT,       ///< 8bit encoding (allows 8-bit characters, but not null bytes)
		BINARY,          ///< Binary encoding (unrestricted binary data)
		BASE64,          ///< Base64 encoding (efficient for binary data)
		QUOTED_PRINTABLE, ///< Quoted-printable encoding (for mostly-ASCII text)
		UNKNOWN          ///< Unknown or unsupported encoding
	};

public:
	/**
     * @brief Default constructor.
     *
     * Creates a ContentTransferEncoding object with the default type "7bit".
     * This corresponds to the default Content-Transfer-Encoding assumed by MIME
     * processors when no Content-Transfer-Encoding header is provided, according to RFC 2045.
     */
	ContentTransferEncoding() : m_type(SEVEN_BIT) {}
	
	/**
     * @brief Constructor with type.
     * @param type The encoding type to use.
     *
     * Creates a ContentTransferEncoding object with the specified type.
     */
	explicit ContentTransferEncoding(Type type) : m_type(type) {}
	
	/**
     * @brief Constructor from a string representation.
     * @param type A string representation of the encoding type (e.g., "base64", "quoted-printable").
     *
     * Parses a string representation of an encoding type and creates a
     * ContentTransferEncoding object with the corresponding type.
     * If the string doesn't match any known encoding type, UNKNOWN is used.
     */
	explicit ContentTransferEncoding(const std::string& type);

public:
	/**
     * @brief Encodes data using the current encoding type.
     * @param data The binary data to encode.
     * @return The encoded data as a string.
     * @throw std::runtime_error If the data cannot be encoded with the current type.
     *
     * Applies the current encoding type to the provided binary data. For 7bit and 8bit
     * encodings, this function validates that the data meets the requirements of the
     * encoding. For base64 and quoted-printable, it applies the actual encoding algorithm.
     * For binary, it simply converts the data to a string without modification.
     */
	std::string Encode(const std::vector<uint8_t>& data) const;
	
	/**
     * @brief Decodes data that was encoded with the current encoding type.
     * @param encoded The encoded data as a string.
     * @return The decoded binary data.
     * @throw std::runtime_error If the data cannot be decoded with the current type.
     *
     * Reverses the encoding applied by the current encoding type. For 7bit, 8bit,
     * and binary encodings, this simply converts the string to binary data without
     * modification. For base64 and quoted-printable, it applies the appropriate
     * decoding algorithm.
     */
	std::vector<uint8_t> Decode(const std::string& encoded) const;

public:
	/**
     * @brief Gets the current encoding type.
     * @return The current ContentTransferEncoding::Type.
     *
     * Returns the current encoding type stored in this object.
     */
	Type GetType() const { return m_type; }
	
	/**
     * @brief Sets the encoding type.
     * @param type The ContentTransferEncoding::Type to set.
     *
     * Updates the encoding type stored in this object.
     */
	void SetType(Type type) { m_type = type; }
	
	/**
     * @brief Sets the encoding type from a string representation.
     * @param type A string representation of the encoding type (e.g., "base64", "quoted-printable").
     *
     * Parses a string representation of an encoding type and updates
     * the type stored in this object. If the string doesn't match any
     * known encoding type, UNKNOWN is used.
     */
	void SetType(const std::string& type);

public:
	/**
     * @brief Converts the encoding type to its string representation.
     * @return A string representation of the current encoding type.
     *
     * Returns the standard string representation of the current encoding type
     * as defined in RFC 2045 (e.g., "base64", "quoted-printable").
     */
	std::string Str() const;

	/**
     * @brief Checks if the encoding type is binary-safe.
     * @return True if the encoding type can handle binary data, false otherwise.
     *
     * Determines whether the current encoding type can safely handle all possible
     * byte values (0-255). Returns true for all encoding types except 7bit.
     */
	bool IsBinarySafe() const;
	
	/**
     * @brief Checks if the encoding type requires actual encoding/decoding.
     * @return True if encoding/decoding is required, false otherwise.
     *
     * Determines whether the current encoding type requires actual transformation
     * of the data. Returns true for base64 and quoted-printable, false for 7bit,
     * 8bit, and binary (which don't transform the data, they just have restrictions
     * on what data can be represented).
     */
	bool RequiresEncoding() const;

private:
	/**
     * @brief Validates that data meets 7bit encoding requirements.
     * @param data The binary data to validate.
     * @return True if the data is valid for 7bit encoding, false otherwise.
     *
     * Checks that all bytes in the data are in the range 1-127 (ASCII characters
     * excluding NULL). 7bit encoding can only represent ASCII characters.
     */
	static bool IsValid7Bit(const std::vector<uint8_t>& data);
	
	/**
     * @brief Validates that data meets 8bit encoding requirements.
     * @param data The binary data to validate.
     * @return True if the data is valid for 8bit encoding, false otherwise.
     *
     * Checks that all bytes in the data are non-zero. 8bit encoding can represent
     * any character except the NULL character (0).
     */
	static bool IsValid8Bit(const std::vector<uint8_t>& data);

private:
	/**
     * @brief Converts an encoding type to its string representation.
     * @param type The ContentTransferEncoding::Type to convert.
     * @return A string representation of the encoding type.
     *
     * Returns the standard string representation of the given encoding type
     * as defined in RFC 2045 (e.g., "base64", "quoted-printable").
     */
	static const char* TypeToString(Type type);
	
	/**
     * @brief Converts a string representation to an encoding type.
     * @param str The string representation to convert.
     * @return The corresponding ContentTransferEncoding::Type.
     *
     * Parses a string representation of an encoding type and returns
     * the corresponding enum value. If the string doesn't match any
     * known encoding type, UNKNOWN is returned.
     */
	static Type StringToType(const std::string& str);

private:
	Type m_type;  ///< The current encoding type
};

} // namespace ISXMime
