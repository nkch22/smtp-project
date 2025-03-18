/**
 * @file ContentType.hpp
 * @brief Defines the ContentType class for handling MIME Content-Type headers.
 *
 * This file declares the ContentType class which provides functionality for working
 * with MIME Content-Type headers as defined in RFC 2045. The Content-Type header
 * field is used to specify the nature of the data in the body of a MIME entity,
 * by giving media type and subtype identifiers, and by providing auxiliary information.
 * 
 * The ContentType class supports various media types including text, image, application,
 * and multipart types, along with their parameters such as charset and boundary.
 */

#pragma once

#include <map>
#include <stdexcept>
#include <string>

namespace ISXMime
{

/**
 * @class MimeException
 * @brief Exception class for MIME-related errors.
 *
 * This exception class is used to report runtime errors related to MIME
 * processing, such as malformed headers or content. It derives from
 * std::runtime_error and provides the same functionality, but with a
 * distinct type for catching specific MIME-related exceptions.
 */
class MimeException : public std::runtime_error
{
public:
	/**
	 * @brief Constructor with error message.
	 * @param message The error message describing the exception.
	 * 
	 * Creates a MimeException with the specified error message, which
	 * can be retrieved using the what() method inherited from std::runtime_error.
	 */
	explicit MimeException(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @class ContentType
 * @brief Represents a MIME Content-Type header field.
 *
 * The ContentType class encapsulates a MIME Content-Type header, which consists of
 * a type, subtype, and optional parameters. This class provides methods for 
 * creating, modifying, and formatting Content-Type headers according to RFC 2045.
 * 
 * Content-Types are used to identify the format of content in MIME messages,
 * such as "text/plain", "image/jpeg", or "multipart/mixed". Parameters provide
 * additional information about the content, such as character set or boundary strings.
 * 
 * This class also provides factory methods for common Content-Type values.
 */
class ContentType
{
public:
	/**
     * @brief Default constructor.
     *
     * Creates a ContentType object with default type "text" and subtype "plain".
     * This corresponds to the default Content-Type assumed by MIME processors
     * when no Content-Type header is provided, according to RFC 2045.
     */
	ContentType();
	
	/**
     * @brief Constructor with type and subtype.
     * @param type The media type (e.g., "text", "image", "application").
     * @param subtype The media subtype (e.g., "plain", "html", "jpeg").
     *
     * Creates a ContentType object with the specified type and subtype, but no parameters.
     * The resulting Content-Type would be formatted as "type/subtype" without any
     * additional parameters.
     */
	ContentType(const std::string& type, const std::string& subtype);

    /**
     * @brief Constructor from a complete Content-Type string.
     * @param fullContentType A string in the format "type/subtype; param1=value1; param2=value2".
     *
     * Parses a complete Content-Type string, extracting the type, subtype, and parameters.
     * This is useful when processing existing Content-Type headers from MIME messages.
     * If the string is improperly formatted, defaults to "text/plain".
     */
	explicit ContentType(const std::string& fullContentType);

    /**
     * @brief Destructor.
     *
     * Cleans up resources used by the ContentType object.
     */
	~ContentType();

public:
    /**
     * @brief Sets the media type.
     * @param type The media type to set (e.g., "text", "image", "application").
     *
     * Updates the media type component of the Content-Type. According to RFC 2045,
     * the type should consist of US-ASCII letters, digits, and hyphens.
     */
	void SetType(const std::string& type);
	
	/**
     * @brief Gets the media type.
     * @return The current media type.
     *
     * Returns the current media type component of the Content-Type.
     */
	const std::string& Type() const;

    /**
     * @brief Sets the media subtype.
     * @param subtype The media subtype to set (e.g., "plain", "html", "jpeg").
     *
     * Updates the media subtype component of the Content-Type. According to RFC 2045,
     * the subtype should consist of US-ASCII letters, digits, and hyphens.
     */
	void SetSubtype(const std::string& subtype);
	
	/**
     * @brief Gets the media subtype.
     * @return The current media subtype.
     *
     * Returns the current media subtype component of the Content-Type.
     */
	const std::string& Subtype() const;

public:
    /**
     * @brief Adds a parameter to the Content-Type.
     * @param name The parameter name (e.g., "charset", "boundary").
     * @param value The parameter value.
     *
     * Adds or updates a parameter in the Content-Type header. Parameters provide
     * additional information about the content, such as character set for text media
     * types or boundary strings for multipart types.
     */
	void AddParameter(const std::string& name, const std::string& value);
	
	/**
     * @brief Gets the value of a parameter.
     * @param name The parameter name to look up.
     * @return The parameter value, or an empty string if the parameter doesn't exist.
     *
     * Retrieves the value of a specific parameter from the Content-Type header.
     */
	std::string GetParameter(const std::string& name) const;
	
	/**
     * @brief Checks if a parameter exists.
     * @param name The parameter name to check.
     * @return True if the parameter exists, false otherwise.
     *
     * Determines whether a specific parameter is present in the Content-Type header.
     */
	bool HasParameter(const std::string& name) const;

    /**
     * @brief Parses parameters from a string.
     * @param paramString A string containing parameters in the format "param1=value1; param2=value2".
     *
     * Parses a string of parameters and adds them to the ContentType object.
     * This is used internally by the constructor that takes a full Content-Type string,
     * but can also be called directly to add parameters from a separate string.
     */
	void ParseParameters(const std::string& paramString);

public:
    /**
     * @brief Creates a ContentType for any text media type.
     * @return A ContentType object with type "text" and subtype "*".
     *
     * This factory method creates a content type that represents any text media type.
     * The "*" subtype is a wildcard that matches any specific text subtype.
     */
	static ContentType Text();
	
	/**
     * @brief Creates a ContentType for plain text.
     * @return A ContentType object with type "text" and subtype "plain".
     *
     * This factory method creates the standard content type for plain text content,
     * which is the default content type in MIME when none is specified.
     */
	static ContentType TextPlain();
	
	/**
     * @brief Creates a ContentType for HTML text.
     * @return A ContentType object with type "text" and subtype "html".
     *
     * This factory method creates the standard content type for HTML content,
     * commonly used in email messages and web content.
     */
	static ContentType TextHtml();
	
	/**
     * @brief Creates a ContentType for JSON data.
     * @return A ContentType object with type "application" and subtype "json".
     *
     * This factory method creates the standard content type for JSON data,
     * commonly used in APIs and data exchange.
     */
	static ContentType ApplicationJson();
	
	/**
     * @brief Creates a ContentType for XML data.
     * @return A ContentType object with type "application" and subtype "xml".
     *
     * This factory method creates the standard content type for XML data,
     * commonly used in web services and data exchange.
     */
	static ContentType ApplicationXml();
	
	/**
     * @brief Creates a ContentType for multipart/mixed content.
     * @return A ContentType object with type "multipart", subtype "mixed", and a random boundary.
     *
     * This factory method creates a content type for multipart/mixed content, which
     * contains independent parts with different content types. A random boundary
     * parameter is automatically generated to separate the parts.
     * 
     * Multipart/mixed is commonly used for email messages with attachments.
     */
	static ContentType MultipartMixed();
	
	/**
     * @brief Creates a ContentType for multipart/alternative content.
     * @return A ContentType object with type "multipart", subtype "alternative", and a random boundary.
     *
     * This factory method creates a content type for multipart/alternative content, which
     * contains multiple representations of the same content (e.g., plain text and HTML).
     * A random boundary parameter is automatically generated to separate the parts.
     * 
     * Multipart/alternative is commonly used for email messages with both text and HTML versions.
     */
	static ContentType MultipartAlternative();
	
	/**
     * @brief Creates a ContentType for multipart/related content.
     * @return A ContentType object with type "multipart", subtype "related", and a random boundary.
     *
     * This factory method creates a content type for multipart/related content, which
     * contains related parts that reference each other (e.g., HTML with inline images).
     * A random boundary parameter is automatically generated to separate the parts.
     * 
     * Multipart/related is commonly used for HTML email messages with embedded images.
     */
	static ContentType MultipartRelated();
	
	/**
     * @brief Creates a ContentType for JPEG images.
     * @return A ContentType object with type "image" and subtype "jpeg".
     *
     * This factory method creates the standard content type for JPEG image content.
     */
	static ContentType ImageJpeg();
	
	/**
     * @brief Creates a ContentType for PNG images.
     * @return A ContentType object with type "image" and subtype "png".
     *
     * This factory method creates the standard content type for PNG image content.
     */
	static ContentType ImagePng();
	
	/**
     * @brief Creates a ContentType for GIF images.
     * @return A ContentType object with type "image" and subtype "gif".
     *
     * This factory method creates the standard content type for GIF image content.
     */
	static ContentType ImageGif();

public:
	std::string Str() const;
	bool IsMultipart() const;
	bool IsValid() const;

    /**
     * @brief Gets the complete Content-Type string.
     * @return A string containing the full Content-Type header value.
     *
     * Returns the complete Content-Type header value, including all parameters.
     * The format is "type/subtype; param1=value1; param2=value2".
     */
	std::string GetContentType() const;
	
	/**
     * @brief Gets the media type.
     * @return The media type (e.g., "text", "image", "application").
     *
     * Returns the primary media type of the content.
     */
	std::string GetType() const;
	
	/**
     * @brief Gets the media subtype.
     * @return The media subtype (e.g., "plain", "jpeg", "json").
     *
     * Returns the specific subtype of the content.
     */
	std::string GetSubtype() const;
	
	/**
     * @brief Gets the boundary parameter.
     * @return The boundary string, or an empty string if not set.
     *
     * Returns the boundary parameter value, which is used to separate parts
     * in multipart content types.
     */
	std::string GetBoundary() const;
	
	/**
     * @brief Gets the charset parameter.
     * @return The character set, or an empty string if not set.
     *
     * Returns the character set parameter value, which specifies the encoding
     * used for text content.
     */
	std::string GetCharset() const;
	
	/**
     * @brief Gets the name parameter.
     * @return The name value, or an empty string if not set.
     *
     * Returns the name parameter value, which can be used to provide a
     * suggested filename for the content.
     */
	std::string GetName() const;
	
	/**
     * @brief Gets the filename parameter.
     * @return The filename value, or an empty string if not set.
     *
     * Returns the filename parameter value, which provides a suggested
     * filename for the content when it is saved.
     */
	std::string GetFilename() const;
	
	/**
     * @brief Gets the format parameter.
     * @return The format value, or an empty string if not set.
     *
     * Returns the format parameter value, which can specify additional
     * information about the content format.
     */
	std::string GetFormat() const;
	
	/**
     * @brief Gets the protocol parameter.
     * @return The protocol value, or an empty string if not set.
     *
     * Returns the protocol parameter value, which can specify the protocol
     * used to transfer the content.
     */
	std::string GetProtocol() const;
	
	/**
     * @brief Gets the method parameter.
     * @return The method value, or an empty string if not set.
     *
     * Returns the method parameter value, which can specify the method
     * used to process the content.
     */
	std::string GetMethod() const;
	
	/**
     * @brief Gets the access-type parameter.
     * @return The access-type value, or an empty string if not set.
     *
     * Returns the access-type parameter value, which can specify how
     * the content should be accessed.
     */
	std::string GetAccessType() const;
	
	/**
     * @brief Gets the disposition parameter.
     * @return The disposition value, or an empty string if not set.
     *
     * Returns the disposition parameter value, which can specify how
     * the content should be displayed or handled.
     */
	std::string GetDisposition() const;
	
	/**
     * @brief Gets the duration parameter.
     * @return The duration value, or an empty string if not set.
     *
     * Returns the duration parameter value, which can specify the length
     * of time-based content.
     */
	std::string GetDuration() const;
	
	/**
     * @brief Gets the size parameter.
     * @return The size value, or an empty string if not set.
     *
     * Returns the size parameter value, which can specify the size
     * of the content in bytes.
     */
	std::string GetSize() const;
	
	/**
     * @brief Gets the language parameter.
     * @return The language value, or an empty string if not set.
     *
     * Returns the language parameter value, which can specify the
     * language of the content.
     */
	std::string GetLanguage() const;
	
	/**
     * @brief Gets the MD5 parameter.
     * @return The MD5 value, or an empty string if not set.
     *
     * Returns the MD5 parameter value, which can provide a checksum
     * of the content.
     */
	std::string GetMD5() const;
	
	/**
     * @brief Gets the location parameter.
     * @return The location value, or an empty string if not set.
     *
     * Returns the location parameter value, which can specify a URL
     * where the content can be found.
     */
	std::string GetLocation() const;
	
	/**
     * @brief Gets the base parameter.
     * @return The base value, or an empty string if not set.
     *
     * Returns the base parameter value, which can specify a base URL
     * for resolving relative URLs in the content.
     */
	std::string GetBase() const;
	
	/**
     * @brief Gets the version parameter.
     * @return The version value, or an empty string if not set.
     *
     * Returns the version parameter value, which can specify the version
     * of the content format.
     */
	std::string GetVersion() const;

private:
	static std::string TrimString(const std::string& str);
	static std::string GenerateRandomBoundary();

private:
	std::string m_type;
	std::string m_subtype;
	std::map<std::string, std::string> m_params;
};

} // namespace ISXMime
