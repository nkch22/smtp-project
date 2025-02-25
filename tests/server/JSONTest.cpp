#include "../../src/server/JSON/JSON.hpp"

#include <gtest/gtest.h>
#include <ostream>
#include <sstream>

using namespace ISXJson; // Using the ISXJson namespace directly

// Test: DefaultConstructor
// Ensures that a default-constructed JSON object has type NUL.
TEST(JSONTest, DefaultConstructor)
{
	JSON json;
	EXPECT_EQ(json.GetType(), JSON::Type::NUL);
}

// Test: BooleanType
// Verifies that JSON objects constructed with true or false are correctly typed,
// and that the boolean accessor returns the expected value.
TEST(JSONTest, BooleanType)
{
	JSON json_true(true);
	ASSERT_EQ(json_true.GetType(), JSON::Type::BOOL);
	EXPECT_TRUE(json_true.AsBool());

	JSON json_false(false);
	ASSERT_EQ(json_false.GetType(), JSON::Type::BOOL);
	EXPECT_FALSE(json_false.AsBool());
}

// Test: NumberType
// Validates that a numeric JSON object stores a number correctly and that the number accessor returns the proper value.
TEST(JSONTest, NumberType)
{
	JSON json(3.14);
	ASSERT_EQ(json.GetType(), JSON::Type::NUMBER);
	EXPECT_DOUBLE_EQ(json.AsNumber(), 3.14);

	// Test integer conversion to double
	JSON json_int(42);
	ASSERT_EQ(json_int.GetType(), JSON::Type::NUMBER);
	EXPECT_DOUBLE_EQ(json_int.AsNumber(), 42.0);
}

// Test: StringType
// Confirms that JSON objects constructed from C-style strings and std::string have type STRING
// and that the stored string can be retrieved correctly.
TEST(JSONTest, StringType)
{
	JSON json_str1("test"); // const char* constructor
	ASSERT_EQ(json_str1.GetType(), JSON::Type::STRING);
	EXPECT_EQ(json_str1.AsString(), "test");

	JSON json_str2(std::string("another")); // std::string constructor
	ASSERT_EQ(json_str2.GetType(), JSON::Type::STRING);
	EXPECT_EQ(json_str2.AsString(), "another");
}

