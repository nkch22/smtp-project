/**
 * @file Config.hpp
 * @brief Defines the Config class providing structured access to application configuration parameters.
 *
 * This file declares the Config class which loads configuration settings from a JSON file
 * and provides structured access to configuration parameters.
 *
 * @section usage_example Usage Example
 * @code
 * #include "Config/Config.hpp"
 * #include <iostream>
 *
 * // Expected JSON structure:
 * // {
 * //   "root": {
 * //     "Server": { ... },
 * //     "communicationsettings": { ... },
 * //     "logging": { ... },
 * //     "time": { ... },
 * //     "threadpool": { ... }
 * //   }
 * // }
 *
 * try
 * {
 *     // Load configuration from JSON file
 *     Config config("config.json");
 *
 *     // Access server configuration
 *     Config::Server server = config.GetServer();
 *     std::string server_namne = server.server_name;
 *     std::string display_name = server.server_display_name;
 *     int port = server.port;
 *     std::string ip_address = server.ip;
 *
 *     // Access communication settings
 *     Config::Communication comm = config.GetCommunication();
 *     bool is_blocking = comm.blocking;
 *     int socket_timeout = comm.socket_timeout;
 *
 *     // Access logging configuration
 *     Config::Logging logging = config.GetLogging();
 *     std::string logs_dir = logging.logs_directory;
 *     int log_level = logging.log_level;
 *     bool should_flush = logging.flush;
 *
 *     // Access thread settings
 *     Config::Threads threads = config.GetThreads();
 *     int period_time = threads.period_time;
 *     int max_threads = threads.max_working_threads;
 *
 *     // Use the configuration values
 *     std::cout << "Server " << server_namne << " listening on " << ip_address << ":" << port << "\n";
 *     std::cout << "Log directory: " << logs_dir << " (Level: " << log_level << ")\n";
 *     std::cout << "Using " << max_threads << " worker threads\n";
 * }
 * catch (const std::exception& ex)
 * {
 *     std::cerr << "Configuration error: " << ex.what() << std::endl;
 *     return 1;
 * }
 * @endcode
 */

#pragma once

#include <filesystem>
#include <string>

#include "../JSON/Parser.hpp"

/**
 * @class Config
 * @brief Provides structured access to application configuration parameters from JSON
 */
class Config
{
public:
	/**
	 * @struct Server
	 * @brief Contains server network configuration parameters.
	 *
	 * Expected JSON keys:
	 * - "servername": Server identifier (e.g., "ServTest")
	 * - "serverdisplayname": Server display name (e.g., "ServTestserver")
	 * - "listenerport": Listening port number (e.g., 25000)
	 * - "ipaddress": IP address to bind to (e.g., "127.0.0.1")
	 */
	struct Server
	{
		std::string server_name;		 ///< Server identifier (config key: "servername")
		std::string server_display_name; ///< Server display name (config key: "serverdisplayname")
		int port;						 ///< Listening port number (config key: "listenerport")
		std::string ip;					 ///< IP address to bind to (config key: "ipaddress")
	};

	/**
	 * @struct Communication
	 * @brief Network communication settings.
	 *
	 * Expected JSON keys:
	 * - "blocking": Blocking socket mode (0 for false, non-zero for true)
	 * - "socket_timeout": Socket timeout in milliseconds (e.g., 5)
	 */
	struct Communication
	{
		bool blocking;		///< Blocking socket mode (config key: "blocking")
		int socket_timeout; ///< Socket timeout in milliseconds (config key: "socket_timeout")
	};

	/**
	 * @struct Logging
	 * @brief Logging system configuration.
	 *
	 * Expected JSON keys:
	 * - "logs_directory": Directory path for storing log files (e.g., "logs/")
	 * - "LogLevel": Logging verbosity level (0 - No logs, 1 - Production, 2 - Debug, 3 - Trace)
	 * - "flush": Immediate flush setting (0 for false, non-zero for true)
	 */
	struct Logging
	{
		std::string logs_directory; ///< Directory path for log files (config key: "logs_directory")
		int log_level;				///< Logging verbosity level (config key: "LogLevel")
		bool flush;					///< Immediate flush setting (config key: "flush")
	};

	/**
	 * @struct Threads
	 * @brief Thread pool configuration parameters.
	 *
	 * Expected JSON keys:
	 * - "Period_time": Interval in milliseconds for task processing (e.g., 30)
	 * - "maxworkingthreads": Maximum number of concurrent threads (e.g., 10)
	 */
	struct Threads
	{
		int period_time;		 ///< Task processing interval in ms (config key: "Period_time")
		int max_working_threads; ///< Maximum number of concurrent threads (config key: "maxworkingthreads")
	};

	/**
	 * @brief Retrieve server configuration
	 * @return Copy of Server configuration struct
	 */
	Server GetServer() const;

	/**
	 * @brief Retrieve communication settings
	 * @return Copy of Communication configuration struct
	 */
	Communication GetCommunication() const;

	/**
	 * @brief Retrieve logging configuration
	 * @return Copy of Logging configuration struct
	 */
	Logging GetLogging() const;

	/**
	 * @brief Retrieve thread pool settings
	 * @return Copy of Threads configuration struct
	 */
	Threads GetThreads() const;

	/**
	 * @brief Construct Config from JSON file
	 * @param file_path Path to configuration JSON file
	 * @throw std::runtime_error On parsing errors or missing required fields
	 */
	explicit Config(const std::filesystem::path& file_path);

private:
	/// @name Configuration Parsers
	/// @{
	void ParseServer(const ISXJson::JSON& server_json);
	void ParseCommunication(const ISXJson::JSON& comm_json);
	void ParseLogging(const ISXJson::JSON& logger_json);
	void ParseTime(const ISXJson::JSON& time_json);
	void ParseThreadpool(const ISXJson::JSON& threadpool_json);
	/// @}

private:
	Server m_server;			   ///< Storage for server configuration
	Communication m_communication; ///< Storage for communication settings
	Logging m_logging;			   ///< Storage for logging configuration
	Threads m_threads;			   ///< Storage for thread pool parameters
};
