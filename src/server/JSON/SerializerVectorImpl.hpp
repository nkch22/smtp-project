
/**
 * @file SerializerVectorImpl.hpp
 * @brief Implementation of the std::vector serialization for JSON.
 *
 * @details
 * This header provides the implementation of the Serializer template specialization
 * for std::vector<T>. It is included at the end of the JSON header to avoid circular
 * dependencies, as it requires the full definition of the JSON class.
 */

#pragma once

#include <vector>

#include "JSON.hpp"

namespace ISXJson
{

/**
 * @brief Implementation of vector serialization.
 *
 * @details
 * This implements the Serializer specialization for std::vector<T>.
 * For serialization, each element is converted to JSON using the appropriate
 * Serializer<T> specialization. For deserialization, each JSON value in the
 * array is converted back to type T.
 */

/**
 * @brief Converts a vector to a JSON array.
 *
 * @details
 * Creates a JSON array and populates it with JSON values created by
 * serializing each element of the input vector.
 *
 * @tparam T The type of elements in the vector.
 * @param vec The vector to serialize.
 * @return A JSON array containing the serialized elements.
 */
template<typename T>
JSON Serializer<std::vector<T>>::Serialize(const std::vector<T>& vec)
{
	JSON::Array arr;
	for (const auto& element : vec)
	{
		arr.push_back(JSON::From(element));
	}
	return JSON(arr);
}

/**
 * @brief Converts a JSON array to a vector.
 *
 * @details
 * Creates a vector and populates it with values created by
 * deserializing each element of the input JSON array.
 *
 * @tparam T The type of elements in the vector.
 * @param json The JSON array to deserialize.
 * @return A vector containing the deserialized elements.
 * @throw std::runtime_error If the JSON value is not an array or
 *                          if any element can't be converted to type T.
 */
template<typename T>
std::vector<T> Serializer<std::vector<T>>::Deserialize(const JSON& json)
{
	const auto& arr = json.AsArray();
	std::vector<T> vec;
	vec.reserve(arr.size());
	for (const auto& element : arr)
	{
		vec.push_back(element.To<T>());
	}
	return vec;
}

} // namespace ISXJson
