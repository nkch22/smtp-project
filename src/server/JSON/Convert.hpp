#pragma once

#include <string>

#include "JSON.hpp"

namespace ISXJson
{

template<typename T>
T Convert(const JSON& json);

template<>
inline std::string Convert<std::string>(const JSON& json)
{
	return json.AsString();
}

template<>
inline int Convert<int>(const JSON& json)
{
	return static_cast<int>(json.AsNumber());
}

template<>
inline double Convert<double>(const JSON& json)
{
	return json.AsNumber();
}

template<>
inline bool Convert<bool>(const JSON& json)
{
	return json.AsBool();
}
} // namespace ISXJson
