/**
 * @file BinarySerializer.cpp
 * @brief Implementation of the BinarySerializer class.
 *
 * @details
 * This file implements the binary serialization methods for JSON objects.
 * It contains the code for converting between JSON objects and their binary
 * representations using a compact binary format inspired by MessagePack.
 */

#include "BinarySerializer.hpp"

#include <cstring>
#include <limits>

namespace ISXJson
{

/**
 * @brief Serialize a JSON object to binary format
 *
 * Creates a buffer and calls SerializeValue to recursively process the JSON structure.
 *
 * @param json The JSON object to serialize
 * @return A vector of bytes containing the serialized data
 */
std::vector<uint8_t> BinarySerializer::Serialize(const JSON& json)
{
	std::vector<uint8_t> buffer;
	SerializeValue(buffer, json);
	return buffer;
}

/**
 * @brief Deserialize binary data back to a JSON object
 *
 * Validates that the data is not empty and calls DeserializeValue to process the binary format.
 *
 * @param data The binary data to deserialize
 * @return The deserialized JSON object
 * @throw std::runtime_error If deserialization fails due to empty data or invalid format
 */
JSON BinarySerializer::Deserialize(const std::vector<uint8_t>& data)
{
	if (data.empty()) throw std::runtime_error("Cannot deserialize empty binary data");

	size_t pos = 0;
	return DeserializeValue(data, pos);
}

void BinarySerializer::WriteUint8(std::vector<uint8_t>& buffer, uint8_t value)
{
	buffer.push_back(value);
}

void BinarySerializer::WriteUint16(std::vector<uint8_t>& buffer, uint16_t value)
{
	buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
	buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void BinarySerializer::WriteUint32(std::vector<uint8_t>& buffer, uint32_t value)
{
	buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
	buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void BinarySerializer::WriteUint64(std::vector<uint8_t>& buffer, uint64_t value)
{
	buffer.push_back(static_cast<uint8_t>((value >> 56) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 48) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 40) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 32) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
	buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void BinarySerializer::WriteDouble(std::vector<uint8_t>& buffer, double value)
{
	// Interpret the double as 64 bits
	uint64_t bits;
	std::memcpy(&bits, &value, sizeof(double));
	WriteUint64(buffer, bits);
}

void BinarySerializer::WriteString(std::vector<uint8_t>& buffer, const std::string& str)
{
	const size_t LENGTH = str.length();

	if (LENGTH <= std::numeric_limits<uint8_t>::max())
	{
		buffer.push_back(static_cast<uint8_t>(FormatType::STRING8));
		WriteUint8(buffer, static_cast<uint8_t>(LENGTH));
	}
	else if (LENGTH <= std::numeric_limits<uint16_t>::max())
	{
		buffer.push_back(static_cast<uint8_t>(FormatType::STRING16));
		WriteUint16(buffer, static_cast<uint16_t>(LENGTH));
	}
	else if (LENGTH <= std::numeric_limits<uint32_t>::max())
	{
		buffer.push_back(static_cast<uint8_t>(FormatType::STRING32));
		WriteUint32(buffer, static_cast<uint32_t>(LENGTH));
	}
	else
	{
		buffer.push_back(static_cast<uint8_t>(FormatType::STRING64));
		WriteUint64(buffer, static_cast<uint64_t>(LENGTH));
	}

	buffer.insert(buffer.end(), str.begin(), str.end());
}

void BinarySerializer::SerializeValue(std::vector<uint8_t>& buffer, const JSON& json)
{
	switch (json.GetType())
	{
	case JSON::Type::NUL:
		buffer.push_back(static_cast<uint8_t>(FormatType::NUL));
		break;

	case JSON::Type::BOOL:
		buffer.push_back(static_cast<uint8_t>(json.AsBool() ? FormatType::TRUE : FormatType::FALSE));
		break;

	case JSON::Type::NUMBER:
	{
		double number = json.AsNumber();
		double intpart;

		// Check if the number is an integer
		if (std::modf(number, &intpart) == 0.0)
		{
			auto int_value = static_cast<int64_t>(intpart);
			if (int_value >= std::numeric_limits<int8_t>::min() && int_value <= std::numeric_limits<int8_t>::max())
			{
				buffer.push_back(static_cast<uint8_t>(FormatType::INT8));
				WriteUint8(buffer, static_cast<uint8_t>(int_value));
			}
			else if (int_value >= std::numeric_limits<int16_t>::min()
					 && int_value <= std::numeric_limits<int16_t>::max())
			{
				buffer.push_back(static_cast<uint8_t>(FormatType::INT16));
				WriteUint16(buffer, static_cast<uint16_t>(int_value));
			}
			else if (int_value >= std::numeric_limits<int32_t>::min()
					 && int_value <= std::numeric_limits<int32_t>::max())
			{
				buffer.push_back(static_cast<uint8_t>(FormatType::INT32));
				WriteUint32(buffer, static_cast<uint32_t>(int_value));
			}
			else
			{
				buffer.push_back(static_cast<uint8_t>(FormatType::INT64));
				WriteUint64(buffer, static_cast<uint64_t>(int_value));
			}
		}
		else
		{
			// Float value
			buffer.push_back(static_cast<uint8_t>(FormatType::FLOAT64));
			WriteDouble(buffer, number);
		}
		break;
	}

	case JSON::Type::STRING:
		WriteString(buffer, json.AsString());
		break;

	case JSON::Type::ARRAY:
	{
		const auto& array = json.AsArray();
		const size_t SIZE = array.size();

		if (SIZE <= std::numeric_limits<uint16_t>::max())
		{
			buffer.push_back(static_cast<uint8_t>(FormatType::ARRAY16));
			WriteUint16(buffer, static_cast<uint16_t>(SIZE));
		}
		else if (SIZE <= std::numeric_limits<uint32_t>::max())
		{
			buffer.push_back(static_cast<uint8_t>(FormatType::ARRAY32));
			WriteUint32(buffer, static_cast<uint32_t>(SIZE));
		}
		else
		{
			buffer.push_back(static_cast<uint8_t>(FormatType::ARRAY64));
			WriteUint64(buffer, static_cast<uint64_t>(SIZE));
		}

		for (const auto& element : array)
		{
			SerializeValue(buffer, element);
		}
		break;
	}

	case JSON::Type::OBJECT:
	{
		const auto& object = json.AsObject();
		const size_t SIZE = object.size();

		if (SIZE <= std::numeric_limits<uint16_t>::max())
		{
			buffer.push_back(static_cast<uint8_t>(FormatType::MAP16));
			WriteUint16(buffer, static_cast<uint16_t>(SIZE));
		}
		else if (SIZE <= std::numeric_limits<uint32_t>::max())
		{
			buffer.push_back(static_cast<uint8_t>(FormatType::MAP32));
			WriteUint32(buffer, static_cast<uint32_t>(SIZE));
		}
		else
		{
			buffer.push_back(static_cast<uint8_t>(FormatType::MAP64));
			WriteUint64(buffer, static_cast<uint64_t>(SIZE));
		}

		for (const auto& [key, value] : object)
		{
			WriteString(buffer, key);
			SerializeValue(buffer, value);
		}
		break;
	}

	default:
		throw std::runtime_error("Unknown JSON type in binary serialization");
	}
}

uint8_t BinarySerializer::ReadUint8(const std::vector<uint8_t>& buffer, size_t& pos)
{
	if (pos >= buffer.size()) throw std::runtime_error("Reading past end of buffer");

	return buffer[pos++];
}

uint16_t BinarySerializer::ReadUint16(const std::vector<uint8_t>& buffer, size_t& pos)
{
	if (pos + 1 >= buffer.size()) throw std::runtime_error("Reading past end of buffer");

	uint16_t value = static_cast<uint16_t>(buffer[pos]) << 8;
	value |= buffer[pos + 1];
	pos += 2;

	return value;
}

uint32_t BinarySerializer::ReadUint32(const std::vector<uint8_t>& buffer, size_t& pos)
{
	if (pos + 3 >= buffer.size()) throw std::runtime_error("Reading past end of buffer");

	uint32_t value = static_cast<uint32_t>(buffer[pos]) << 24;
	value |= static_cast<uint32_t>(buffer[pos + 1]) << 16;
	value |= static_cast<uint32_t>(buffer[pos + 2]) << 8;
	value |= buffer[pos + 3];
	pos += 4;

	return value;
}

uint64_t BinarySerializer::ReadUint64(const std::vector<uint8_t>& buffer, size_t& pos)
{
	if (pos + 7 >= buffer.size()) throw std::runtime_error("Reading past end of buffer");

	uint64_t value = static_cast<uint64_t>(buffer[pos]) << 56;
	value |= static_cast<uint64_t>(buffer[pos + 1]) << 48;
	value |= static_cast<uint64_t>(buffer[pos + 2]) << 40;
	value |= static_cast<uint64_t>(buffer[pos + 3]) << 32;
	value |= static_cast<uint64_t>(buffer[pos + 4]) << 24;
	value |= static_cast<uint64_t>(buffer[pos + 5]) << 16;
	value |= static_cast<uint64_t>(buffer[pos + 6]) << 8;
	value |= buffer[pos + 7];
	pos += 8;

	return value;
}

double BinarySerializer::ReadDouble(const std::vector<uint8_t>& buffer, size_t& pos)
{
	uint64_t bits = ReadUint64(buffer, pos);
	double value;
	std::memcpy(&value, &bits, sizeof(double));
	return value;
}

std::string BinarySerializer::ReadString(const std::vector<uint8_t>& buffer, size_t& pos, size_t length)
{
	if (pos + length > buffer.size()) throw std::runtime_error("Reading past end of buffer");

	std::string str(buffer.begin() + pos, buffer.begin() + pos + length);
	pos += length;

	return str;
}

JSON BinarySerializer::DeserializeValue(const std::vector<uint8_t>& buffer, size_t& pos)
{
	if (pos >= buffer.size()) throw std::runtime_error("Reading past end of buffer");

	auto type = static_cast<FormatType>(buffer[pos++]);

	switch (type)
	{
	case FormatType::NUL:
		return {};

	case FormatType::FALSE:
		return {false};

	case FormatType::TRUE:
		return {true};

	case FormatType::INT8:
	{
		auto value = static_cast<int8_t>(ReadUint8(buffer, pos));
		return {static_cast<double>(value)};
	}

	case FormatType::INT16:
	{
		auto value = static_cast<int16_t>(ReadUint16(buffer, pos));
		return {static_cast<double>(value)};
	}

	case FormatType::INT32:
	{
		auto value = static_cast<int32_t>(ReadUint32(buffer, pos));
		return {static_cast<double>(value)};
	}

	case FormatType::INT64:
	{
		auto value = static_cast<int64_t>(ReadUint64(buffer, pos));
		return {static_cast<double>(value)};
	}

	case FormatType::FLOAT64:
	{
		double value = ReadDouble(buffer, pos);
		return {value};
	}

	case FormatType::STRING8:
	{
		uint8_t length = ReadUint8(buffer, pos);
		return JSON(ReadString(buffer, pos, length));
	}

	case FormatType::STRING16:
	{
		uint16_t length = ReadUint16(buffer, pos);
		return JSON(ReadString(buffer, pos, length));
	}

	case FormatType::STRING32:
	{
		uint32_t length = ReadUint32(buffer, pos);
		return JSON(ReadString(buffer, pos, length));
	}

	case FormatType::STRING64:
	{
		uint64_t length = ReadUint64(buffer, pos);
		return JSON(ReadString(buffer, pos, length));
	}

	case FormatType::ARRAY16:
	case FormatType::ARRAY32:
	{
		uint32_t size = (type == FormatType::ARRAY16) ? ReadUint16(buffer, pos) : ReadUint32(buffer, pos);

		JSON::Array array;
		try
		{
			for (uint32_t i = 0; i < size; ++i) array.push_back(DeserializeValue(buffer, pos));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error deserializing array element: ") + e.what());
		}

		return JSON(array);
	}

	case FormatType::ARRAY64:
	{
		uint64_t size = ReadUint64(buffer, pos);
		JSON::Array array;
		try
		{
			for (uint64_t i = 0; i < size; ++i) array.push_back(DeserializeValue(buffer, pos));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error deserializing array element: ") + e.what());
		}
		return JSON(array);
	}

	case FormatType::MAP16:
	case FormatType::MAP32:
	{
		uint32_t size = (type == FormatType::MAP16) ? ReadUint16(buffer, pos) : ReadUint32(buffer, pos);

		JSON::Object object;
		try
		{
			for (uint32_t i = 0; i < size; ++i)
			{
				if (pos >= buffer.size()) throw std::runtime_error("Reading past end of buffer when reading map key");

				auto key_type = static_cast<FormatType>(buffer[pos++]);
				std::string key;

				if (key_type == FormatType::STRING8)
				{
					uint8_t length = ReadUint8(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else if (key_type == FormatType::STRING16)
				{
					uint16_t length = ReadUint16(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else if (key_type == FormatType::STRING32)
				{
					uint32_t length = ReadUint32(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else if (key_type == FormatType::STRING64)
				{
					uint64_t length = ReadUint64(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else
				{
					throw std::runtime_error("Object key must be a string");
				}

				object[key] = DeserializeValue(buffer, pos);
			}
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error deserializing object: ") + e.what());
		}

		return JSON(object);
	}

	case FormatType::MAP64:
	{
		uint64_t size = ReadUint64(buffer, pos);
		JSON::Object object;
		try
		{
			for (uint64_t i = 0; i < size; ++i)
			{
				if (pos >= buffer.size()) throw std::runtime_error("Reading past end of buffer when reading map key");

				auto key_type = static_cast<FormatType>(buffer[pos++]);
				std::string key;

				if (key_type == FormatType::STRING8)
				{
					uint8_t length = ReadUint8(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else if (key_type == FormatType::STRING16)
				{
					uint16_t length = ReadUint16(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else if (key_type == FormatType::STRING32)
				{
					uint32_t length = ReadUint32(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else if (key_type == FormatType::STRING64)
				{
					uint64_t length = ReadUint64(buffer, pos);
					key = ReadString(buffer, pos, length);
				}
				else
				{
					throw std::runtime_error("Object key must be a string");
				}

				object[key] = DeserializeValue(buffer, pos);
			}
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error deserializing object: ") + e.what());
		}

		return JSON(object);
	}

	default:
		throw std::runtime_error("Unknown format type in binary deserialization");
	}
}

} // namespace ISXJson
