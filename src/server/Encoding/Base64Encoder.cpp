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
	encoded.reserve(((data.size() + 2) / 3) * 4);

	for (size_t i = 0; i < data.size(); i += 3)
	{
		uint32_t chunk = 0;

		// convert 3 bytes to a 24-bit chunk
		chunk |= (i < data.size()) ? (data[i] << 16) : 0;
		chunk |= (i + 1 < data.size()) ? (data[i + 1] << 8) : 0;
		chunk |= (i + 2 < data.size()) ? data[i + 2] : 0;

		// convert the 24-bit chunk to 4 base64 characters
		encoded.push_back(base64_chars[(chunk >> 18) & 0x3F]);
		encoded.push_back(base64_chars[(chunk >> 12) & 0x3F]);
		encoded.push_back((i + 1 < data.size()) ? base64_chars[(chunk >> 6) & 0x3F] : '=');
		encoded.push_back((i + 2 < data.size()) ? base64_chars[chunk & 0x3F] : '=');
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
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '/'
			|| c == '=')
		{
			clean.push_back(c);
		}
	}

	// check if the input length is valid
	if (clean.size() % 4 != 0) return {};

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
				if (j < 2) return {};

				break;
			}

			values[j] = base64_table[(unsigned char)c];

			if (values[j] == 64) return {};
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
