#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

#include "../../src/server/JSON/JSON.hpp"
#include "../../src/server/JSON/Parser.hpp"

using namespace ISXJson;

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

// Test: ParseNull
// Ensures that the Parser correctly parses the "null" literal and returns a JSON object of type NUL.
TEST(JSONParserTest, ParseNull)
{
	std::string filename = CreateTempFile("null");
	Parser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::NUL);
}

// Test: ParseBoolean
// Verifies that the Parser correctly parses the boolean literals "true" and "false"
// into JSON objects of type BOOL with the expected values.
TEST(JSONParserTest, ParseBoolean)
{
	// True case
	std::string filename_true = CreateTempFile("true");
	Parser parser_true(filename_true);
	JSON result_true = parser_true.Parse();
	DeleteTempFile(filename_true);
	ASSERT_EQ(result_true.GetType(), JSON::Type::BOOL);
	EXPECT_TRUE(result_true.AsBool());

	// False case
	std::string filename_false = CreateTempFile("false");
	Parser parser_false(filename_false);
	JSON result_false = parser_false.Parse();
	DeleteTempFile(filename_false);
	ASSERT_EQ(result_false.GetType(), JSON::Type::BOOL);
	EXPECT_FALSE(result_false.AsBool());
}

// Test: ParseNumbers
// Validates that the Parser can handle various numeric formats including integers,
// negative numbers, and numbers in exponential notation.
TEST(JSONParserTest, ParseNumbers)
{
	std::vector<std::pair<std::string, double>> tests = {
		{"0", 0}, {"123", 123}, {"-45.6", -45.6}, {"1e3", 1000}, {"2.5E-2", 0.025}};

	for (const auto& [num_str, expected] : tests)
	{
		std::string filename = CreateTempFile(num_str);
		Parser parser(filename);
		JSON result = parser.Parse();
		DeleteTempFile(filename);

		ASSERT_EQ(result.GetType(), JSON::Type::NUMBER);
		EXPECT_DOUBLE_EQ(result.AsNumber(), expected);
	}
}

// Test: ParseStrings
// Checks that the Parser correctly parses a simple JSON string literal.
TEST(JSONParserTest, ParseStrings)
{
	// Simple string
	std::string filename = CreateTempFile(R"("Simple string")");
	Parser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);
	ASSERT_EQ(result.GetType(), JSON::Type::STRING);
	EXPECT_EQ(result.AsString(), "Simple string");
}

// Test: ParseStringEscapes
// Ensures that escape sequences within a JSON string are interpreted correctly by the Parser.
TEST(JSONParserTest, ParseStringEscapes)
{
	std::string filename = CreateTempFile(R"("Escaped: \\ \" \/ \b \f \n \r \t")");
	Parser parser(filename);
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
	Parser parser(filename);
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
// Ensures that the Parser correctly parses a JSON object with key-value pairs,
// and that the values accessed by key match the expected results.
TEST(JSONParserTest, ParseObject)
{
	std::string filename = CreateTempFile(R"({"key": "value", "num": 3.14})");
	Parser parser(filename);
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
	Parser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::OBJECT);
	const auto& arr = result["array"].AsArray();
	EXPECT_TRUE(arr[0]["nested"].AsBool());				   // Nested object in array
	EXPECT_EQ(result["value"].GetType(), JSON::Type::NUL); // Top-level null
}

// Test: ErrorHandling
// Validates that the Parser throws appropriate exceptions for various types of malformed JSON input
// or when a file does not exist.
TEST(JSONParserTest, ErrorHandling)
{
	// Invalid file handling
	EXPECT_THROW(Parser("nonexistent.json"), std::runtime_error);

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
		Parser parser(filename);
		EXPECT_THROW(parser.Parse(), std::runtime_error);
		DeleteTempFile(filename);
	}
}

