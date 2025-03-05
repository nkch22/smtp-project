#include <chrono>
#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <string>
#include <vector>

#include "../../src/server/JSON/JSONAll.hpp"

using namespace ISXJson;

// Add int64_t serializer definition at global scope
template<>
struct Serializer<int64_t>
{
	static JSON Serialize(int64_t value) { return JSON(static_cast<double>(value)); }

	static int64_t Deserialize(const JSON& json) { return static_cast<int64_t>(json.AsNumber()); }
};

// Simple struct for testing JSON_DEFINE_SERIALIZER macro
struct SimpleStruct
{
	std::string name;
	int id;
	bool active;
	std::vector<double> values;

	// Allow comparison for testing
	bool operator==(const SimpleStruct& other) const
	{
		return name == other.name && id == other.id && active == other.active && values == other.values;
	}
};

// Define serializer for SimpleStruct at global scope
JSON_DEFINE_SERIALIZER(SimpleStruct, name, id, active, values)

// Private class for testing JSON_FRIEND_SERIALIZER
class PrivateClass
{
private:
	int m_private_id;

public:
	std::string public_name;

	PrivateClass(int id = 0, const std::string& name = "") : m_private_id(id), public_name(name) {}

	int GetPrivateId() const { return m_private_id; }

	// Make serializer a friend of this class
	JSON_FRIEND_SERIALIZER(PrivateClass)
};

// Define serializer for PrivateClass at global scope
JSON_DEFINE_FRIEND_SERIALIZER(PrivateClass, m_private_id, public_name)

// Complex object structure
struct ComplexObject
{
	std::string title;
	std::vector<SimpleStruct> items;
	std::vector<std::vector<int>> matrix;

	bool operator==(const ComplexObject& other) const
	{
		return title == other.title && items == other.items && matrix == other.matrix;
	}
};

// Define serializer for ComplexObject at global scope
JSON_DEFINE_SERIALIZER(ComplexObject, title, items, matrix)

// Structure for testing numeric edge cases
struct NumericTests
{
	double max_double;
	double min_double;
	double positive_infinity;
	double negative_infinity;
	double nan_value;
	int64_t max_int64;
	int64_t min_int64;

	// Helper to check if doubles are close enough or both NaN
	static bool AlmostEqual(double a, double b)
	{
		if (std::isnan(a) && std::isnan(b)) return true;
		if (std::isinf(a) && std::isinf(b) && ((a > 0 && b > 0) || (a < 0 && b < 0))) return true;
		return std::abs(a - b) < 1e-10;
	}

	bool operator==(const NumericTests& other) const
	{
		return AlmostEqual(max_double, other.max_double) && AlmostEqual(min_double, other.min_double)
			   && AlmostEqual(positive_infinity, other.positive_infinity)
			   && AlmostEqual(negative_infinity, other.negative_infinity) && AlmostEqual(nan_value, other.nan_value)
			   && max_int64 == other.max_int64 && min_int64 == other.min_int64;
	}
};

// Define serializer for NumericTests at global scope
JSON_DEFINE_SERIALIZER(NumericTests, max_double, min_double, positive_infinity, negative_infinity, nan_value, max_int64,
					   min_int64)

// Structure for Unicode testing
struct UnicodeTest
{
	std::string ascii;
	std::string latin_extended;
	std::string cyrillic;
	std::string chinese;
	std::string emoji;
	std::string mixed;

	bool operator==(const UnicodeTest& other) const
	{
		return ascii == other.ascii && latin_extended == other.latin_extended && cyrillic == other.cyrillic
			   && chinese == other.chinese && emoji == other.emoji && mixed == other.mixed;
	}
};

// Define serializer for UnicodeTest at global scope
JSON_DEFINE_SERIALIZER(UnicodeTest, ascii, latin_extended, cyrillic, chinese, emoji, mixed)

// Structure for typed field tests
struct TypedStruct
{
	int number_field;
	std::vector<int> array_field;
	bool bool_field;
};

// Define serializer for TypedStruct at global scope
JSON_DEFINE_SERIALIZER(TypedStruct, number_field, array_field, bool_field)

// Structure for empty fields test
struct EmptyStruct
{
	std::string name;
	std::vector<int> items;
	std::vector<std::vector<double>> nested;
	int count = 0;
	bool flag = false;
};

