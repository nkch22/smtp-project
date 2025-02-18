#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

#include "../../src/server/JsonParser/JsonParser.hpp"

// Helper function to create temporary JSON file
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

// Helper to delete temporary file
void DeleteTempFile(const std::string& filename)
{
	std::remove(filename.c_str());
}

TEST(JSONTest, DefaultConstructor)
{
	JSON json;
	EXPECT_EQ(json.GetType(), JSON::Type::NUL);
}

// Tests boolean constructor and value access
TEST(JSONTest, BooleanType)
{
	JSON json_true(true);
	ASSERT_EQ(json_true.GetType(), JSON::Type::BOOL);
	EXPECT_TRUE(json_true.AsBool());

	JSON json_false(false);
	ASSERT_EQ(json_false.GetType(), JSON::Type::BOOL);
	EXPECT_FALSE(json_false.AsBool());
}

// Validates numeric storage and retrieval
TEST(JSONTest, NumberType)
{
	JSON json(3.14);
	ASSERT_EQ(json.GetType(), JSON::Type::NUMBER);
	EXPECT_DOUBLE_EQ(json.AsNumber(), 3.14);
}

// Checks string construction from different sources
TEST(JSONTest, StringType)
{
	JSON json_str1("test"); // const char* constructor
	ASSERT_EQ(json_str1.GetType(), JSON::Type::STRING);
	EXPECT_EQ(json_str1.AsString(), "test");

	JSON json_str2(std::string("another")); // std::string constructor
	ASSERT_EQ(json_str2.GetType(), JSON::Type::STRING);
	EXPECT_EQ(json_str2.AsString(), "another");
}

// Verifies array construction (copy and move semantics)
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

// Verifies object construction (copy and move semantics)
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

// Ensures type-checking works in accessor methods
TEST(JSONTest, AsMethodsThrow)
{
	JSON json_null;
	EXPECT_THROW(json_null.AsBool(), std::runtime_error); // Null -> bool

	JSON json_num(3.14);
	EXPECT_THROW(json_num.AsString(), std::runtime_error); // Number -> string

	JSON json_str("test");
	EXPECT_THROW(json_str.AsArray(), std::runtime_error); // String -> array
}

// Tests object member access and error conditions
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

// Tests parsing of null literal
TEST(JSONParserTest, ParseNull)
{
	std::string filename = CreateTempFile("null");
	JSONParser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::NUL);
}

// Verifies parsing of true/false literals
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

// Validates various number formats (integers, negatives, exponents)
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

// Tests basic string parsing and escape sequences
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

// Verifies handling of JSON escape sequences
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

// Tests array parsing with mixed value types
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

// Verifies object parsing with key-value pairs
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

// Tests nested structures (object in array in object)
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

// Validates error conditions and exception throwing
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

// Ensures no trailing characters after JSON root
TEST(JSONParserTest, TrailingCharacters)
{
	std::string filename = CreateTempFile("null garbage");
	JSONParser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}

// Verifies whitespace handling in various positions
TEST(JSONParserTest, WhitespaceHandling)
{
	std::string filename = CreateTempFile("\t\n\r{\n\"key\"\n:\n[\n]\n}\n");
	JSONParser parser(filename);
	EXPECT_NO_THROW(parser.Parse()); // Should handle all whitespace
	DeleteTempFile(filename);
}

// Tests unsupported unicode escape handling
TEST(JSONParserTest, UnicodeEscapeError)
{
	std::string filename = CreateTempFile(R"("\u2603")"); // Snowman character
	JSONParser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}
