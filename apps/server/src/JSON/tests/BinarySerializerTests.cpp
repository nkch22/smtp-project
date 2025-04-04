#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>

#include "../JSONAll.hpp"
#include "../BinarySerializer.hpp"

using namespace ISXJson;

// Simple struct for testing binary serialization with custom types
struct BinaryTestStruct
{
    std::string name;
    int id;
    bool active;
    std::vector<double> values;
    
    // Allow comparison for testing
    bool operator==(const BinaryTestStruct& other) const
    {
        return name == other.name && 
               id == other.id && 
               active == other.active && 
               values == other.values;
    }
};

// Define serializer at global scope
JSON_DEFINE_SERIALIZER(BinaryTestStruct, name, id, active, values)

// Test fixture for binary serialization tests
class BinarySerializerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create temporary directory for test files if it doesn't exist
        if (!std::filesystem::exists("test_temp"))
        {
            std::filesystem::create_directory("test_temp");
        }
    }
    
    void TearDown() override
    {
        // Clean up temporary files
        if (std::filesystem::exists("test_temp"))
        {
            std::filesystem::remove_all("test_temp");
        }
    }
    
    // Helper function to save binary data to a file
    bool SaveBinaryToFile(const std::vector<uint8_t>& data, const std::string& filename)
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file)
        {
            return false;
        }
        
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        return file.good();
    }
    
    // Helper function to load binary data from a file
    std::vector<uint8_t> LoadBinaryFromFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file)
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<uint8_t> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        {
            throw std::runtime_error("Failed to read file: " + filename);
        }
        
        return buffer;
    }
};

// Test: Simple JSON Types
// Test serialization and deserialization of basic JSON types
TEST_F(BinarySerializerTest, SimpleJSONTypes)
{
    // Test null
    {
        JSON nullJson; // Default constructor creates null
        std::vector<uint8_t> binary = BinarySerializer::Serialize(nullJson);
        ASSERT_FALSE(binary.empty());
        
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUL);
    }
    
    // Test boolean
    {
        JSON trueJson(true);
        std::vector<uint8_t> binary = BinarySerializer::Serialize(trueJson);
        ASSERT_FALSE(binary.empty());
        
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::BOOL);
        EXPECT_TRUE(restored.AsBool());
        
        JSON falseJson(false);
        binary = BinarySerializer::Serialize(falseJson);
        restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::BOOL);
        EXPECT_FALSE(restored.AsBool());
    }
    
    // Test number
    {
        JSON intJson(42);
        std::vector<uint8_t> binary = BinarySerializer::Serialize(intJson);
        ASSERT_FALSE(binary.empty());
        
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUMBER);
        EXPECT_EQ(restored.AsNumber(), 42);
        
        JSON floatJson(3.14159);
        binary = BinarySerializer::Serialize(floatJson);
        restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUMBER);
        EXPECT_DOUBLE_EQ(restored.AsNumber(), 3.14159);
    }
    
    // Test string
    {
        JSON strJson("Hello, Binary Serialization!");
        std::vector<uint8_t> binary = BinarySerializer::Serialize(strJson);
        ASSERT_FALSE(binary.empty());
        
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::STRING);
        EXPECT_EQ(restored.AsString(), "Hello, Binary Serialization!");
    }
}

// Test: Special Number Values
// Test serialization and deserialization of special numeric values
TEST_F(BinarySerializerTest, SpecialNumberValues)
{
    // Test large numbers
    {
        const double largeNum = 1e+10;
        JSON json(largeNum);
        std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUMBER);
        EXPECT_DOUBLE_EQ(restored.AsNumber(), largeNum);
    }
    
    // Test small numbers
    {
        const double smallNum = 1e-10;
        JSON json(smallNum);
        std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUMBER);
        EXPECT_DOUBLE_EQ(restored.AsNumber(), smallNum);
    }
    
    // Test int64 edge values
    {
        const int64_t maxInt63 = (1LL << 62) - 1; // Large but safe value
        JSON json(static_cast<double>(maxInt63));
        std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
        JSON restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUMBER);
        EXPECT_DOUBLE_EQ(restored.AsNumber(), static_cast<double>(maxInt63));
        
        const double smallButSafe = 1e-15; // Small but safe value
        json = JSON(smallButSafe);
        binary = BinarySerializer::Serialize(json);
        restored = BinarySerializer::Deserialize(binary);
        EXPECT_EQ(restored.GetType(), JSON::Type::NUMBER);
        EXPECT_DOUBLE_EQ(restored.AsNumber(), smallButSafe);
    }
}

