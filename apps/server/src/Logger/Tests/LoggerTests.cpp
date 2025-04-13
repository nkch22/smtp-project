#include <gtest/gtest.h>
#include "../Include/Logger.h"

using namespace logger;

TEST(LoggerTest, LogMessage) {
	LOGGER(log);

	ASSERT_NO_THROW(log.log_message("LogMessage"));
	ASSERT_NO_THROW(log.log_warning("LogMessage"));
	ASSERT_NO_THROW(log.log_error("LogMessage"));
}

TEST(LoggerTest, CustomMessageType) {
	LOGGER(log);

	CREATE_TYPE(mes_type, " MT ", DEFAULT_COLOR);
	
	ASSERT_NO_THROW(log.log_any("Hello, world!", mes_type));
}

TEST(LoggerTest, GetCustomGlobalLogLevel)
{
	LOGGER(log);

	ASSERT_EQ(log.get_global_level(), GET_LOG_LEVEL(4));
}

TEST(LoggerTest, LocalLogLeve) {
	LOGGER(log);
	log.set_local_level(TRACE_LOG_LEVEL);

	ASSERT_NE(log.get_local_level(), log.get_global_level());
}

TEST(LoggerTest, ReInit) {
	LOGGER(log);

	Logger::destroy();
	
	ASSERT_ANY_THROW(log.log_message("Hello"));

	Logger::init(TRACE_LOG_LEVEL, "", 1);

	LOGGER(new_log);

	ASSERT_NO_THROW(new_log.log_message("Hello"));
}

int main(int args, char** arg) {
	testing::InitGoogleTest(&args, arg);

	CREATE_LOG_LEVEL(TEST_LEVEL, "{:m}");

	Logger::init(TEST_LEVEL, "", 1);

	return RUN_ALL_TESTS();
}