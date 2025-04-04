#pragma once

#include "../Config/Config.hpp"

class Initializer
{
public:
	static void Init(const std::filesystem::path& configPath);

private:
	static bool StartLoggerWithDefaults();
	static Config LoadConfig(const std::filesystem::path& configPath);
	static void InitLogger(const Config& config);
    static void InitServer();
};