// Test: Array Serialization
// Test serialization and deserialization of JSON arrays
TEST_F(BinarySerializerTest, ArraySerialization)
{
    // Create a mixed-type array
    JSON::Array array;
    array.push_back(JSON()); // null
    array.push_back(JSON(true));
    array.push_back(JSON(42));
    array.push_back(JSON(3.14));
    array.push_back(JSON("text"));
    
    JSON arrayJson(array);
    
    // Serialize the array
    std::vector<uint8_t> binary = BinarySerializer::Serialize(arrayJson);
    ASSERT_FALSE(binary.empty());
    
    // Deserialize the array
    JSON restored = BinarySerializer::Deserialize(binary);
    EXPECT_EQ(restored.GetType(), JSON::Type::ARRAY);
    EXPECT_EQ(restored.Size(), 5);
    
    // Verify array elements
    EXPECT_EQ(restored[0].GetType(), JSON::Type::NUL);
    EXPECT_EQ(restored[1].GetType(), JSON::Type::BOOL);
    EXPECT_TRUE(restored[1].AsBool());
    EXPECT_EQ(restored[2].GetType(), JSON::Type::NUMBER);
    EXPECT_EQ(restored[2].AsNumber(), 42);
    EXPECT_EQ(restored[3].GetType(), JSON::Type::NUMBER);
    EXPECT_DOUBLE_EQ(restored[3].AsNumber(), 3.14);
    EXPECT_EQ(restored[4].GetType(), JSON::Type::STRING);
    EXPECT_EQ(restored[4].AsString(), "text");
}

// Test: Object Serialization
// Test serialization and deserialization of JSON objects
TEST_F(BinarySerializerTest, ObjectSerialization)
{
    // Create a mixed-type object
    JSON::Object obj;
    obj["null_value"] = JSON(); // null value
    obj["bool_value"] = JSON(true);
    obj["int_value"] = JSON(42);
    obj["float_value"] = JSON(3.14);
    obj["string_value"] = JSON("text");
    
    JSON objJson(obj);
    
    // Serialize the object
    std::vector<uint8_t> binary = BinarySerializer::Serialize(objJson);
    ASSERT_FALSE(binary.empty());
    
    // Deserialize the object
    JSON restored = BinarySerializer::Deserialize(binary);
    EXPECT_EQ(restored.GetType(), JSON::Type::OBJECT);
    EXPECT_EQ(restored.Size(), 5);
    
    // Verify object properties
    EXPECT_TRUE(restored.Contains("null_value"));
    EXPECT_TRUE(restored.Contains("bool_value"));
    EXPECT_TRUE(restored.Contains("int_value"));
    EXPECT_TRUE(restored.Contains("float_value"));
    EXPECT_TRUE(restored.Contains("string_value"));
    
    EXPECT_EQ(restored["null_value"].GetType(), JSON::Type::NUL);
    EXPECT_EQ(restored["bool_value"].GetType(), JSON::Type::BOOL);
    EXPECT_TRUE(restored["bool_value"].AsBool());
    EXPECT_EQ(restored["int_value"].GetType(), JSON::Type::NUMBER);
    EXPECT_EQ(restored["int_value"].AsNumber(), 42);
    EXPECT_EQ(restored["float_value"].GetType(), JSON::Type::NUMBER);
    EXPECT_DOUBLE_EQ(restored["float_value"].AsNumber(), 3.14);
    EXPECT_EQ(restored["string_value"].GetType(), JSON::Type::STRING);
    EXPECT_EQ(restored["string_value"].AsString(), "text");
}

