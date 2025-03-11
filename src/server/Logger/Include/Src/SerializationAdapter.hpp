#pragma once

#include <typeinfo>

#include "../../../JSON/Serializer.hpp"
#include "../Logger.h"

namespace logger
{
template<typename T>
class SerializationAdapter
{
public:
	static void Serialize(logger::Buffer& buff, const T& obj)
	{
		try
		{
			auto json = ISXJson::Serializer<T>::Serialize(obj);
			buff << json.Serialize();
		}
		catch (const std::exception&)
		{
			buff << "[Object of type '" << typeid(T).name() << "' - serialization not available]";
		}
	}
};

#define DEFINE_LOGGABLE(Type)                                                                                          \
	namespace logger                                                                                                   \
	{                                                                                                                  \
	inline Buffer& operator<<(Buffer& buff, const Type& obj)                                                           \
	{                                                                                                                  \
		SerializationAdapter<Type>::Serialize(buff, obj);                                                              \
		return buff;                                                                                                   \
	}                                                                                                                  \
	}

} // namespace logger
