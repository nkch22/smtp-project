/**
 * @file Serializer.cpp
 *
 * @details
 * This file implements the Serializer specializations for basic types.
 * It provides the conversion functions between C++ types and JSON values.
 */

#include "Serializer.hpp"

#include "JSON.hpp"

namespace ISXJson
{

// Implementation of the std::string serializer
JSON Serializer<std::string>::Serialize(const std::string& value)
{
	return JSON(value);
}

std::string Serializer<std::string>::Deserialize(const JSON& json)
{
	return json.AsString();
}

// Implementation of the int serializer
JSON Serializer<int>::Serialize(int value)
{
	return JSON(value);
}

int Serializer<int>::Deserialize(const JSON& json)
{
	return static_cast<int>(json.AsNumber());
}

// Implementation of the bool serializer
JSON Serializer<bool>::Serialize(bool value)
{
	return JSON(value);
}

bool Serializer<bool>::Deserialize(const JSON& json)
{
	return json.AsBool();
}

// Implementation of the double serializer
JSON Serializer<double>::Serialize(double value)
{
	return JSON(value);
}

double Serializer<double>::Deserialize(const JSON& json)
{
	return json.AsNumber();
}

// Vector serialization implementations are in SerializerVectorImpl.hpp
// We can't fully implement the vector specialization here because it's a template
// Instead, we provide explicit instantiation for common vector types as needed

} // namespace ISXJson