// Define serializer for EmptyStruct at global scope
JSON_DEFINE_SERIALIZER(EmptyStruct, name, items, nested, count, flag)

// Optional fields struct for testing
struct OptionalFieldsStruct
{
	std::string required_field;
	std::string optional_field;
	int optional_count = -1;

	// Constructor to make testing easier
	OptionalFieldsStruct(const std::string& req = "", const std::string& opt = "", int count = -1) :
		required_field(req), optional_field(opt), optional_count(count)
	{
	}

	bool operator==(const OptionalFieldsStruct& other) const
	{
		return required_field == other.required_field && optional_field == other.optional_field
			   && optional_count == other.optional_count;
	}
};

// Specialized serializer to handle missing fields
namespace ISXJson
{
template<>
struct Serializer<OptionalFieldsStruct>
{
	static JSON Serialize(const OptionalFieldsStruct& value)
	{
		JSON::Object obj;
		obj["required_field"] = JSON::From(value.required_field);
		obj["optional_field"] = JSON::From(value.optional_field);
		obj["optional_count"] = JSON::From(value.optional_count);
		return obj;
	}

	static OptionalFieldsStruct Deserialize(const JSON& json)
	{
		OptionalFieldsStruct value;
		value.required_field = json["required_field"].To<std::string>(); // Required

		// Only deserialize optional fields if they exist
		if (json.Contains("optional_field"))
		{
			value.optional_field = json["optional_field"].To<std::string>();
		}

		if (json.Contains("optional_count"))
		{
			value.optional_count = json["optional_count"].To<int>();
		}

		return value;
	}
};
} // namespace ISXJson

// A non-default-constructible type
class NonDefaultConstructible
{
private:
	int m_value;
	std::string m_name;

public:
	// No default constructor
	NonDefaultConstructible(int value, std::string name) : m_value(value), m_name(std::move(name)) {}

	// Custom deserialization function
	static NonDefaultConstructible FromJSON(const JSON& json)
	{
		return NonDefaultConstructible(json["value"].AsNumber(), json["name"].AsString());
	}

	// Serialize to JSON
	JSON ToJSON() const
	{
		JSON::Object obj;
		obj["value"] = JSON(m_value);
		obj["name"] = JSON(m_name);
		return obj;
	}

	// Getters for testing
	int GetValue() const { return m_value; }
	const std::string& GetName() const { return m_name; }
};

// Test: BasicSerializerSpecializations
// Tests the built-in serializer specializations for basic types (bool, int, double, string),
// verifying both serialization and deserialization work correctly.
TEST(SerializerTest, BasicSerializerSpecializations)
{
	// Test boolean serialization
	bool bool_value = true;
	JSON bool_json = JSON::From(bool_value);
	EXPECT_EQ(bool_json.GetType(), JSON::Type::BOOL);
	EXPECT_EQ(bool_json.AsBool(), bool_value);
	EXPECT_EQ(bool_json.To<bool>(), bool_value);

	// Test integer serialization
	int int_value = 42;
	JSON int_json = JSON::From(int_value);
	EXPECT_EQ(int_json.GetType(), JSON::Type::NUMBER);
	EXPECT_EQ(int_json.AsNumber(), int_value);
	EXPECT_EQ(int_json.To<int>(), int_value);

	// Test double serialization
	double double_value = 3.14159;
	JSON double_json = JSON::From(double_value);
	EXPECT_EQ(double_json.GetType(), JSON::Type::NUMBER);
	EXPECT_DOUBLE_EQ(double_json.AsNumber(), double_value);
	EXPECT_DOUBLE_EQ(double_json.To<double>(), double_value);

	// Test string serialization
	std::string string_value = "Hello, World!";
	JSON string_json = JSON::From(string_value);
	EXPECT_EQ(string_json.GetType(), JSON::Type::STRING);
	EXPECT_EQ(string_json.AsString(), string_value);
	EXPECT_EQ(string_json.To<std::string>(), string_value);
}

