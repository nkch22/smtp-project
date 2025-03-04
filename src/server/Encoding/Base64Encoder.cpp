#include "Base64Encoder.hpp"

#include <algorithm>

namespace ISXEncoding
{

const char* Base64Encoder::GetEncodingTable()
{
	static const char S_BASE64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	return S_BASE64_CHARS;
}

const unsigned char* Base64Encoder::GetDecodingTable()
{
	static const unsigned char BASE64_TABLE[256] = {
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63, 52, 53, 54, 55,
		56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64, 64, 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64, 64, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
	return BASE64_TABLE;
}

std::string Base64Encoder::Encode(const std::vector<uint8_t>& data)
{
	const char* base64_chars = GetEncodingTable();
	std::string encoded;
	encoded.reserve(((data.size() + 2) / 3) * 4
					+ (data.size() / 57) * 2); // Account for CRLF every 76 chars (57 bytes input = 76 chars output)

	size_t line_length = 0;
	const size_t MAX_LINE_LENGTH = 76; // RFC 2045 requirement

	for (size_t i = 0; i < data.size(); i += 3)
	{
		uint32_t chunk = 0;

		// convert 3 bytes to a 24-bit chunk
		chunk |= (i < data.size()) ? (data[i] << 16) : 0;
		chunk |= (i + 1 < data.size()) ? (data[i + 1] << 8) : 0;
		chunk |= (i + 2 < data.size()) ? data[i + 2] : 0;

		// Check if we need to add a line break (before adding 4 more chars)
		if (line_length + 4 > MAX_LINE_LENGTH && i > 0)
		{
			encoded.append("\r\n");
			line_length = 0;
		}

		// convert the 24-bit chunk to 4 base64 characters
		encoded.push_back(base64_chars[(chunk >> 18) & 0x3F]);
		encoded.push_back(base64_chars[(chunk >> 12) & 0x3F]);
		encoded.push_back((i + 1 < data.size()) ? base64_chars[(chunk >> 6) & 0x3F] : '=');
		encoded.push_back((i + 2 < data.size()) ? base64_chars[chunk & 0x3F] : '=');

		line_length += 4;
	}

	return encoded;
}

std::vector<uint8_t> Base64Encoder::Decode(const std::string& encoded)
{
	const unsigned char* base64_table = GetDecodingTable();

	// remove whitespace and other non-base64 characters
	std::string clean;
	clean.reserve(encoded.size());
	for (char c : encoded)
	{
		// RFC 2045: Base64 decoder should ignore all whitespace characters
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '/'
			|| c == '=')
		{
			clean.push_back(c);
		}
	}

	// RFC 2045: The encoded output must be represented in lines of no more than 76 characters each

	// check if the input length is valid (after removing whitespace)
	if (clean.size() % 4 != 0)
	{
		// RFC 2045: Input must be a multiple of 4 characters
		throw std::runtime_error("Invalid Base64 input: length is not a multiple of 4");
	}

	size_t padding = 0;
	if (!clean.empty())
	{
		if (clean.back() == '=') padding++;
		if (clean.size() > 1 && clean[clean.size() - 2] == '=') padding++;
	}

	// calculate the size of the decoded data
	std::vector<uint8_t> decoded;
	decoded.reserve(((clean.size() / 4) * 3) - padding);

	// process each group of 4 characters
	for (size_t i = 0; i < clean.size(); i += 4)
	{
		// get values for each base64 character
		uint32_t values[4] = {0};
		for (size_t j = 0; j < 4; j++)
		{
			char c = clean[i + j];
			if (c == '=')
			{
				// Padding character can only appear at positions 2 or 3 in a quartet
				if (j < 2)
				{
					throw std::runtime_error("Invalid Base64 input: padding character in illegal position");
				}
				break;
			}

			values[j] = base64_table[(unsigned char)c];

			// Verify the character is a valid Base64 character
			if (values[j] == 64)
			{
				throw std::runtime_error("Invalid Base64 input: illegal character in input");
			}
		}

		// combine the values into bytes
		uint32_t combined = (values[0] << 18) | (values[1] << 12) | (values[2] << 6) | values[3];

		// add the bytes to the output
		decoded.push_back((combined >> 16) & 0xFF);
		if (clean[i + 2] != '=') decoded.push_back((combined >> 8) & 0xFF);
		if (clean[i + 3] != '=') decoded.push_back(combined & 0xFF);
	}

	return decoded;
}

} // namespace ISXEncoding