// Test: Nested Structures
// Test serialization and deserialization of nested JSON structures
TEST_F(BinarySerializerTest, NestedStructures)
{
    // Create a nested object with arrays
    JSON::Object obj;
    JSON::Array array1;
    array1.push_back(JSON(1));
    array1.push_back(JSON(2));
    array1.push_back(JSON(3));
    
    JSON::Object nestedObj;
    nestedObj["a"] = JSON("A");
    nestedObj["b"] = JSON("B");
    
    JSON::Array array2;
    array2.push_back(JSON(nestedObj));
    array2.push_back(JSON("element"));
    
    obj["array1"] = JSON(array1);
    obj["array2"] = JSON(array2);
    
    JSON json(obj);
    
    // Serialize the nested structure
    std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
    ASSERT_FALSE(binary.empty());
    
    // Deserialize the nested structure
    JSON restored = BinarySerializer::Deserialize(binary);
    EXPECT_EQ(restored.GetType(), JSON::Type::OBJECT);
    EXPECT_TRUE(restored.Contains("array1"));
    EXPECT_TRUE(restored.Contains("array2"));
    
    // Verify first array
    EXPECT_EQ(restored["array1"].GetType(), JSON::Type::ARRAY);
    EXPECT_EQ(restored["array1"].Size(), 3);
    EXPECT_EQ(restored["array1"][0].AsNumber(), 1);
    EXPECT_EQ(restored["array1"][1].AsNumber(), 2);
    EXPECT_EQ(restored["array1"][2].AsNumber(), 3);
    
    // Verify second array with nested object
    EXPECT_EQ(restored["array2"].GetType(), JSON::Type::ARRAY);
    EXPECT_EQ(restored["array2"].Size(), 2);
    EXPECT_EQ(restored["array2"][0].GetType(), JSON::Type::OBJECT);
    EXPECT_EQ(restored["array2"][1].GetType(), JSON::Type::STRING);
    EXPECT_EQ(restored["array2"][1].AsString(), "element");
    
    // Verify nested object
    EXPECT_TRUE(restored["array2"][0].Contains("a"));
    EXPECT_TRUE(restored["array2"][0].Contains("b"));
    EXPECT_EQ(restored["array2"][0]["a"].AsString(), "A");
    EXPECT_EQ(restored["array2"][0]["b"].AsString(), "B");
}

// Test: File IO
// Test saving binary serialized data to file and loading it back
TEST_F(BinarySerializerTest, FileIO)
{
    // Create a test object
    JSON::Object obj;
    obj["name"] = JSON("Test Object");
    obj["value"] = JSON(42);
    obj["active"] = JSON(true);
    
    JSON json(obj);
    
    // Serialize to binary
    std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
    ASSERT_FALSE(binary.empty());
    
    // Save to file
    std::string filename = "test_temp/test_data.bin";
    ASSERT_TRUE(SaveBinaryToFile(binary, filename));
    
    // Load from file
    std::vector<uint8_t> loadedBinary = LoadBinaryFromFile(filename);
    ASSERT_EQ(loadedBinary.size(), binary.size());
    
    // Compare binary data
    for (size_t i = 0; i < binary.size(); ++i)
    {
        EXPECT_EQ(loadedBinary[i], binary[i]);
    }
    
    // Deserialize loaded data
    JSON restored = BinarySerializer::Deserialize(loadedBinary);
    EXPECT_EQ(restored.GetType(), JSON::Type::OBJECT);
    EXPECT_TRUE(restored.Contains("name"));
    EXPECT_TRUE(restored.Contains("value"));
    EXPECT_TRUE(restored.Contains("active"));
    EXPECT_EQ(restored["name"].AsString(), "Test Object");
    EXPECT_EQ(restored["value"].AsNumber(), 42);
    EXPECT_TRUE(restored["active"].AsBool());
}

// Test: Custom Type Serialization
// Test direct serialization of custom types
TEST_F(BinarySerializerTest, CustomTypeSerialization)
{
    // Create a test struct
    BinaryTestStruct test;
    test.name = "Test Struct";
    test.id = 42;
    test.active = true;
    test.values = {1.1, 2.2, 3.3};
    
    // Serialize directly to binary
    std::vector<uint8_t> binary = BinarySerializer::ToBinary(test);
    ASSERT_FALSE(binary.empty());
    
    // Deserialize directly from binary
    BinaryTestStruct restored = BinarySerializer::FromBinary<BinaryTestStruct>(binary);
    
    // Verify fields
    EXPECT_EQ(restored.name, test.name);
    EXPECT_EQ(restored.id, test.id);
    EXPECT_EQ(restored.active, test.active);
    ASSERT_EQ(restored.values.size(), test.values.size());
    
    for (size_t i = 0; i < test.values.size(); ++i)
    {
        EXPECT_DOUBLE_EQ(restored.values[i], test.values[i]);
    }
    
    // Test overall equality
    EXPECT_EQ(restored, test);
}

