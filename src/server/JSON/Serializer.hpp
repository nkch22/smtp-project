#pragma once

namespace ISXJson
{

template<typename T>
struct Serializer
{
	static_assert(sizeof(T) == 0, "No JSONSerializer specialization found for this type");

	// static JSON Serialize(const T& value);
	// static T Deserialize(const JSON& json);
};

} // namespace ISXJson
