#pragma once

namespace logger
{
enum LogLevels
{
	LOG_LEVEL_NO,
	LOG_LEVEL_PROD,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE
};

enum MessageTypes
{
	ERROR,
	WARNING,
	INFORMATION
};

template<typename T>
struct LoggerSerializer;

} // namespace logger

#define PP_IDENTITY(x) x

#define PP_CONCAT(a, b) PP_CONCAT_IMPL(a, b)
#define PP_CONCAT_IMPL(a, b) a##b

#define PP_NARG(...) PP_NARG_IMPL(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_IMPL(...) PP_ARG_N(__VA_ARGS__)

#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22,  \
				 _23, _24, _25, _26, _27, _28, _29, _30, N, ...)                                                       \
	N

#define PP_RSEQ_N()                                                                                                    \
	30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH(action, ...) PP_IDENTITY(PP_CONCAT(FOR_EACH_, PP_NARG(__VA_ARGS__))(action, __VA_ARGS__))

#define FOR_EACH_1(action, x) action(x)
#define FOR_EACH_2(action, x, ...) action(x) FOR_EACH_1(action, __VA_ARGS__)
#define FOR_EACH_3(action, x, ...) action(x) FOR_EACH_2(action, __VA_ARGS__)
#define FOR_EACH_4(action, x, ...) action(x) FOR_EACH_3(action, __VA_ARGS__)
#define FOR_EACH_5(action, x, ...) action(x) FOR_EACH_4(action, __VA_ARGS__)
#define FOR_EACH_6(action, x, ...) action(x) FOR_EACH_5(action, __VA_ARGS__)
#define FOR_EACH_7(action, x, ...) action(x) FOR_EACH_6(action, __VA_ARGS__)
#define FOR_EACH_8(action, x, ...) action(x) FOR_EACH_7(action, __VA_ARGS__)
#define FOR_EACH_9(action, x, ...) action(x) FOR_EACH_8(action, __VA_ARGS__)
#define FOR_EACH_10(action, x, ...) action(x) FOR_EACH_9(action, __VA_ARGS__)

#define DEFAULT_LEVEL logger::LOG_LEVEL_PROD
#define DEFAULT_AMOUNT 30
#define DEFAULT_PATH ""
#define DEFAULT_CONFIG false
#define DEFAULT_FLUSH true

#define DEFAULT_COLOR "\033[0m"
#define ERROR_COLOR "\033[41m"
#define WARNING_COLOR "\033[43m"
#define INFORMATION_COLOR "\033[42m"

#define INNER_LOGGER_SERIALIZATION(value) ser_buff << value;
#define INNER_LOGGER_LOOP(...) FOR_EACH(INNER_LOGGER_SERIALIZATION, __VA_ARGS__)

#define LOGGER_DEFINE_SERIALIZER(Type, ...)                                                                            \
	namespace logger                                                                                                   \
	{                                                                                                                  \
	template<>                                                                                                         \
	struct LoggerSerializer<Type>                                                                                      \
	{                                                                                                                  \
		Buffer& serialize(Buffer& ser_buff, const Type& value)                                                  \
		{                                                                                                              \
			value;                                                                                                     \
			__VA_OPT__(INNER_LOGGER_LOOP(__VA_ARGS__)) return ser_buff;                                                \
		}                                                                                                              \
	};                                                                                                                 \
	}

#define LOGGER_DEFINE(Type)                                                                                            \
	namespace logger                                                                                                   \
	{                                                                                                                  \
	inline Buffer& operator<<(Buffer& buff, const Type& obj)                                                           \
	{                                                                                                                  \
		LoggerSerializer<Type> ser; ser.serialize(buff, obj);                                                                  \
		return buff;                                                                                                   \
	}                                                                                                                  \
	}

#define MAKE_PRIVATE_LOGGER(Type) friend struct logger::LoggerSerializer<Type>;

#define MAKE_LOGGABLE(Type, ...)                                                                                       \
	LOGGER_DEFINE_SERIALIZER(Type, __VA_ARGS__)                                                                        \
	LOGGER_DEFINE(Type)
