/**
 * @file JSONMacros.hpp
 * @brief Macros for defining JSON serializer specializations.
 *
 * @details
 * This file declares macros that generate specializations of the Serializer template.
 * It enables automatic generation of both serialization and deserialization routines for
 * custom C++ types. Higher-level macros—such as those that automatically add and extract fields—
 * rely on variadic argument handling provided by the utilities in PreprocessorUtilities.hpp.
 *
 * The macros here allow you to simply list the fields of your type and have the corresponding
 * JSON conversion code generated automatically. This helps reduce code repetition and
 * minimizes the potential for errors.
 */

#pragma once
#include "Convert.hpp"
#include "PreprocessorUtilities.hpp"

/**
 * @brief Macro to add a field to a JSON object.
 *
 * @details
 * This macro expands to an expression that sets a key in a JSON object using the field name
 * (converted to a string) as the key and the corresponding member of the C++ object as the value.
 *
 * @param field The name of the field.
 */
#define JSON_ADD_FIELD(field) obj[#field] = value.field;

/**
 * @brief Macro to extract a field from a JSON object.
 *
 * @details
 * This macro expands to an expression that extracts a value from a JSON object for a specific key.
 * It then assigns it to the corresponding member of the C++ object after converting it with the
 * Convert function. This ensures that the JSON value is transformed to the correct type.
 *
 * @param field The name of the field.
 */
#define JSON_GET_FIELD(field) value.field = ISXJson::Convert<decltype(value.field)>(json[#field]);

/**
 * @brief Applies an action macro to each variadic argument.
 *
 * @details
 * This macro counts the number of variadic arguments using PP_NARG (defined in PreprocessorUtilities.hpp)
 * and dispatches to the appropriate FOR_EACH_n macro (for example, FOR_EACH_3 for three arguments).
 * This technique is used because the C/C++ preprocessor does not have a built-in way to iterate over
 * a variable number of arguments.
 *
 * @param action The macro to be applied to each argument.
 * @param ... The variadic arguments.
 */
#define FOR_EACH(action, ...) PP_IDENTITY(PP_CONCAT(FOR_EACH_, PP_NARG(__VA_ARGS__))(action, __VA_ARGS__))

// Definitions for applying the action macro to 1 through 10 arguments.
// These definitions can be extended if more arguments are needed.
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

// Shorthands for expanding fields in serialization and deserialization.
/**
 * @brief Expands the action for serializing fields.
 *
 * @details
 * Applies the JSON_ADD_FIELD macro to each field in the variadic list.
 */
#define JSON_EXPAND_FIELDS(...) FOR_EACH(JSON_ADD_FIELD, __VA_ARGS__)

/**
 * @brief Expands the action for deserializing fields.
 *
 * @details
 * Applies the JSON_GET_FIELD macro to each field in the variadic list.
 */
#define JSON_EXPAND_DESERIALIZE(...) FOR_EACH(JSON_GET_FIELD, __VA_ARGS__)

/**
 * @brief Generates a Serializer specialization for a given type.
 *
 * @details
 * This macro generates a specialization of the Serializer template for the provided type.
 * It defines two static member functions:
 * - Serialize: Converts an instance of the type into a JSON::Object by applying
 *   JSON_EXPAND_FIELDS to the provided list of member fields.
 * - Deserialize: Reconstructs an instance of the type from a JSON::Object by applying
 *   JSON_EXPAND_DESERIALIZE.
 *
 * This makes it extremely easy to add JSON serialization support for a new type—simply list the fields.
 *
 * @tparam Type The C++ type being serialized/deserialized.
 * @param ... The member fields to be processed.
 */
#define JSON_DEFINE_SERIALIZER(Type, ...)                                                                              \
	namespace ISXJson                                                                                                  \
	{                                                                                                                  \
	template<>                                                                                                         \
	struct Serializer<Type>                                                                                            \
	{                                                                                                                  \
		static JSON Serialize(const Type& value)                                                                       \
		{                                                                                                              \
			JSON::Object obj;                                                                                          \
			__VA_OPT__(JSON_EXPAND_FIELDS(__VA_ARGS__))                                                                \
			return obj;                                                                                                \
		}                                                                                                              \
		static Type Deserialize(const JSON& json)                                                                      \
		{                                                                                                              \
			Type value;                                                                                                \
			__VA_OPT__(JSON_EXPAND_DESERIALIZE(__VA_ARGS__))                                                           \
			return value;                                                                                              \
		}                                                                                                              \
	};                                                                                                                 \
	}
