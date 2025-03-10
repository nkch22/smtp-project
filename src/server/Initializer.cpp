#include "Initializer.hpp"

#include <exception>
#include <filesystem>

#include "Logger/Include/Logger.h"

void Initializer::Init(const std::filesystem::path& configPath)
{
	StartLoggerWithDefaults();

	InitLogger(LoadConfig(configPath));

	// InitServer
}

bool Initializer::StartLoggerWithDefaults()
{
	return logger::Logger::init(logger::LOG_LEVEL_TRACE, DEFAULT_PATH, DEFAULT_AMOUNT, true);
}

Config Initializer::LoadConfig(const std::filesystem::path& configPath)
{
	try
	{
		Config config(configPath);
		return config;
	}
	catch (const std::exception& e)
	{
		logger::Logger::stop_config();
		throw e;
	}
}

void Initializer::InitLogger(const Config& config)
{
	Config::Logging logger_config = config.GetLogging();

	logger::Logger logger;
	logger.set_global_level(static_cast<logger::LogLevels>(logger_config.log_level));
	logger::Logger::set_output_dir(logger_config.logs_directory);

	logger::Logger::stop_config();
}

void Initializer::InitServer() {}
