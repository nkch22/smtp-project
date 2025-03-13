#pragma once
#include "SharedMacros.h"

#define INNER_LOGGER_ACTION(value) buff << '(' << #value << ':' << obj.value << ')';
#define INNER_LOGGER_LOOP(...) FOR_EACH(INNER_LOGGER_ACTION, __VA_ARGS__)

#define LOGGER_GET_PRIVATE(Type) friend logger::Buffer& operator<<(logger::Buffer&, const Type&);

#define MAKE_LOGGABLE(Type, ...)                                                                                       \
	logger::Buffer& operator<<(logger::Buffer& buff, const Type& obj)                                                  \
	{                                                                                                                  \
		__VA_OPT__(INNER_LOGGER_LOOP(__VA_ARGS__))                                                                     \
		return buff;                                                                                                   \
	}