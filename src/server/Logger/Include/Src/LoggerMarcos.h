#pragma once
#include "../../../JSON/Macros.hpp"
#include "Buffer.h"

#define INNER_LOGGER_ACTION(value) buff << #value << ':' << obj.value;
#define INNER_LOGGER_LOOP(...) FOR_EACH(INNER_LOGGER_ACTION, __VA_ARGS__)

#define LOGGER_GET_PRIVATE(Type) friend struct logger::RealLoggerSerializer<Type>;

#define DEFINE_SER(Type, ...)                                                                                          \
	namespace logger                                                                                                   \
	{                                                                                                                  \
	template<>                                                                                                         \
	struct RealLoggerSerializer<Type>                                                                                  \
	{                                                                                                                  \
		static Buffer& serialize(Buffer& buff, const Type& obj)                                                        \
		{                                                                                                              \
			__VA_OPT__(INNER_LOGGER_LOOP(__VA_ARGS__));                                                                \
			return buff;                                                                                               \
		}                                                                                                              \
	};                                                                                                                 \
	}

#define LOGGABLE(Type)                                                                                                 \
	namespace logger                                                                                                   \
	{                                                                                                                  \
	inline Buffer& operator<<(Buffer& buff, const Type& obj)                                                           \
	{                                                                                                                  \
		LoggerSerializer<Type>::serialize(buff, obj);                                                                  \
		return buff;                                                                                                   \
	}                                                                                                                  \
	}

#define MAKE_LOGGABLE(Type, ...)                                                                                       \
	DEFINE_SER(Type, __VA_ARGS__)                                                                                      \
	LOGGABLE(Type)