// Test: ArrayType
// Verifies that JSON arrays can be constructed both by copy and by move,
// and that the array accessor returns the correct size.
TEST(JSONTest, ArrayType)
{
	JSON::Array arr = {JSON(1.0), JSON("test")};
	JSON json_copy(arr);
	ASSERT_EQ(json_copy.GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(json_copy.AsArray().size(), 2);

	JSON::Array arr_move = {JSON(2.0), JSON("move")};
	JSON json_move(std::move(arr_move)); // Test move constructor
	ASSERT_EQ(json_move.GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(json_move.AsArray().size(), 2);
}

// Test: ObjectType
// Ensures that JSON objects can be constructed by copy or move,
// and that the object accessor provides the correct key-value pairs.
TEST(JSONTest, ObjectType)
{
	JSON::Object obj = {{"key", JSON("value")}};
	JSON json_copy(obj);
	ASSERT_EQ(json_copy.GetType(), JSON::Type::OBJECT);
	EXPECT_EQ(json_copy.AsObject().size(), 1);

	JSON::Object obj_move = {{"key2", JSON("value2")}};
	JSON json_move(std::move(obj_move)); // Test move constructor
	ASSERT_EQ(json_move.GetType(), JSON::Type::OBJECT);
	EXPECT_EQ(json_move.AsObject().size(), 1);
}

// Test: AsMethodsThrow
// Verifies that attempting to access a JSON value with an inappropriate accessor
// (for example, treating a number as a string) throws a runtime error.
TEST(JSONTest, AsMethodsThrow)
{
	JSON json_null;
	EXPECT_THROW(json_null.AsBool(), std::runtime_error); // Null -> bool

	JSON json_num(3.14);
	EXPECT_THROW(json_num.AsString(), std::runtime_error); // Number -> string

	JSON json_str("test");
	EXPECT_THROW(json_str.AsArray(), std::runtime_error); // String -> array
}

// Test: ObjectAccess
// Verifies that valid member access on JSON objects returns the expected result,
// and that accessing a non-existent key or using object access on a non-object triggers an error.
TEST(JSONTest, ObjectAccess)
{
	JSON::Object obj = {{"name", JSON("test")}, {"value", JSON(42.0)}};
	JSON json(obj);

	// Valid key access
	EXPECT_NO_THROW(json["name"]);
	EXPECT_EQ(json["name"].AsString(), "test");

	// Invalid key access
	EXPECT_THROW(json["invalid"], std::runtime_error);

	// Non-object access attempt
	JSON not_obj(3.14);
	EXPECT_THROW(not_obj["key"], std::runtime_error);
}

// Test: ArrayAccess
// Verifies array element access behavior, specifically that elements can be accessed by index,
// out-of-bounds access throws an exception, and array access on non-array types throws an exception.
TEST(JSONTest, ArrayAccess)
{
	JSON::Array arr = {JSON(1.0), JSON("test")};
	JSON json(arr);

	// Valid index access
	EXPECT_NO_THROW(json[0]);
	EXPECT_DOUBLE_EQ(json[0].AsNumber(), 1.0);
	EXPECT_EQ(json[1].AsString(), "test");

	// Invalid index access
	EXPECT_THROW(json[2], std::runtime_error);

	// Non-array access attempt
	JSON not_arr(3.14);
	EXPECT_THROW(not_arr[0], std::runtime_error);
}

// Test: SizeMethod
// Tests the Size() method behavior on arrays and objects, verifying it returns the correct
// element count and throws an exception when called on non-collection JSON types.
TEST(JSONTest, SizeMethod)
{
	// Array size
	JSON::Array arr = {JSON(1.0), JSON("test"), JSON(true)};
	JSON arr_json(arr);
	EXPECT_EQ(arr_json.Size(), 3);

	// Object size
	JSON::Object obj = {{"one", JSON(1.0)}, {"two", JSON("test")}};
	JSON obj_json(obj);
	EXPECT_EQ(obj_json.Size(), 2);

	// Size not applicable to other types
	JSON num(42.0);
	EXPECT_THROW(num.Size(), std::runtime_error);
}

// Test: ContainsMethod
// Tests the Contains() method on objects, ensuring it correctly reports existing and non-existing keys,
// and throws an exception when called on non-object JSON types.
TEST(JSONTest, ContainsMethod)
{
	JSON::Object obj = {{"name", JSON("test")}, {"value", JSON(42.0)}};
	JSON json(obj);

	EXPECT_TRUE(json.Contains("name"));
	EXPECT_TRUE(json.Contains("value"));
	EXPECT_FALSE(json.Contains("nonexistent"));

	// Contains not applicable to non-objects
	JSON arr(JSON::Array{JSON(1.0), JSON(2.0)});
	EXPECT_THROW(arr.Contains("key"), std::runtime_error);
}

// Test: Serialization
// Tests the Serialize() method to ensure correct JSON string output.
TEST(JSONTest, Serialization)
{
	// Test various JSON value types
	JSON null_val;
	EXPECT_EQ(null_val.Serialize(), "null");

	JSON bool_val(true);
	EXPECT_EQ(bool_val.Serialize(), "true");

	JSON int_val(42);
	EXPECT_EQ(int_val.Serialize(), "42");

	JSON float_val(3.14);
	EXPECT_EQ(float_val.Serialize(), "3.14");

	JSON str_val("test");
	EXPECT_EQ(str_val.Serialize(), "\"test\"");

	// Array serialization
	JSON::Array arr = {JSON(1), JSON(2), JSON("three")};
	JSON arr_val(arr);
	EXPECT_EQ(arr_val.Serialize(false), "[1,2,\"three\"]");

	// Object serialization
	JSON::Object obj = {{"name", JSON("value")}, {"num", JSON(42)}};
	JSON obj_val(obj);
	// We need to check both possible orders since object serialization order isn't guaranteed
	std::string expected1 = "{\"name\":\"value\",\"num\":42}";
	std::string expected2 = "{\"num\":42,\"name\":\"value\"}";
	std::string actual = obj_val.Serialize(false);
	EXPECT_TRUE(actual == expected1 || actual == expected2);
}

// Test: StreamOperator
// Tests the JSON stream operator (<<) outputs the correct JSON serialization for
// both simple and complex JSON values, enabling easy printing of JSON to streams.
TEST(JSONTest, StreamOperator)
{
	JSON json(42);
	std::ostringstream oss;
	oss << json;
	EXPECT_EQ(oss.str(), "42");

	oss.str("");
	JSON complex(JSON::Object{{"key", JSON("value")}});
	oss << complex;
	EXPECT_EQ(oss.str(), "{\"key\":\"value\"}");
}

// Test: EscapeString
// Tests string escaping for special characters.
TEST(JSONTest, StringEscaping)
{
	JSON json(R"(Special chars: " \ / \b \f \n \r \t)");
	std::string serialized = json.Serialize();
	EXPECT_EQ(serialized, R"("Special chars: \" \\ / \\b \\f \\n \\r \\t")");
}

// Test: PrettyPrint
// Tests the pretty-print functionality of the Serialize method, ensuring it properly
// formats JSON with newlines and indentation for improved human readability.
TEST(JSONTest, PrettyPrint)
{
	JSON::Object obj = {{"array", JSON(JSON::Array{JSON(1), JSON(2)})}, {"value", JSON("test")}};
	JSON json(obj);

	std::string pretty = json.Serialize(true);

	// Pretty print should contain newlines
	EXPECT_TRUE(pretty.find('\n') != std::string::npos);

	// Check for proper indentation
	EXPECT_TRUE(pretty.find("  ") != std::string::npos);
}

// Test: ConversionOperators
// Tests the explicit conversion operators to bool, double, and string,
// verifying they return the underlying values with the correct types.
TEST(JSONTest, ConversionOperators)
{
	JSON bool_val(true);
	EXPECT_TRUE(static_cast<bool>(bool_val));

	JSON num_val(3.14);
	EXPECT_DOUBLE_EQ(static_cast<double>(num_val), 3.14);

	JSON str_val("test");
	EXPECT_EQ(static_cast<std::string>(str_val), "test");
}

// Test: GetTemplateMethod
// Tests the templated Get<T> method, which provides type-safe access to the underlying variant,
// and verifies that it throws exceptions when an incorrect type is requested.
TEST(JSONTest, GetTemplateMethod)
{
	JSON bool_val(true);
	EXPECT_TRUE(bool_val.Get<bool>());

	JSON num_val(3.14);
	EXPECT_DOUBLE_EQ(num_val.Get<double>(), 3.14);

	JSON str_val("test");
	EXPECT_EQ(str_val.Get<std::string>(), "test");

	// Test error cases
	EXPECT_THROW(bool_val.Get<double>(), std::runtime_error);
}

// Test: SerializationMethods
// Tests the From<T> and To<T> methods for serialization and deserialization,
// ensuring proper bidirectional conversion between C++ types and JSON representations.
TEST(JSONTest, SerializationMethods)
{
	// Basic type serialization
	JSON bool_json = JSON::From<bool>(true);
	EXPECT_TRUE(bool_json.AsBool());
	EXPECT_TRUE(bool_json.To<bool>());

	JSON num_json = JSON::From<int>(42);
	EXPECT_EQ(num_json.AsNumber(), 42);
	EXPECT_EQ(num_json.To<int>(), 42);

	JSON str_json = JSON::From<std::string>("test");
	EXPECT_EQ(str_json.AsString(), "test");
	EXPECT_EQ(str_json.To<std::string>(), "test");
}

// Test: GetValueMethod
// Tests the GetValue method for accessing the underlying variant storage directly,
// which can be useful for advanced manipulation of the JSON value.
TEST(JSONTest, GetValueMethod)
{
	JSON json(42);
	const JSON::Value& value = json.GetValue();
	EXPECT_EQ(std::get<double>(value), 42);
}

// Test: EdgeCases
// Tests various edge cases like zero values, empty collections, and integer-like
// floating point values to ensure the JSON class handles these cases appropriately.
TEST(JSONTest, EdgeCases)
{
	// Zero values
	JSON zero_num(0);
	EXPECT_EQ(zero_num.AsNumber(), 0);
	EXPECT_EQ(zero_num.Serialize(), "0");

	// Empty collections
	JSON empty_array(JSON::Array{});
	EXPECT_EQ(empty_array.Size(), 0);
	EXPECT_EQ(empty_array.Serialize(), "[]");

	JSON empty_object(JSON::Object{});
	EXPECT_EQ(empty_object.Size(), 0);
	EXPECT_EQ(empty_object.Serialize(), "{}");

	// Integer vs. double distinctions
	double int_value = 42.0;
	JSON int_like_double(int_value);
	EXPECT_EQ(int_like_double.Serialize(), "42");
}

// Test: ZeroValues
// Tests that zero numeric values are correctly handled and serialized.
TEST(JSONTest, ZeroValues)
{
	JSON zero_num(0);
	EXPECT_EQ(zero_num.AsNumber(), 0);
	EXPECT_EQ(zero_num.Serialize(), "0");

	// Also test negative zero for floating point
	JSON neg_zero(-0.0);
	EXPECT_EQ(neg_zero.AsNumber(), 0.0);
	EXPECT_EQ(neg_zero.Serialize(), "0");
}

// Test: EmptyCollections
// Tests that empty arrays and objects are correctly handled and serialized.
TEST(JSONTest, EmptyCollections)
{
	JSON empty_array(JSON::Array{});
	EXPECT_EQ(empty_array.Size(), 0);
	EXPECT_EQ(empty_array.Serialize(), "[]");

	JSON empty_object(JSON::Object{});
	EXPECT_EQ(empty_object.Size(), 0);
	EXPECT_EQ(empty_object.Serialize(), "{}");

	// Test empty array operations
	EXPECT_THROW(empty_array[0], std::runtime_error); // Should throw on access to empty array

	// Test empty object operations
	EXPECT_FALSE(empty_object.Contains("key"));
}

// Test: NumericFormatting
// Tests that numeric values are formatted correctly during serialization,
// particularly that integer-like floating point values are serialized without decimal portions.
TEST(JSONTest, NumericFormatting)
{
	// Integer vs. double distinctions
	double int_value = 42.0;
	JSON int_like_double(int_value);
	EXPECT_EQ(int_like_double.Serialize(), "42");

	// Explicit double with decimal
	JSON explicit_double(42.5);
	EXPECT_EQ(explicit_double.Serialize(), "42.5");

	// Very large integer-like double
	JSON large_int_like(1000000.0);
	EXPECT_EQ(large_int_like.Serialize(), "1000000");
}

// Test: MutableObjectAccess
// Tests the behavior of the non-const operator[] for objects, verifying that
// existing keys can be modified and accessing non-existent keys throws an exception.
TEST(JSONTest, MutableObjectAccess)
{
	JSON::Object obj = {{"key", JSON("original")}};
	JSON json(obj);

	// Modify an existing key
	json["key"] = JSON("modified");
	EXPECT_EQ(json["key"].AsString(), "modified");

	// Attempt to access a non-existent key (should throw)
	EXPECT_THROW({ json["nonexistent"] = JSON(true); }, std::runtime_error);
}

// Test: MutableArrayAccess
// Tests the behavior of the non-const operator[] for arrays, ensuring that
// existing elements can be modified and out-of-bounds access throws an exception.
TEST(JSONTest, MutableArrayAccess)
{
	JSON::Array arr = {JSON(1.0), JSON("test")};
	JSON json(arr);

	// Modify existing elements
	json[0] = JSON(99.9);
	json[1] = JSON("modified");

	EXPECT_DOUBLE_EQ(json[0].AsNumber(), 99.9);
	EXPECT_EQ(json[1].AsString(), "modified");

	// Attempt to access out of bounds (should throw)
	EXPECT_THROW({ json[2] = JSON(true); }, std::runtime_error);
}

// Test: ComplexNestedStructures
// Tests serialization and access of complex nested JSON structures, verifying deep property
// access works correctly and that serialization preserves the structure's hierarchy.
TEST(JSONTest, ComplexNestedStructures)
{
	// Create a complex nested structure: object containing array containing objects
	JSON::Object innerObj1 = {{"name", JSON("first")}, {"value", JSON(1)}};
	JSON::Object innerObj2 = {{"name", JSON("second")}, {"value", JSON(2)}};

	JSON::Array innerArr = {JSON(innerObj1), JSON(innerObj2)};

	JSON::Object outerObj = {{"data", JSON(innerArr)},
							 {"metadata", JSON(JSON::Object{{"count", JSON(2)}, {"active", JSON(true)}})}};

	JSON json(outerObj);

	// Test deep access
	EXPECT_EQ(json["data"][0]["name"].AsString(), "first");
	EXPECT_EQ(json["data"][1]["value"].AsNumber(), 2);
	EXPECT_EQ(json["metadata"]["count"].AsNumber(), 2);
	EXPECT_TRUE(json["metadata"]["active"].AsBool());

	// Test serialization and roundtrip
	std::string serialized = json.Serialize(false);

	// Pass the serialized string to a parser and verify it's parsed correctly
	// Since we can't directly use Parser here, we'll just check the serialized string
	// contains expected patterns
	EXPECT_TRUE(serialized.find("\"data\":[") != std::string::npos);
	EXPECT_TRUE(serialized.find("\"name\":\"first\"") != std::string::npos);
	EXPECT_TRUE(serialized.find("\"value\":2") != std::string::npos);
	EXPECT_TRUE(serialized.find("\"active\":true") != std::string::npos);
}

// Test: UnicodeCharacterHandling
// Tests handling of Unicode characters in JSON strings, including basic ASCII,
// extended ASCII with special characters, and Unicode symbols, ensuring they're
// preserved during serialization and deserialization.
TEST(JSONTest, UnicodeCharacterHandling)
{
	// Basic ASCII characters
	JSON ascii_json(std::string("Hello, world!"));
	EXPECT_EQ(ascii_json.AsString(), "Hello, world!");

	// Extended ASCII and special characters
	std::string special_chars = "Special: © ® ™ € £ ¥";
	JSON special_json(special_chars);
	EXPECT_EQ(special_json.AsString(), special_chars);

	// Emoji and other Unicode characters (if supported by the platform)
	std::string unicode_str = "Unicode: ❤️ 😊 🌍 星 月";
	JSON unicode_json(unicode_str);
	EXPECT_EQ(unicode_json.AsString(), unicode_str);

	// Test serialization and escaping
	std::string serialized = unicode_json.Serialize();
	// The serialized form should contain the escaped Unicode characters
	// The actual escaping depends on implementation details
	EXPECT_TRUE(serialized.size() > unicode_str.size()); // Serialized should be longer due to quotes and escapes
}

// Test: LargeJSONStructures
// Tests handling of large JSON structures (arrays and objects with 1000 elements)
// to ensure performance and memory handling for significant data volumes.
TEST(JSONTest, LargeJSONStructures)
{
	// Create a large array with 1000 elements
	JSON::Array large_array;
	large_array.reserve(1000);

	for (int i = 0; i < 1000; i++)
	{
		large_array.push_back(JSON(i));
	}

	JSON json(large_array);

	// Verify size
	EXPECT_EQ(json.Size(), 1000);

	// Verify first, middle, and last elements
	EXPECT_EQ(json[0].AsNumber(), 0);
	EXPECT_EQ(json[500].AsNumber(), 500);
	EXPECT_EQ(json[999].AsNumber(), 999);

	// Test serialization (we don't need to check the entire string)
	std::string serialized = json.Serialize(false);
	EXPECT_TRUE(serialized.size() > 2000); // At least 2000 chars (numbers + commas + brackets)
	EXPECT_EQ(serialized[0], '[');
	EXPECT_EQ(serialized[serialized.size() - 1], ']');

	// Create a large object with 1000 key-value pairs
	JSON::Object large_object;
	for (int i = 0; i < 1000; i++)
	{
		large_object["key" + std::to_string(i)] = JSON(i);
	}

	JSON obj_json(large_object);

	// Verify size
	EXPECT_EQ(obj_json.Size(), 1000);

	// Verify some elements
	EXPECT_EQ(obj_json["key0"].AsNumber(), 0);
	EXPECT_EQ(obj_json["key500"].AsNumber(), 500);
	EXPECT_EQ(obj_json["key999"].AsNumber(), 999);
}

// Test: AdditionalErrorConditions
// Tests comprehensive error conditions by attempting to access each JSON type
// using all of the wrong accessors, verifying that appropriate exceptions are thrown.
TEST(JSONTest, AdditionalErrorConditions)
{
	// Test all accessor methods with all wrong types
	JSON null_json;
	JSON bool_json(true);
	JSON num_json(42.0);
	JSON str_json("test");
	JSON arr_json(JSON::Array{});
	JSON obj_json(JSON::Object{});

	// Test AsBool with all wrong types
	EXPECT_THROW(null_json.AsBool(), std::runtime_error);
	EXPECT_THROW(num_json.AsBool(), std::runtime_error);
	EXPECT_THROW(str_json.AsBool(), std::runtime_error);
	EXPECT_THROW(arr_json.AsBool(), std::runtime_error);
	EXPECT_THROW(obj_json.AsBool(), std::runtime_error);

	// Test AsNumber with all wrong types
	EXPECT_THROW(null_json.AsNumber(), std::runtime_error);
	EXPECT_THROW(bool_json.AsNumber(), std::runtime_error);
	EXPECT_THROW(str_json.AsNumber(), std::runtime_error);
	EXPECT_THROW(arr_json.AsNumber(), std::runtime_error);
	EXPECT_THROW(obj_json.AsNumber(), std::runtime_error);

	// Test AsString with all wrong types
	EXPECT_THROW(null_json.AsString(), std::runtime_error);
	EXPECT_THROW(bool_json.AsString(), std::runtime_error);
	EXPECT_THROW(num_json.AsString(), std::runtime_error);
	EXPECT_THROW(arr_json.AsString(), std::runtime_error);
	EXPECT_THROW(obj_json.AsString(), std::runtime_error);

	// Test AsArray with all wrong types
	EXPECT_THROW(null_json.AsArray(), std::runtime_error);
	EXPECT_THROW(bool_json.AsArray(), std::runtime_error);
	EXPECT_THROW(num_json.AsArray(), std::runtime_error);
	EXPECT_THROW(str_json.AsArray(), std::runtime_error);
	EXPECT_THROW(obj_json.AsArray(), std::runtime_error);

	// Test AsObject with all wrong types
	EXPECT_THROW(null_json.AsObject(), std::runtime_error);
	EXPECT_THROW(bool_json.AsObject(), std::runtime_error);
	EXPECT_THROW(num_json.AsObject(), std::runtime_error);
	EXPECT_THROW(str_json.AsObject(), std::runtime_error);
	EXPECT_THROW(arr_json.AsObject(), std::runtime_error);

	// Test Size with inappropriate types
	EXPECT_THROW(null_json.Size(), std::runtime_error);
	EXPECT_THROW(bool_json.Size(), std::runtime_error);
	EXPECT_THROW(num_json.Size(), std::runtime_error);
	EXPECT_THROW(str_json.Size(), std::runtime_error);

	// Test Contains with inappropriate types
	EXPECT_THROW(null_json.Contains("key"), std::runtime_error);
	EXPECT_THROW(bool_json.Contains("key"), std::runtime_error);
	EXPECT_THROW(num_json.Contains("key"), std::runtime_error);
	EXPECT_THROW(str_json.Contains("key"), std::runtime_error);
	EXPECT_THROW(arr_json.Contains("key"), std::runtime_error);
}

// Test: TemplatedToAndFromRoundTrip
// Tests the template To<T> and From<T> methods for roundtrip conversions between
// C++ types and JSON, ensuring values are preserved and incompatible conversions throw.
TEST(JSONTest, TemplatedToAndFromRoundTrip)
{
	// Test with various types
	bool bool_val = true;
	double num_val = 3.14159;
	int int_val = 42;
	std::string str_val = "Hello, world!";

	// Test roundtrip: bool -> JSON -> bool
	JSON bool_json = JSON::From(bool_val);
	EXPECT_EQ(bool_json.GetType(), JSON::Type::BOOL);
	EXPECT_EQ(bool_json.To<bool>(), bool_val);

	// Test roundtrip: double -> JSON -> double
	JSON double_json = JSON::From(num_val);
	EXPECT_EQ(double_json.GetType(), JSON::Type::NUMBER);
	EXPECT_DOUBLE_EQ(double_json.To<double>(), num_val);

	// Test roundtrip: int -> JSON -> int
	JSON int_json = JSON::From(int_val);
	EXPECT_EQ(int_json.GetType(), JSON::Type::NUMBER);
	EXPECT_EQ(int_json.To<int>(), int_val);

	// Test roundtrip: string -> JSON -> string
	JSON str_json = JSON::From(str_val);
	EXPECT_EQ(str_json.GetType(), JSON::Type::STRING);
	EXPECT_EQ(str_json.To<std::string>(), str_val);

	// Test with incompatible types (should throw)
	EXPECT_THROW(bool_json.To<double>(), std::runtime_error);
	EXPECT_THROW(double_json.To<std::string>(), std::runtime_error);
	EXPECT_THROW(str_json.To<bool>(), std::runtime_error);
}
