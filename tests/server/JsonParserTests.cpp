#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

#include "../../src/server/JsonParser/JsonParser.hpp"

namespace
{
// Helper function to create temporary JSON file (with internal linkage)
std::string CreateTempFile(const std::string& content)
{
	static int counter = 0;
	std::string filename = "test_temp_" + std::to_string(counter++) + ".json";
	std::ofstream file(filename);
	if (!file) throw std::runtime_error("Failed to create temp file");
	file << content;
	file.close();
	return filename;
}

// Helper function to delete temporary file
void DeleteTempFile(const std::string& filename)
{
	std::remove(filename.c_str());
}
} // namespace

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

	JSON json_move(std::move(arr)); // Test move constructor
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

	JSON json_move(std::move(obj)); // Test move constructor
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

// Test: ParseNull
// Ensures that the JSONParser correctly parses the "null" literal and returns a JSON object of type NUL.
TEST(JSONParserTest, ParseNull)
{
	std::string filename = CreateTempFile("null");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::NUL);
}

// Test: ParseBoolean
// Verifies that the JSONParser correctly parses the boolean literals "true" and "false"
// into JSON objects of type BOOL with the expected values.
TEST(JSONParserTest, ParseBoolean)
{
	// True case
	std::string filename_true = CreateTempFile("true");
	JSONParser parser_true(filename_true);
	JSON result_true = parser_true.Parse();
	DeleteTempFile(filename_true);
	ASSERT_EQ(result_true.GetType(), JSON::Type::BOOL);
	EXPECT_TRUE(result_true.AsBool());

	// False case
	std::string filename_false = CreateTempFile("false");
	JSONParser parser_false(filename_false);
	JSON result_false = parser_false.Parse();
	DeleteTempFile(filename_false);
	ASSERT_EQ(result_false.GetType(), JSON::Type::BOOL);
	EXPECT_FALSE(result_false.AsBool());
}

// Test: ParseNumbers
// Validates that the JSONParser can handle various numeric formats including integers,
// negative numbers, and numbers in exponential notation.
TEST(JSONParserTest, ParseNumbers)
{
	std::vector<std::pair<std::string, double>> tests = {
		{"0", 0}, {"123", 123}, {"-45.6", -45.6}, {"1e3", 1000}, {"2.5E-2", 0.025}};

	for (const auto& [num_str, expected] : tests)
	{
		std::string filename = CreateTempFile(num_str);
		JSONParser parser(filename);
		JSON result = parser.Parse();
		DeleteTempFile(filename);

		ASSERT_EQ(result.GetType(), JSON::Type::NUMBER);
		EXPECT_DOUBLE_EQ(result.AsNumber(), expected);
	}
}

// Test: ParseStrings
// Checks that the JSONParser correctly parses a simple JSON string literal.
TEST(JSONParserTest, ParseStrings)
{
	// Simple string
	std::string filename = CreateTempFile(R"("Simple string")");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);
	ASSERT_EQ(result.GetType(), JSON::Type::STRING);
	EXPECT_EQ(result.AsString(), "Simple string");
}

// Test: ParseStringEscapes
// Ensures that escape sequences within a JSON string are interpreted correctly by the JSONParser.
TEST(JSONParserTest, ParseStringEscapes)
{
	std::string filename = CreateTempFile(R"("Escaped: \\ \" \/ \b \f \n \r \t")");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	std::string expected = "Escaped: \\ \" / \b \f \n \r \t";
	ASSERT_EQ(result.GetType(), JSON::Type::STRING);
	EXPECT_EQ(result.AsString(), expected);
}

// Test: ParseArray
// Verifies that a JSON array containing mixed types (null, boolean, number, string)
// is parsed correctly into a JSON array with the appropriate element types.
TEST(JSONParserTest, ParseArray)
{
	std::string filename = CreateTempFile(R"([null, true, 42, "test"])");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::ARRAY);
	const auto& arr = result.AsArray();
	ASSERT_EQ(arr.size(), 4);
	EXPECT_EQ(arr[0].GetType(), JSON::Type::NUL); // Null
	EXPECT_TRUE(arr[1].AsBool());				  // Boolean
	EXPECT_EQ(arr[2].AsNumber(), 42);			  // Number
	EXPECT_EQ(arr[3].AsString(), "test");		  // String
}

// Test: ParseObject
// Ensures that the JSONParser correctly parses a JSON object with key-value pairs,
// and that the values accessed by key match the expected results.
TEST(JSONParserTest, ParseObject)
{
	std::string filename = CreateTempFile(R"({"key": "value", "num": 3.14})");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::OBJECT);
	const auto& obj = result.AsObject();
	EXPECT_EQ(obj.at("key").AsString(), "value");	  // String value
	EXPECT_DOUBLE_EQ(obj.at("num").AsNumber(), 3.14); // Numeric value
}

// Test: ParseNested
// Tests that nested JSON structures (such as an object inside an array inside an object)
// are parsed correctly.
TEST(JSONParserTest, ParseNested)
{
	std::string filename = CreateTempFile(R"({"array": [{"nested": true}], "value": null})");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::OBJECT);
	const auto& arr = result["array"].AsArray();
	EXPECT_TRUE(arr[0]["nested"].AsBool());				   // Nested object in array
	EXPECT_EQ(result["value"].GetType(), JSON::Type::NUL); // Top-level null
}

// Test: ErrorHandling
// Validates that the JSONParser throws appropriate exceptions for various types of malformed JSON input
// or when a file does not exist.
TEST(JSONParserTest, ErrorHandling)
{
	// Invalid file handling
	EXPECT_THROW(JSONParser("nonexistent.json"), std::runtime_error);

	// Malformed JSON cases
	std::vector<std::string> bad_jsons = {
		"{",				  // Unterminated object
		"]",				  // Unexpected array close
		"nul",				  // Invalid null literal
		"tru",				  // Invalid true literal
		R"({"key": )",		  // Unterminated object
		R"("invalid\escape")" // Invalid escape
	};

	for (const auto& content : bad_jsons)
	{
		std::string filename = CreateTempFile(content);
		JSONParser parser(filename);
		EXPECT_THROW(parser.Parse(), std::runtime_error);
		DeleteTempFile(filename);
	}
}

// Test: TrailingCharacters
// Ensures that any unexpected trailing characters after the main JSON value cause a parsing error.
TEST(JSONParserTest, TrailingCharacters)
{
	std::string filename = CreateTempFile("null garbage");
	JSONParser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: WhitespaceHandling
// Checks that the JSONParser properly ignores extraneous whitespace in various positions.
TEST(JSONParserTest, WhitespaceHandling)
{
	std::string filename = CreateTempFile("\t\n\r{\n\"key\"\n:\n[\n]\n}\n");
	JSONParser parser(filename);
	EXPECT_NO_THROW(parser.Parse()); // Should handle all whitespace
	DeleteTempFile(filename);
}

// Test: UnicodeEscapeError
// Ensures that the JSONParser throws an error when it encounters unsupported Unicode escape sequences.
TEST(JSONParserTest, UnicodeEscapeError)
{
	std::string filename = CreateTempFile(R"("\u2603")"); // Snowman character
	JSONParser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}
