/**
 * @file BinarySerializer.hpp
 * @brief Binary serialization for JSON objects
 *
 * @details
 * This header provides functions to serialize JSON objects to binary format
 * and deserialize binary data back to JSON objects. It implements a simple
 * MessagePack-inspired binary format that is more efficient than text-based
 * JSON for storage and transmission.
 *
 * The binary format uses type markers followed by data, allowing for compact
 * representation of various JSON data types including null, boolean, numbers,
 * strings, arrays, and objects.
 *
 * @section binary_serialization_example Binary Serialization Example
 * @code
 * #include "JSON/JSON.hpp"
 * #include "JSON/BinarySerializer.hpp"
 * #include <iostream>
 * #include <fstream>
 *
 * // Create a JSON object
 * ISXJson::JSON json;
 * json["name"] = "Test Object";
 * json["value"] = 42;
 * json["active"] = true;
 *
 * // Serialize to binary
 * std::vector<uint8_t> binary = ISXJson::BinarySerializer::Serialize(json);
 *
 * std::cout << "Binary size: " << binary.size() << " bytes" << std::endl;
 *
 * // Save to file
 * std::ofstream file("data.bin", std::ios::binary);
 * file.write(reinterpret_cast<const char*>(binary.data()), binary.size());
 * file.close();
 *
 * // Load from file
 * std::ifstream infile("data.bin", std::ios::binary | std::ios::ate);
 * std::streamsize size = infile.tellg();
 * infile.seekg(0, std::ios::beg);
 *
 * std::vector<uint8_t> loaded(size);
 * infile.read(reinterpret_cast<char*>(loaded.data()), size);
 *
 * // Deserialize back to JSON
 * ISXJson::JSON restored = ISXJson::BinarySerializer::Deserialize(loaded);
 *
 * // Verify
 * std::cout << "Restored JSON: " << restored.Serialize(true) << std::endl;
 * @endcode
 *
 * @section direct_serialization_example Direct Type Serialization
 * @code
 * #include "JSON/JSONAll.hpp"
 * #include "JSON/BinarySerializer.hpp"
 *
 * // Define a custom type
 * struct User
 * {
 *     std::string username;
 *     int id;
 *     bool active;
 *     std::vector<std::string> roles;
 * };
 *
 * // Define serializer
 * JSON_DEFINE_SERIALIZER(User, username, id, active, roles)
 *
 * // Create a user
 * User user;
 * user.username = "admin";
 * user.id = 1001;
 * user.active = true;
 * user.roles = {"admin", "moderator"};
 *
 * // Serialize directly to binary
 * std::vector<uint8_t> binary = ISXJson::BinarySerializer::ToBinary(user);
 *
 * // Later, deserialize directly to User
 * User restoredUser = ISXJson::BinarySerializer::FromBinary<User>(binary);
 * @endcode
 */

#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "JSON.hpp"

namespace ISXJson
{

/**
 * @class BinarySerializer
 * @brief Binary serializer for JSON objects
 *
 * This class provides static methods to convert JSON objects to binary format
 * and back. The implementation is inspired by the MessagePack format
 * but simplified for our specific needs.
 *
 * The binary format uses type markers to indicate the data type, followed by
 * the actual data in a compact binary representation. This approach allows
 * for efficient storage and transmission of JSON data compared to text-based
 * JSON strings.
 *
 * In addition to JSON serialization methods, this class also provides template
 * functions for direct conversion between any type and binary format, using
 * JSON as an intermediate representation.
 */
class BinarySerializer
{
public:
	/**
	 * @brief Serialize a JSON object to binary format
	 *
	 * @param json The JSON object to serialize
	 * @return A vector of bytes containing the serialized data
	 *
	 * @details
	 * This method converts a JSON object to a binary representation. The binary
	 * format consists of type markers followed by data. The exact format depends
	 * on the JSON type:
	 * - Null: single byte marker
	 * - Boolean: single byte marker (different for true and false)
	 * - Number: marker + binary representation of the number
	 * - String: marker + length + string data
	 * - Array: marker + length + serialized items
	 * - Object: marker + size + serialized key/value pairs
	 */
	static std::vector<uint8_t> Serialize(const JSON& json);

	/**
	 * @brief Deserialize binary data back to a JSON object
	 *
	 * @param data The binary data to deserialize
	 * @return The deserialized JSON object
	 * @throw std::runtime_error If deserialization fails due to invalid format
	 *
	 * @details
	 * This method converts binary data created by Serialize() back into a JSON object.
	 * It reads the type marker to determine the data type, then reads the appropriate
	 * amount of data based on the type. For compound types (arrays and objects),
	 * it recursively deserializes all contained elements.
	 */
	static JSON Deserialize(const std::vector<uint8_t>& data);

