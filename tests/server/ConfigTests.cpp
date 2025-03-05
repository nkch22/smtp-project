#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
#include <vector>

#include "../../src/server/Config/Config.hpp"

namespace
{
// Helper functions for temporary files (now with internal linkage)
std::string CreateTempFile(const std::string& content)
{
	std::string filename = "config_temp.json";
	std::ofstream file(filename);
	if (!file) throw std::runtime_error("Failed to create temp file");
	file << content;
	file.close();
	return filename;
}

void DeleteTempFile(const std::string& filename)
{
	std::remove(filename.c_str());
}
} // namespace

// Valid configuration tests
// Test: LoadsFullConfiguration
// Verifies that a complete valid configuration is parsed correctly,
// and that the returned values for Server, Communication, Logging,
// and Threads exactly match the expected values.
TEST(ConfigTest, LoadsFullConfiguration)
{
	// Tests complete valid configuration parsing
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "TestServer",
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 8
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	// Verify server configuration
	auto server = config.GetServer();
	EXPECT_EQ(server.server_name, "TestServer");
	EXPECT_EQ(server.server_display_name, "Test Server");
	EXPECT_EQ(server.port, 8080);
	EXPECT_EQ(server.ip, "192.168.1.1");

	// Verify communication settings
	auto comm = config.GetCommunication();
	EXPECT_TRUE(comm.blocking);
	EXPECT_EQ(comm.socket_timeout, 5000);

	// Verify logging configuration
	auto logging = config.GetLogging();
	EXPECT_EQ(logging.logs_directory, "logs/");
	EXPECT_EQ(logging.log_level, 2);
	EXPECT_FALSE(logging.flush);

	// Verify thread settings
	auto threads = config.GetThreads();
	EXPECT_EQ(threads.period_time, 30);
	EXPECT_EQ(threads.max_working_threads, 8);
}

// Test: HandlesMissingRoot
// Checks that a configuration missing the required "root" element throws an error.
TEST(ConfigTest, HandlesMissingRoot)
{
	// Tests missing root element detection
	std::string json_content = R"({ "invalid": {} })";
	std::string filename = CreateTempFile(json_content);
	EXPECT_THROW(Config config(filename), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: HandlesMissingSections
// Verifies that if any of the required configuration sections is missing,
// the Config constructor throws an error.
TEST(ConfigTest, HandlesMissingSections)
{
	// Tests detection of missing required sections
	std::vector<std::string> sections = {R"("Server")", R"("communicationsettings")", R"("logging")", R"("time")",
										 R"("threadpool")"};

	for (const auto& section : sections)
	{
		(void)section;
		std::string json_content = R"({
            "root": {
                "not_the_section": {}
            }
        })";
		// The test expects a throw since required sections are missing.
		std::string filename = CreateTempFile(json_content);
		EXPECT_THROW(Config config(filename), std::runtime_error);
		DeleteTempFile(filename);
	}
}

// Test: ValidatesFieldTypes
// Confirms that when specific configuration fields are replaced with values of the wrong type,
// parsing fails and an exception is thrown.
TEST(ConfigTest, ValidatesFieldTypes)
{
	struct FieldTest
	{
		std::string field;
		std::string target; // the exact substring to replace
		std::string invalid_value;
	};

	std::vector<FieldTest> tests = {{.field = "root/Server/servername",
									 .target = R"("servername": "Test")",
									 .invalid_value = R"("servername": 123)"},
									{.field = "root/Server/listenerport",
									 .target = R"("listenerport": 8080)",
									 .invalid_value = R"("listenerport": "8080")"},
									{.field = "root/communicationsettings/blocking",
									 .target = R"("blocking": 1)",
									 .invalid_value = R"("blocking": "true")"},
									{.field = "root/logging/LogLevel",
									 .target = R"("LogLevel": 2)",
									 .invalid_value = R"("LogLevel": "debug")"}};

	std::string base_json = R"({
        "root": {
            "Server": {
                "servername": "Test",
                "serverdisplayname": "Test",
                "listenerport": 8080,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {"blocking": 1, "socket_timeout": 5000},
            "logging": {"logs_directory": "logs/", "LogLevel": 2, "flush": 0},
            "time": {"Period_time": 30},
            "threadpool": {"maxworkingthreads": 8}
        }
    })";

	for (const auto& test : tests)
	{
		std::string modified = base_json;
		size_t pos = modified.find(test.target);
		EXPECT_NE(pos, std::string::npos) << "Target substring " << test.target << " not found!";
		if (pos != std::string::npos)
		{
			modified.replace(pos, test.target.size(), test.invalid_value);
		}
		std::string filename = CreateTempFile(modified);
		EXPECT_THROW(Config config(filename), std::runtime_error);
		DeleteTempFile(filename);
	}
}

// Test: HandlesBooleanConversion
// Ensures that numeric values given for boolean fields (e.g. blocking or flush)
// are correctly converted to their corresponding boolean values.
TEST(ConfigTest, HandlesBooleanConversion)
{
	// Tests numeric to boolean conversion logic
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "TestServer",
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "127.0.0.1"
            },
            "communicationsettings": {
                "blocking": 5,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 8
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	EXPECT_TRUE(config.GetCommunication().blocking);
	EXPECT_FALSE(config.GetLogging().flush);
}

// Test: HandlesEdgeCaseValues
// Tests configuration handling for boundary numeric values,
// such as INT_MAX for the server port and very high numbers for thread pool settings.
TEST(ConfigTest, HandlesEdgeCaseValues)
{
	// Tests boundary values for numeric fields
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "TestServer",
                "serverdisplayname": "Test Server",
                "listenerport": )"
							   + std::to_string(std::numeric_limits<int>::max()) + R"(,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 1000000
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	EXPECT_EQ(config.GetServer().port, std::numeric_limits<int>::max());
	EXPECT_EQ(config.GetThreads().max_working_threads, 1000000);
}

