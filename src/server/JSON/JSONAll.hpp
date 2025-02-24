#pragma once

/**
 * @file JSONAll.hpp
 * @brief Comprehensive header for JSON serialization and parsing functionality.
 *
 * @details
 * This header serves as a convenient single include point for all JSON-related
 * functionality in the library. It includes the JSON class definition, serialization
 * templates, macros for defining serializers for custom types, and the Parser class
 * for reading and parsing JSON files.
 *
 * @section serialization_example Serialization Usage Example
 * @code
 * #include "JSON/JSONAll.hpp"
 *
 * struct MyType
 * {
 *     std::string name;
 *     int value;
 *     std::vector<double> data;
 * };
 *
 * // Define serializer
 * JSON_DEFINE_SERIALIZER(MyType, name, value, data)
 *
 * // Use serialization
 * MyType obj = {"test", 42, {1.1, 2.2, 3.3}};
 * JSON json = JSON::From(obj);
 * std::string serialized = json.Serialize(true);
 * std::cout << serialized << std::endl;
 *
 * // Deserialize back to object
 * MyType reconstructed = json.To<MyType>();
 * @endcode
 *
 * @section parsing_example JSON Parsing Example
 * @code
 * #include "JSON/JSONAll.hpp"
 * #include <iostream>
 *
 * // Example config.json content:
 * // {
 * //   "root": {
 * //     "Server": {
 * //       "servername": "ServTest",
 * //       "serverdisplayname": "ServTestserver",
 * //       "listenerport": 25000,
 * //       "ipaddress": "127.0.0.1"
 * //     }
 * //   }
 * // }
 *
 * try
 * {
 *     // Parse JSON from a file
 *     ISXJson::Parser parser("config.json");
 *     ISXJson::JSON config = parser.Parse();
 *
 *     // Print the entire config
 *     std::cout << "Full config: " << config.Serialize(true) << std::endl;
 *
 *     // Access nested Server object (two levels deep)
 *     ISXJson::JSON server_obj = config["root"]["Server"];
 *
 *     // Extract server properties
 *     std::string server_name = server_obj["servername"].AsString();
 *     std::string display_name = server_obj["serverdisplayname"].AsString();
 *     int port = server_obj["listenerport"].AsNumber();
 *     std::string ipAddress = server_obj["ipaddress"].AsString();
 *
 * }
 * catch (const std::exception& ex)
 * {
 *     std::cerr << "Error: " << ex.what() << std::endl;
 * }
 * @endcode
 */

// Include the core JSON headers in the correct order
#include "JSON.hpp"		  // JSON class definition
#include "Macros.hpp"	  // Macros for defining serializers
#include "Parser.hpp"	  // Parser for reading JSON files
#include "Serializer.hpp" // Basic serializer template

// Note: The order of includes is important to avoid circular dependencies.
// Serializer.hpp defines the template, JSON.hpp includes its implementation,
// and SerializerVectorImpl.hpp is included by JSON.hpp after the JSON class
// is fully defined.