	/**
	 * @brief Serialize any type to binary format
	 *
	 * @tparam T Type to serialize (must have a JSON serializer defined)
	 * @param value The value to serialize
	 * @return A vector of bytes containing the serialized data
	 *
	 * @details
	 * This function provides a convenient way to serialize any type directly to binary
	 * without having to manually convert to JSON first. It internally uses the
	 * JSON::From<T> template method to convert the value to JSON, and then
	 * BinarySerializer::Serialize to convert the JSON to binary.
	 *
	 * For this function to work, the type T must have a JSON serializer defined,
	 * typically using the JSON_DEFINE_SERIALIZER macro or a custom specialization
	 * of the Serializer template.
	 */
	template<typename T>
	static std::vector<uint8_t> ToBinary(const T& value)
	{
		// First convert to JSON using the existing serializer
		JSON json = JSON::From(value);

		// Then convert JSON to binary
		return Serialize(json);
	}

	/**
	 * @brief Deserialize binary data to any type
	 *
	 * @tparam T Type to deserialize to (must have a JSON serializer defined)
	 * @param data The binary data to deserialize
	 * @return The deserialized value
	 * @throw std::runtime_error If deserialization fails
	 *
	 * @details
	 * This function provides a convenient way to deserialize binary data directly to
	 * any type without having to manually convert from JSON. It internally uses
	 * BinarySerializer::Deserialize to convert the binary to JSON, and then
	 * JSON::To<T> to convert the JSON to the desired type.
	 *
	 * For this function to work, the type T must have a JSON serializer defined,
	 * typically using the JSON_DEFINE_SERIALIZER macro or a custom specialization
	 * of the Serializer template.
	 */
	template<typename T>
	static T FromBinary(const std::vector<uint8_t>& data)
	{
		// First convert binary to JSON
		JSON json = Deserialize(data);

		// Then convert JSON to the desired type
		return json.To<T>();
	}

private:
	/**
	 * @brief Format types used in binary serialization
	 *
	 * These type markers are used to identify the data type in the binary format.
	 * The values are compatible with a subset of the MessagePack specification.
	 */
	enum class FormatType : uint8_t
	{
		NUL = 0xC0,	 ///< Null value
		FALSE = 0xC2,	 ///< Boolean false
		TRUE = 0xC3,	 ///< Boolean true
		INT8 = 0xD0,	 ///< 8-bit signed integer
		INT16 = 0xD1,	 ///< 16-bit signed integer
		INT32 = 0xD2,	 ///< 32-bit signed integer
		INT64 = 0xD3,	 ///< 64-bit signed integer
        UINT64 = 0xDD, ///< 64-bit unsigned integer
		FLOAT64 = 0xCB,	 ///< 64-bit floating point number
		STRING8 = 0xD9,	 ///< String with 8-bit length
		STRING16 = 0xDA, ///< String with 16-bit length
		STRING32 = 0xDB, ///< String with 32-bit length
		STRING64 = 0xE2, ///< String with 64-bit length
		ARRAY16 = 0xDC,	 ///< Array with 16-bit item count
		ARRAY32 = 0xDD,	 ///< Array with 32-bit item count
		ARRAY64 = 0xE0,   ///< Array with 64-bit item count
		MAP16 = 0xDE,	 ///< Map with 16-bit pair count
		MAP32 = 0xDF,	 ///< Map with 32-bit pair count
		MAP64 = 0xE1,     ///< Map with 64-bit pair count
	};

private:
	static void WriteUint8(std::vector<uint8_t>& buffer, uint8_t value);
	static void WriteUint16(std::vector<uint8_t>& buffer, uint16_t value);
	static void WriteUint32(std::vector<uint8_t>& buffer, uint32_t value);
	static void WriteUint64(std::vector<uint8_t>& buffer, uint64_t value);
	static void WriteDouble(std::vector<uint8_t>& buffer, double value);
	static void WriteString(std::vector<uint8_t>& buffer, const std::string& str);
	static void SerializeValue(std::vector<uint8_t>& buffer, const JSON& json);

private:
	static uint8_t ReadUint8(const std::vector<uint8_t>& buffer, size_t& pos);
	static uint16_t ReadUint16(const std::vector<uint8_t>& buffer, size_t& pos);
	static uint32_t ReadUint32(const std::vector<uint8_t>& buffer, size_t& pos);
	static uint64_t ReadUint64(const std::vector<uint8_t>& buffer, size_t& pos);
	static double ReadDouble(const std::vector<uint8_t>& buffer, size_t& pos);
	static std::string ReadString(const std::vector<uint8_t>& buffer, size_t& pos, size_t length);
	static JSON DeserializeValue(const std::vector<uint8_t>& buffer, size_t& pos);
};

} // namespace ISXJson