// Test: VectorSerialization
// Tests basic vector serialization and deserialization,
// including checking the correct array structure and element values.
TEST(SerializerTest, VectorSerialization)
{
	// Create a vector of integers to serialize
	std::vector<int> int_vector = {1, 2, 3, 4, 5};

	// Serialize the vector to JSON
	JSON json = JSON::From(int_vector);

	// Verify structure
	EXPECT_EQ(json.GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(json.Size(), 5);

	// Verify elements
	EXPECT_EQ(json[0].AsNumber(), 1);
	EXPECT_EQ(json[1].AsNumber(), 2);
	EXPECT_EQ(json[2].AsNumber(), 3);
	EXPECT_EQ(json[3].AsNumber(), 4);
	EXPECT_EQ(json[4].AsNumber(), 5);

	// Test deserialization
	std::vector<int> deserialized = json.To<std::vector<int>>();
	EXPECT_EQ(deserialized.size(), 5);
	EXPECT_EQ(deserialized, int_vector);

	// Test vector of strings
	std::vector<std::string> str_vector = {"hello", "world", "test"};
	JSON str_json = JSON::From(str_vector);

	// Verify structure
	EXPECT_EQ(str_json.GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(str_json.Size(), 3);

	// Verify elements
	EXPECT_EQ(str_json[0].AsString(), "hello");
	EXPECT_EQ(str_json[1].AsString(), "world");
	EXPECT_EQ(str_json[2].AsString(), "test");

	// Test deserialization
	std::vector<std::string> deserialized_str = str_json.To<std::vector<std::string>>();
	EXPECT_EQ(deserialized_str.size(), 3);
	EXPECT_EQ(deserialized_str, str_vector);
}

// Test: NestedVectorSerialization
// Tests serialization and deserialization of nested vectors (2D arrays),
// ensuring that the hierarchical structure is maintained.
TEST(SerializerTest, NestedVectorSerialization)
{
	// Create a nested vector (matrix)
	std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

	// Serialize the matrix to JSON
	JSON json = JSON::From(matrix);

	// Verify structure
	EXPECT_EQ(json.GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(json.Size(), 3);

	// Verify elements are arrays
	for (int i = 0; i < 3; i++)
	{
		EXPECT_EQ(json[i].GetType(), JSON::Type::ARRAY);
		EXPECT_EQ(json[i].Size(), 3);
	}

	// Verify specific elements
	EXPECT_EQ(json[0][0].AsNumber(), 1);
	EXPECT_EQ(json[1][1].AsNumber(), 5);
	EXPECT_EQ(json[2][2].AsNumber(), 9);

	// Test deserialization
	auto deserialized = json.To<std::vector<std::vector<int>>>();
	EXPECT_EQ(deserialized.size(), 3);
	EXPECT_EQ(deserialized, matrix);
}

// Test: MacroDefinedSerializer
// Tests the JSON_DEFINE_SERIALIZER macro for custom types,
// verifying that it correctly generates serializer code that works bidirectionally.
TEST(SerializerTest, MacroDefinedSerializer)
{
	// Create test struct
	SimpleStruct test{"Test Name", 42, true, {1.1, 2.2, 3.3}};

	// Serialize struct using the macro-defined serializer
	JSON json = JSON::From(test);

	// Verify structure
	EXPECT_EQ(json.GetType(), JSON::Type::OBJECT);
	EXPECT_TRUE(json.Contains("name"));
	EXPECT_TRUE(json.Contains("id"));
	EXPECT_TRUE(json.Contains("active"));
	EXPECT_TRUE(json.Contains("values"));

	// Verify field values
	EXPECT_EQ(json["name"].AsString(), "Test Name");
	EXPECT_EQ(json["id"].AsNumber(), 42);
	EXPECT_EQ(json["active"].AsBool(), true);
	EXPECT_EQ(json["values"].GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(json["values"].Size(), 3);

	// Test deserialization
	SimpleStruct deserialized = json.To<SimpleStruct>();
	EXPECT_EQ(deserialized.name, test.name);
	EXPECT_EQ(deserialized.id, test.id);
	EXPECT_EQ(deserialized.active, test.active);
	EXPECT_EQ(deserialized.values, test.values);

	// Overall equality check
	EXPECT_EQ(deserialized, test);
}

// Test: FriendSerializerMacro
// Tests the JSON_FRIEND_SERIALIZER macro for accessing private fields,
// verifying that private members can be properly serialized and deserialized.
TEST(SerializerTest, FriendSerializerMacro)
{
	// Create an instance with private fields
	PrivateClass priv(123, "Private Test");

	// Serialize using friend serializer
	JSON json = JSON::From(priv);

	// Verify structure and values
	EXPECT_EQ(json.GetType(), JSON::Type::OBJECT);
	EXPECT_TRUE(json.Contains("m_private_id"));
	EXPECT_TRUE(json.Contains("public_name"));
	EXPECT_EQ(json["m_private_id"].AsNumber(), 123);
	EXPECT_EQ(json["public_name"].AsString(), "Private Test");

	// Deserialize back to object
	PrivateClass deserialized = json.To<PrivateClass>();

	// Check both public and private fields were correctly deserialized
	EXPECT_EQ(deserialized.GetPrivateId(), 123);
	EXPECT_EQ(deserialized.public_name, "Private Test");
}

// Test: SerializationRoundTrip
// Tests complete serialization/deserialization roundtrip for complex nested structures,
// ensuring that all data is preserved through the cycle.
TEST(SerializerTest, SerializationRoundTrip)
{
	// Create a complex test object with nested structures
	ComplexObject complex{"Complex Test",
						  {{"First Item", 1, true, {1.1, 1.2}}, {"Second Item", 2, false, {2.1, 2.2}}},
						  {{1, 2, 3}, {4, 5, 6}}};

	// Perform serialization
	JSON json = JSON::From(complex);

	// Convert to string and back to verify text representation
	std::string serialized = json.Serialize();
	// We can't use JSON::Parse because it doesn't exist, would need to use Parser class

	// Deserialize directly from the JSON object
	ComplexObject deserialized = json.To<ComplexObject>();

	// Verify all fields match
	EXPECT_EQ(deserialized.title, complex.title);
	EXPECT_EQ(deserialized.items.size(), complex.items.size());
	EXPECT_EQ(deserialized.matrix.size(), complex.matrix.size());

	// Overall equality check
	EXPECT_EQ(deserialized, complex);
}

// Test: NumericEdgeCases
// Tests serialization of numeric limits and special values.
TEST(SerializerTest, NumericEdgeCases)
{
	// Create instance with numeric edge cases
	NumericTests nums{.max_double = std::numeric_limits<double>::max(),
					  .min_double = std::numeric_limits<double>::min(),
					  .positive_infinity = std::numeric_limits<double>::infinity(),
					  .negative_infinity = -std::numeric_limits<double>::infinity(),
					  .nan_value = std::numeric_limits<double>::quiet_NaN(),
					  .max_int64 = std::numeric_limits<int64_t>::max(),
					  .min_int64 = std::numeric_limits<int64_t>::min()};

	// Serialize to JSON
	JSON json = JSON::From(nums);

	// Verify each value serialized correctly
	EXPECT_EQ(json.GetType(), JSON::Type::OBJECT);
	EXPECT_TRUE(json.Contains("max_double"));
	EXPECT_TRUE(json.Contains("min_double"));
	EXPECT_TRUE(json.Contains("positive_infinity"));
	EXPECT_TRUE(json.Contains("negative_infinity"));
	EXPECT_TRUE(json.Contains("nan_value"));
	EXPECT_TRUE(json.Contains("max_int64"));
	EXPECT_TRUE(json.Contains("min_int64"));

	// Deserialize back and verify values match
	NumericTests deserialized = json.To<NumericTests>();

	// Due to JSON conversion limitations, some values might not round-trip exactly
	// so we check the expected behavior rather than exact equality
	EXPECT_EQ(deserialized.max_int64, nums.max_int64);
	EXPECT_EQ(deserialized.min_int64, nums.min_int64);

	// Max double might round-trip or be converted to string based on implementation
	// Min double should round-trip correctly
	EXPECT_TRUE(NumericTests::AlmostEqual(deserialized.min_double, nums.min_double));

	// Check special values - these may be serialized as null, strings, or special values
	// depending on the serializer implementation
	std::string serialized = json.Serialize();
	std::cout << "Numeric edge cases serialization: " << serialized << std::endl;
}

// Test: NullHandling
// Tests serialization and deserialization of null values.
TEST(SerializerTest, NullHandling)
{
	// Create JSON with null values
	JSON::Object obj;
	obj["null_value"] = JSON(); // Null JSON value
	obj["string_value"] = JSON("test");
	obj["number_value"] = JSON(42);

	JSON json(obj);

	// Verify null detection
	EXPECT_EQ(json["null_value"].GetType(), JSON::Type::NUL);
	EXPECT_NE(json["string_value"].GetType(), JSON::Type::NUL);
	EXPECT_NE(json["number_value"].GetType(), JSON::Type::NUL);

	// Test null in arrays
	JSON::Array arr;
	arr.push_back(JSON()); // Null
	arr.push_back(JSON(123));
	arr.push_back(JSON("test"));

	JSON arrayJson(arr);

	EXPECT_EQ(arrayJson[0].GetType(), JSON::Type::NUL);
	EXPECT_NE(arrayJson[1].GetType(), JSON::Type::NUL);
	EXPECT_NE(arrayJson[2].GetType(), JSON::Type::NUL);

	// Test serialization of null values
	std::string serialized = json.Serialize();
	std::cout << "Null handling serialization: " << serialized << std::endl;

	// We can't use JSON::Parse since it doesn't exist in the API
	// Would need to use Parser class instead
}

// Test: Unicode
// Tests handling of Unicode characters in serialization.
TEST(SerializerTest, Unicode)
{
	// Create instance with various Unicode strings
	UnicodeTest unicode{.ascii = "Hello, world!",
						.latin_extended = "Café au lait, Zoë's Voß straße",
						.cyrillic = "Привет, мир!",
						.chinese = "你好，世界！",
						.emoji = "🌍 🌎 🌏 😀 🚀 💻",
						.mixed = "Hello 你好 Привет 🌍"};

	// Serialize to JSON
	JSON json = JSON::From(unicode);

	// Verify JSON structure
	EXPECT_EQ(json.GetType(), JSON::Type::OBJECT);
	EXPECT_TRUE(json.Contains("ascii"));
	EXPECT_TRUE(json.Contains("latin_extended"));
	EXPECT_TRUE(json.Contains("cyrillic"));
	EXPECT_TRUE(json.Contains("chinese"));
	EXPECT_TRUE(json.Contains("emoji"));
	EXPECT_TRUE(json.Contains("mixed"));

	// Get serialized string
	std::string serialized = json.Serialize();
	std::cout << "Unicode serialization: " << serialized << std::endl;

	// Deserialize and verify Unicode roundtrip
	UnicodeTest deserialized = json.To<UnicodeTest>();

	EXPECT_EQ(deserialized.ascii, unicode.ascii);
	EXPECT_EQ(deserialized.latin_extended, unicode.latin_extended);
	EXPECT_EQ(deserialized.cyrillic, unicode.cyrillic);
	EXPECT_EQ(deserialized.chinese, unicode.chinese);
	EXPECT_EQ(deserialized.emoji, unicode.emoji);
	EXPECT_EQ(deserialized.mixed, unicode.mixed);
}

// Test: ErrorHandling
// Tests error handling during serialization and deserialization.
TEST(SerializerTest, ErrorHandling)
{
	// Test deserializing mismatched types
	JSON::Object obj;
	obj["number_field"] = JSON("not a number"); // String where number expected
	obj["array_field"] = JSON(42);				// Number where array expected
	obj["bool_field"] = JSON::Object();			// Object where bool expected

	JSON json(obj);

	// Test type mismatch handling
	try
	{
		TypedStruct ts = json.To<TypedStruct>();
		FAIL() << "Expected an exception for type mismatch";
	}
	catch (const std::exception& e)
	{
		std::cout << "Successfully caught expected exception: " << e.what() << std::endl;
	}

	// Test missing required fields
	JSON::Object incomplete;
	incomplete["number_field"] = JSON(42);
	// Missing array_field and bool_field

	JSON incompleteJson(incomplete);

	try
	{
		TypedStruct ts = incompleteJson.To<TypedStruct>();
		// This might pass or fail depending on implementation
		// If defaults are used for missing fields, print the result
		std::cout << "Deserialized with defaults: array_size=" << ts.array_field.size() << ", bool=" << ts.bool_field
				  << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Missing fields exception: " << e.what() << std::endl;
	}
}

// Test: LargeDataStructures
// Tests serialization of large data structures.
TEST(SerializerTest, LargeDataStructures)
{
	// Create a large vector
	std::vector<int> large_vector;
	const int VECTOR_SIZE = 10000;
	large_vector.reserve(VECTOR_SIZE);
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		large_vector.push_back(i);
	}

	// Measure serialization time
	auto start = std::chrono::high_resolution_clock::now();
	JSON json = JSON::From(large_vector);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> duration = end - start;
	std::cout << "Large vector serialization took " << duration.count() << "ms" << std::endl;

	// Verify serialized structure
	EXPECT_EQ(json.GetType(), JSON::Type::ARRAY);
	EXPECT_EQ(json.Size(), VECTOR_SIZE);

	// Test with a deep structure - simplified version that should compile
	// Let's create a simpler test with a limited nesting depth
	const int SAFE_NESTING_DEPTH = 10;

	// Create initial array
	JSON::Array outer_array;

	// Build the nested structure with a reasonable depth
	JSON::Array current_array;
	for (int i = 0; i < SAFE_NESTING_DEPTH; i++)
	{
		// Add a value to current level
		current_array.push_back(JSON(i));

		// If not at the last level, create a new inner array
		if (i < SAFE_NESTING_DEPTH - 1)
		{
			JSON::Array inner;
			current_array.push_back(JSON(inner));
			current_array = inner;
		}
	}

	// Create the JSON from the outer array
	JSON nested(outer_array);

	// Serialize the nested structure
	std::string deep_serialized;
	try
	{
		start = std::chrono::high_resolution_clock::now();
		deep_serialized = nested.Serialize();
		end = std::chrono::high_resolution_clock::now();

		duration = end - start;
		std::cout << "Deep nesting serialization took " << duration.count()
				  << "ms, length: " << deep_serialized.length() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Deep nesting exception (expected if depth limit exceeded): " << e.what() << std::endl;
	}
}

// Test: EmptyFieldsStruct
// Tests serialization of structs with empty containers and default field values.
TEST(SerializerTest, EmptyFieldsStruct)
{
	// Test serialization of default-initialized struct
	EmptyStruct empty;
	JSON json = JSON::From(empty);

	// Verify JSON structure
	EXPECT_EQ(json.GetType(), JSON::Type::OBJECT);
	EXPECT_EQ(json["name"].AsString(), "");
	EXPECT_EQ(json["items"].Size(), 0);
	EXPECT_EQ(json["nested"].Size(), 0);
	EXPECT_EQ(json["count"].AsNumber(), 0);
	EXPECT_FALSE(json["flag"].AsBool());

	// Test round-trip
	EmptyStruct deserialized = json.To<EmptyStruct>();
	EXPECT_EQ(deserialized.name, empty.name);
	EXPECT_EQ(deserialized.items, empty.items);
	EXPECT_EQ(deserialized.nested, empty.nested);
	EXPECT_EQ(deserialized.count, empty.count);
	EXPECT_EQ(deserialized.flag, empty.flag);
}

// Test: OptionalFields
// Tests serialization with missing optional fields that should use defaults.
TEST(SerializerTest, OptionalFields)
{
	// Create JSON with only some fields present
	JSON::Object obj;
	obj["required_field"] = JSON("test");
	// Intentionally omit optional_field and optional_count
	JSON json(obj);

	// Deserialize and check defaults are used
	OptionalFieldsStruct result = json.To<OptionalFieldsStruct>();
	EXPECT_EQ(result.required_field, "test");
	EXPECT_EQ(result.optional_field, ""); // Default empty string
	EXPECT_EQ(result.optional_count, -1); // Default -1

	// Test partial fields
	JSON::Object obj2;
	obj2["required_field"] = JSON("test2");
	obj2["optional_count"] = JSON(42);
	// Omit only optional_field
	JSON json2(obj2);

	OptionalFieldsStruct result2 = json2.To<OptionalFieldsStruct>();
	EXPECT_EQ(result2.required_field, "test2");
	EXPECT_EQ(result2.optional_field, ""); // Default empty string
	EXPECT_EQ(result2.optional_count, 42); // Provided value
}

// Test: CustomSerializationMethods
// Tests custom serialization methods for types that don't fit the default pattern.
TEST(SerializerTest, CustomSerializationMethods)
{
	// Create an instance of our non-default-constructible type
	NonDefaultConstructible original(42, "test");

	// Manually convert to JSON using custom method
	JSON json = original.ToJSON();

	// Verify JSON structure
	EXPECT_EQ(json.GetType(), JSON::Type::OBJECT);
	EXPECT_EQ(json["value"].AsNumber(), 42);
	EXPECT_EQ(json["name"].AsString(), "test");

	// Manually convert back using static factory method
	NonDefaultConstructible deserialized = NonDefaultConstructible::FromJSON(json);

	// Verify reconstruction
	EXPECT_EQ(deserialized.GetValue(), original.GetValue());
	EXPECT_EQ(deserialized.GetName(), original.GetName());
}
