#pragma once

/**
 * @file JSONAll.hpp
 * @brief Comprehensive header for JSON serialization functionality.
 *
 * @details
 * This header serves as a convenient single include point for all JSON-related
 * functionality in the library. It includes the JSON class definition, serialization
 * templates, and macros for defining serializers for custom types.
 *
 * Usage example:
 * @code
 * #include "JSON/JSONAll.hpp"
 *
 * struct MyType
 * {
 *     std::string name;
 *     int value;
 * };
 *
 * // Define serializer
 * JSON_DEFINE_SERIALIZER(MyType, name, value)
 *
 * // Use serialization
 * MyType obj = {"test", 42};
 * JSON json = JSON::From(obj);
 * std::string serialized = json.Serialize(true);
 * @endcode
 */

// Include the core JSON headers in the correct order
#include "JSON.hpp"		  // JSON class definition
#include "Macros.hpp"	  // Macros for defining serializers
#include "Serializer.hpp" // Basic serializer template