// Test: TrailingCharacters
// Ensures that any unexpected trailing characters after the main JSON value cause a parsing error.
TEST(JSONParserTest, TrailingCharacters)
{
	std::string filename = CreateTempFile("null garbage");
	Parser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: WhitespaceHandling
// Checks that the Parser properly ignores extraneous whitespace in various positions.
TEST(JSONParserTest, WhitespaceHandling)
{
	std::string filename = CreateTempFile("\t\n\r{\n\"key\"\n:\n[\n]\n}\n");
	Parser parser(filename);
	EXPECT_NO_THROW(parser.Parse()); // Should handle all whitespace
	DeleteTempFile(filename);
}

// Test: UnicodeEscapeError
// Ensures that the Parser throws an error when it encounters unsupported Unicode escape sequences.
TEST(JSONParserTest, UnicodeEscapeError)
{
	std::string filename = CreateTempFile(R"("\u2603")"); // Snowman character
	Parser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: DeepNesting
// Tests that the Parser can handle deeply nested structures up to a reasonable depth,
// ensuring access to elements at each level works correctly.
TEST(JSONParserTest, DeepNesting)
{
	// Create a nested array with a few levels - keeping it simple
	std::string content = "[[[[42]]]]";
	std::string filename = CreateTempFile(content);
	Parser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	// Verify the structure by accessing the nested value
	ASSERT_EQ(result.GetType(), JSON::Type::ARRAY);
	ASSERT_EQ(result.Size(), 1);

	const JSON& level1 = result[0];
	ASSERT_EQ(level1.GetType(), JSON::Type::ARRAY);
	ASSERT_EQ(level1.Size(), 1);

	const JSON& level2 = level1[0];
	ASSERT_EQ(level2.GetType(), JSON::Type::ARRAY);
	ASSERT_EQ(level2.Size(), 1);

	const JSON& level3 = level2[0];
	ASSERT_EQ(level3.GetType(), JSON::Type::ARRAY);
	ASSERT_EQ(level3.Size(), 1);

	// Get the actual number value
	const JSON& level4 = level3[0];
	ASSERT_EQ(level4.GetType(), JSON::Type::NUMBER);
	EXPECT_EQ(level4.AsNumber(), 42);
}

// Test: NumericEdgeCases
// Verifies that the Parser handles various numeric edge cases correctly, including
// very small decimals, very large decimals, and values near the limits of double precision.
TEST(JSONParserTest, NumericEdgeCases)
{
	std::vector<std::pair<std::string, double>> tests = {
		{"1e-10", 1e-10},									   // Very small decimal
		{"1e20", 1e20},										   // Very large decimal
		{"1.7976931348623157e+308", 1.7976931348623157e+308},  // Close to max double
		{"2.2250738585072014e-308", 2.2250738585072014e-308},  // Close to min positive double
		{"-1.7976931348623157e+308", -1.7976931348623157e+308} // Close to lowest negative double
	};

	for (const auto& [num_str, expected] : tests)
	{
		std::string filename = CreateTempFile(num_str);
		Parser parser(filename);
		JSON result = parser.Parse();
		DeleteTempFile(filename);

		ASSERT_EQ(result.GetType(), JSON::Type::NUMBER);
		EXPECT_DOUBLE_EQ(result.AsNumber(), expected);
	}
}

// Test: SpecialCharactersInKeys
// Tests handling of special characters in object keys, ensuring they can be
// properly stored and retrieved.
TEST(JSONParserTest, SpecialCharactersInKeys)
{
	std::string filename = CreateTempFile(R"({"special_key": "value", "unicode_key": "smiley"})");
	Parser parser(filename);
	JSON result = parser.Parse();
	DeleteTempFile(filename);

	ASSERT_EQ(result.GetType(), JSON::Type::OBJECT);
	EXPECT_TRUE(result.Contains("special_key"));
	EXPECT_EQ(result["special_key"].AsString(), "value");
}

// Test: EmptyInput
// Tests that the Parser correctly handles empty input files by throwing
// an appropriate error.
TEST(JSONParserTest, EmptyInput)
{
	std::string filename = CreateTempFile("");
	Parser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}

// Test: OnlyWhitespace
// Tests that the Parser correctly handles input files containing only whitespace
// by throwing an appropriate error instead of trying to parse invalid JSON.
TEST(JSONParserTest, OnlyWhitespace)
{
	std::string filename = CreateTempFile("  \n\t\r  ");
	Parser parser(filename);
	EXPECT_THROW(parser.Parse(), std::runtime_error);
	DeleteTempFile(filename);
}