// Test: HandlesFileNotFound
// Checks that specifying a non-existent configuration file results in a runtime error.
TEST(ConfigTest, HandlesFileNotFound)
{
	// Tests constructor behavior with missing file
	EXPECT_THROW(Config config("nonexistent.json"), std::runtime_error);
}

// Test: ProvidesMeaningfulErrorMessages
// Verifies that, when configuration parsing fails,
// the error message contains useful information about the missing key.
TEST(ConfigTest, ProvidesMeaningfulErrorMessages)
{
	// Tests error message formatting and content
	std::string json_content = R"({ "root": { "Server": {} } })";
	std::string filename = CreateTempFile(json_content);

	try
	{
		Config config(filename);
		FAIL() << "Expected config to throw";
	}
	catch (const std::runtime_error& e)
	{
		EXPECT_NE(std::string(e.what()).find("servername"), std::string::npos);
	}
	DeleteTempFile(filename);
}

// Test: HandlesMissingServerField
// Checks that if a required Server field (e.g., "servername") is missing,
// the configuration parsing fails.
TEST(ConfigTest, HandlesMissingServerField)
{
	// Missing "servername"
	std::string json_content = R"({
        "root": {
            "Server": {
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "127.0.0.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 8
            }
        }
    })";
	std::string filename = CreateTempFile(json_content);
	EXPECT_THROW(Config config(filename), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: HandlesMissingCommunicationField
// Confirms that omitting a required communicationsettings field (e.g., "socket_timeout")
// causes configuration parsing to throw an error.
TEST(ConfigTest, HandlesMissingCommunicationField)
{
	// Missing "socket_timeout" in communicationsettings.
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "TestServer",
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "127.0.0.1"
            },
            "communicationsettings": {
                "blocking": 1
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 8
            }
        }
    })";
	std::string filename = CreateTempFile(json_content);
	EXPECT_THROW(Config config(filename), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: IgnoresExtraFields
// Verifies that extra, unrecognized fields in the configuration JSON are safely ignored,
// and do not affect the parsing of the required configuration data.
TEST(ConfigTest, IgnoresExtraFields)
{
	// The JSON has valid required keys plus several extra keys at various levels.
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "TestServer",
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "127.0.0.1",
                "extra_field": "should be ignored"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000,
                "unused": 999
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0,
                "extra": "value"
            },
            "time": {
                "Period_time": 30,
                "extra_time": 100
            },
            "threadpool": {
                "maxworkingthreads": 8,
                "extra_thread_setting": true
            },
            "unused_top_level": {"foo": "bar"}
        }
    })";
	std::string filename = CreateTempFile(json_content);
	// Should parse without throwing errors.
	EXPECT_NO_THROW(Config config(filename));
	DeleteTempFile(filename);
}

// Test: VeryLongFieldNames
// Tests that the Config class can handle unusually long field names (1000 characters),
// ensuring the parsing mechanism is robust against extreme input.
TEST(ConfigTest, VeryLongFieldNames)
{
	// Create a field name that's 1000 characters long
	std::string very_long_name(1000, 'a');

	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "value",
                "serverdisplayname": ")"
							   + very_long_name + R"(",
                "listenerport": 8080,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 10
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	// Verify the other settings are still loaded correctly
	EXPECT_EQ(config.GetServer().server_display_name, very_long_name);
	EXPECT_EQ(config.GetServer().port, 8080);
}

// Test: VeryLongFieldValues
// Tests that the Config class can handle unusually long field values (10000 characters),
// verifying there are no unexpected limitations on value length.
TEST(ConfigTest, VeryLongFieldValues)
{
	// Create a string that's 10000 characters long
	std::string very_long_value(10000, 'x');

	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": ")"
							   + very_long_value + R"(",
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 10
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	// Verify the long value is loaded correctly
	EXPECT_EQ(config.GetServer().server_name, very_long_value);
}

// Test: SpecialCharactersInPaths
// Tests that the Config class can handle paths with special characters including
// spaces, hash symbols, and ampersands without corruption or errors.
TEST(ConfigTest, SpecialCharactersInPaths)
{
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "TestServer",
                "serverdisplayname": "Test Server",
                "listenerport": 8080,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/with spaces/and#special&chars/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 10
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	// Verify the special character path is loaded correctly
	EXPECT_EQ(config.GetLogging().logs_directory, "logs/with spaces/and#special&chars/");
}

// Test: InternationalCharactersInValues
// Tests that the Config class can handle Unicode/international characters in values,
// ensuring proper support for non-ASCII text in configuration files.
TEST(ConfigTest, InternationalCharactersInValues)
{
	std::string json_content = R"({
        "root": {
            "Server": {
                "servername": "サーバー名",
                "serverdisplayname": "测试服务器",
                "listenerport": 8080,
                "ipaddress": "192.168.1.1"
            },
            "communicationsettings": {
                "blocking": 1,
                "socket_timeout": 5000
            },
            "logging": {
                "logs_directory": "logs/",
                "LogLevel": 2,
                "flush": 0
            },
            "time": {
                "Period_time": 30
            },
            "threadpool": {
                "maxworkingthreads": 10
            }
        }
    })";

	std::string filename = CreateTempFile(json_content);
	Config config(filename);
	DeleteTempFile(filename);

	// Verify the international character values are loaded correctly
	EXPECT_EQ(config.GetServer().server_name, "サーバー名");
	EXPECT_EQ(config.GetServer().server_display_name, "测试服务器");
}