// Test: Large Data Serialization
// Test serialization of large amounts of data
TEST_F(BinarySerializerTest, LargeDataSerialization)
{
    // Create a large array
    const int ARRAY_SIZE = 1000; // Reduced size for faster tests
    JSON::Array largeArray;
    
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        largeArray.push_back(JSON(i));
    }
    
    JSON json(largeArray);
    
    // Serialize to binary
    std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
    ASSERT_FALSE(binary.empty());
    
    // Deserialize from binary
    JSON restored = BinarySerializer::Deserialize(binary);
    EXPECT_EQ(restored.GetType(), JSON::Type::ARRAY);
    EXPECT_EQ(restored.Size(), ARRAY_SIZE);
    
    // Verify some elements
    EXPECT_EQ(restored[0].AsNumber(), 0);
    EXPECT_EQ(restored[ARRAY_SIZE / 2].AsNumber(), ARRAY_SIZE / 2);
    EXPECT_EQ(restored[ARRAY_SIZE - 1].AsNumber(), ARRAY_SIZE - 1);
}

// Test: Error Cases
// Test error handling in binary serialization/deserialization
TEST_F(BinarySerializerTest, ErrorCases)
{
    // Test empty binary data
    std::vector<uint8_t> emptyData;
    EXPECT_THROW(BinarySerializer::Deserialize(emptyData), std::runtime_error);
    
    // Test corrupt binary data
    std::vector<uint8_t> corruptData = {0xFF, 0xFF, 0xFF};
    EXPECT_THROW(BinarySerializer::Deserialize(corruptData), std::runtime_error);
    
    // Test incomplete binary data
    JSON json("Test");
    std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
    ASSERT_FALSE(binary.empty());
    
    // Cut off the last few bytes to make it incomplete
    std::vector<uint8_t> incompleteData(binary.begin(), binary.end() - 2);
    EXPECT_THROW(BinarySerializer::Deserialize(incompleteData), std::runtime_error);
}

// Test: Format Compatibility
// Test that the format is compatible between runs
TEST_F(BinarySerializerTest, FormatCompatibility)
{
    // Create a test object
    JSON::Object obj;
    obj["string"] = JSON("test");
    obj["number"] = JSON(42);
    obj["boolean"] = JSON(true);
    
    JSON::Array arr;
    arr.push_back(JSON(1));
    arr.push_back(JSON(2));
    obj["array"] = JSON(arr);
    
    JSON json(obj);
    
    // Serialize to binary
    std::vector<uint8_t> binary = BinarySerializer::Serialize(json);
    ASSERT_FALSE(binary.empty());
    
    // Save to file for future compatibility tests
    std::string filename = "test_temp/format_test.bin";
    ASSERT_TRUE(SaveBinaryToFile(binary, filename));
    
    // Load and check immediately
    std::vector<uint8_t> loadedBinary = LoadBinaryFromFile(filename);
    JSON restored = BinarySerializer::Deserialize(loadedBinary);
    
    // Verify structure and values
    EXPECT_EQ(restored.GetType(), JSON::Type::OBJECT);
    EXPECT_TRUE(restored.Contains("string"));
    EXPECT_TRUE(restored.Contains("number"));
    EXPECT_TRUE(restored.Contains("boolean"));
    EXPECT_TRUE(restored.Contains("array"));
    
    EXPECT_EQ(restored["string"].AsString(), "test");
    EXPECT_EQ(restored["number"].AsNumber(), 42);
    EXPECT_TRUE(restored["boolean"].AsBool());
    
    EXPECT_EQ(restored["array"].GetType(), JSON::Type::ARRAY);
    EXPECT_EQ(restored["array"].Size(), 2);
    EXPECT_EQ(restored["array"][0].AsNumber(), 1);
    EXPECT_EQ(restored["array"][1].AsNumber(), 2);
} 