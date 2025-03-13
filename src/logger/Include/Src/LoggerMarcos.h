#pragma once
#include "RealLoggerSerializer.h"
#include "SharedMacros.h"

#define INNER_LOGGER_ACTION(value) buff << '(' << #value << ':' << obj.value << ')';
#define INNER_LOGGER_LOOP(...) FOR_EACH(INNER_LOGGER_ACTION, __VA_ARGS__)

#define LOGGER_GET_PRIVATE(Type) friend struct ::logger_serialization::RealLoggerSerializer<Type>;

#define DEFINE_SER(Type, ...)                                                                                          \
	template<>                                                                                                         \
	struct logger_serialization::RealLoggerSerializer<Type>                                                          \
	{                                                                                                                  \
		static logger::Buffer& serialize(logger::Buffer& buff, const Type& obj)                                        \
		{                                                                                                              \
			__VA_OPT__(INNER_LOGGER_LOOP(__VA_ARGS__))                                                                 \
			return buff;                                                                                               \
		}                                                                                                              \
	};

#define LOGGABLE(Type)                                                                                                 \
	inline logger::Buffer& operator<<(logger::Buffer& buff, const Type& obj)                                           \
	{                                                                                                                  \
		::logger_serialization::LoggerSerializer<Type>::serialize(buff, obj);                                          \
		return buff;                                                                                                   \
	}

#define MAKE_LOGGABLE(Type, ...)                                                                                       \
	DEFINE_SER(Type, __VA_ARGS__)                                                                                      \
	LOGGABLE(Type)