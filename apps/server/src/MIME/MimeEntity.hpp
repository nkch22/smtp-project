/**
 * @file MimeEntity.hpp
 * @brief Defines the MimeEntity class, representing a complete MIME entity.
 *
 * This file declares the MimeEntity class which is the fundamental building block
 * of MIME messages. A MimeEntity consists of a header section and a body section,
 * following the structure defined in RFC 5322 and the MIME specifications (RFC 2045-2049).
 * 
 * MimeEntity objects can be used to represent entire messages or individual parts
 * within multipart structures. They provide a unified interface for working with
 * both simple and complex MIME structures.
 */

#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "Header.hpp"

namespace ISXMime
{
class Body;

/**
 * @class MimeEntity
 * @brief Represents a complete MIME entity with headers and body.
 *
 * The MimeEntity class represents a MIME entity as defined in RFC 2045,
 * consisting of a collection of headers and a body. It forms the basic
 * building block of MIME messages, supporting both simple single-part
 * messages and complex multipart structures.
 * 
 * This class uses the PIMPL idiom to hide implementation details and maintain
 * ABI compatibility. It provides accessor methods for the Header and Body components,
 * allowing manipulation of the entity's content and metadata.
 * 
 * A MimeEntity can be serialized to a standard-compliant MIME format using
 * the stream insertion operator, making it suitable for storage or transmission.
 */
class MimeEntity
{
public:
	/**
     * @brief Default constructor.
     *
     * Creates an empty MimeEntity with default Header and Body objects.
     * The default Header includes Content-Type: text/plain and
     * Content-Transfer-Encoding: 7bit, following the MIME specifications
     * for default values when these headers are not explicitly provided.
     */
	MimeEntity();
	
	/**
     * @brief Virtual destructor.
     *
     * Properly cleans up resources used by the MimeEntity object,
     * including the implementation details managed by the PIMPL idiom.
     * The destructor is virtual to allow for proper cleanup of derived classes.
     */
	virtual ~MimeEntity();
	
	/**
     * @brief Deleted copy constructor.
     *
     * MimeEntity objects cannot be copied to prevent unintended duplication
     * of potentially large content. If you need a copy of a MimeEntity,
     * you must explicitly create a new entity and populate it with the
     * content of the original.
     */
	MimeEntity(const MimeEntity&) = delete;
	
	/**
     * @brief Deleted copy assignment operator.
     *
     * MimeEntity objects cannot be copied to prevent unintended duplication
     * of potentially large content. If you need a copy of a MimeEntity,
     * you must explicitly create a new entity and populate it with the
     * content of the original.
     */
	MimeEntity& operator=(const MimeEntity&) = delete;
	
	/**
     * @brief Move constructor.
     *
     * Allows efficient transfer of resources from one MimeEntity object to another.
     * After the move operation, the source object is left in a valid but
     * unspecified state. All resources (headers, body, etc.) are transferred
     * to the new object.
     */
	MimeEntity(MimeEntity&&) noexcept;
	
	/**
     * @brief Move assignment operator.
     *
     * Allows efficient transfer of resources from one MimeEntity object to another.
     * After the move operation, the source object is left in a valid but
     * unspecified state. All resources (headers, body, etc.) are transferred
     * to the target object.
     */
	MimeEntity& operator=(MimeEntity&&) noexcept;

public:
	/**
     * @brief Gets the header section as a mutable reference.
     * @return Reference to the Header object.
     *
     * Provides read-write access to the header section of the MIME entity.
     * This allows adding, modifying, or removing headers as needed.
     * 
     * The Header object provides specialized methods for working with
     * important headers like Content-Type and Content-Transfer-Encoding.
     */
	Header& GetHeader();
	
	/**
     * @brief Gets the header section as a const reference.
     * @return Const reference to the Header object.
     *
     * Provides read-only access to the header section of the MIME entity.
     * This is useful for examining headers without modifying them.
     */
	const Header& GetHeader() const;

	/**
     * @brief Gets the body section as a mutable reference.
     * @return Reference to the Body object.
     *
     * Provides read-write access to the body section of the MIME entity.
     * This allows modifying the content and structure of the body,
     * including adding or removing parts in multipart bodies.
     * 
     * The Body object provides specialized methods for working with
     * different types of content and multipart structures.
     */
	Body& GetBody();
	
	/**
     * @brief Gets the body section as a const reference.
     * @return Const reference to the Body object.
     *
     * Provides read-only access to the body section of the MIME entity.
     * This is useful for examining the content without modifying it.
     */
	const Body& GetBody() const;

public:
	/**
     * @brief Checks if a specific header field exists.
     * @param field_name Name of the header field to check.
     * @return True if the field exists, false otherwise.
     *
     * This is a convenience method that delegates to the Header object's
     * HasField method. It performs a case-sensitive lookup for the specified
     * header field. Use GetHeader().HasFieldCaseInsensitive() for a
     * case-insensitive lookup.
     */
	bool HasField(const std::string& field_name) const;
	
	/**
     * @brief Stream insertion operator for MimeEntity objects.
     * @param os Output stream to write to.
     * @param entity MimeEntity object to output.
     * @return Reference to the output stream.
     *
     * Formats the MIME entity for output according to the MIME specifications,
     * with headers followed by an empty line and then the body content.
     * This operator is useful for serializing a MimeEntity for transmission
     * or storage.
     */
	friend std::ostream& operator<<(std::ostream& os, const MimeEntity& entity);

private:
	/**
     * @brief Writes the MIME entity to an output stream.
     * @param os Output stream to write to.
     * @param oel End-of-line sequence to use (default: "\r\n").
     * @return Reference to the output stream.
     *
     * This internal method handles the actual writing of the MIME entity
     * to the output stream. It formats the entity according to the MIME
     * specifications, with headers followed by an empty line and then the
     * body content. The end-of-line sequence can be customized, which is
     * useful for platforms with different line ending conventions.
     * 
     * This method is virtual to allow derived classes to customize the
     * output format if needed.
     */
	virtual std::ostream& Write(std::ostream& os, const char* oel = "\r\n") const;

private:
	/**
     * @brief Implementation class for MimeEntity.
     *
     * Uses the PIMPL idiom to hide implementation details and maintain
     * ABI compatibility. This allows the implementation to change without
     * requiring recompilation of code that uses the MimeEntity class,
     * as long as the public interface remains the same.
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
 * @brief Stream insertion operator for MimeEntity objects.
 * @param os Output stream to write to.
 * @param entity MimeEntity object to output.
 * @return Reference to the output stream.
 *
 * Global operator that enables writing MimeEntity objects to output streams.
 * This provides the same functionality as the friend operator within the
 * MimeEntity class, but is available for use in any context.
 * 
 * The MIME entity is formatted according to the MIME specifications,
 * with headers followed by an empty line and then the body content.
 */
std::ostream& operator<<(std::ostream& os, const MimeEntity& entity);

} // namespace ISXMime
