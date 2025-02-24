/**
 * @file Parser.hpp
 * @brief Declaration of the Parser class.
 *
 * @details
 *  header file declares the arser class which provides functionality to
 * parse JSON content from a file. It constructs a JSON object hierarchy from the file content.
 */

#pragma once

#include <filesystem>
#include <string>

#include "JSON.hpp"

namespace ISXJson
{

/**
 * @class Parser
 * @brief Parses JSON content from a file and constructs a JSON object hierarchy.
 *
 * This class reads the entire content of a JSON file and parses it into a JSON value.
 */
class Parser
{
public:
	/**
	 * @brief Constructs a parser for the specified JSON file.
	 * @param file_path Path to the JSON file to parse.
	 * @throw std::runtime_error if the file cannot be opened.
	 */
	explicit Parser(const std::filesystem::path& file_path);

	/**
	 * @brief Parses the entire JSON content from the file.
	 * @return The root JSON value parsed from the file.
	 * @throw std::runtime_error if there is a syntax error or unexpected trailing characters.
	 */
	JSON Parse();

private:
	/**
	 * @brief Skips all whitespace characters in the input.
	 */
	void SkipWhitespace();

	/**
	 * @brief Peeks at the current character in the input without advancing.
	 * @return The current character, or '\0' if at the end of the input.
	 */
	char Peek() const;

	/**
	 * @brief Retrieves the current character and advances the position.
	 * @return The current character.
	 * @throw std::runtime_error if at the end of input.
	 */
	char Get();

	/**
	 * @brief Parses a JSON value based on the current character.
	 * @return The parsed JSON value.
	 * @throw std::runtime_error if an unexpected character is encountered.
	 */
	JSON ParseValue();

	/**
	 * @brief Parses a JSON string.
	 * @return A JSON value containing the string.
	 * @throw std::runtime_error if the string is unterminated or contains an invalid escape sequence.
	 */
	JSON ParseString();

	/**
	 * @brief Parses a JSON number.
	 * @return A JSON value containing the number.
	 * @throw std::runtime_error for an invalid number format.
	 */
	JSON ParseNumber();

	/**
	 * @brief Parses a JSON boolean.
	 * @return A JSON value containing the boolean.
	 * @throw std::runtime_error if the boolean literal is invalid.
	 */
	JSON ParseBool();

	/**
	 * @brief Parses a JSON null.
	 * @return A JSON null value.
	 * @throw std::runtime_error if the null literal is invalid.
	 */
	JSON ParseNull();

	/**
	 * @brief Parses a JSON array.
	 * @return A JSON value containing the array.
	 * @throw std::runtime_error if there is a syntax error in the array.
	 */
	JSON ParseArray();

	/**
	 * @brief Parses a JSON object.
	 * @return A JSON value containing the object.
	 * @throw std::runtime_error if there is a syntax error in the object.
	 */
	JSON ParseObject();

private:
	std::string m_input; ///< Buffer containing the JSON file content.
	size_t m_pos = 0;	 ///< Current position in the input buffer.
};

} // namespace ISXJson
