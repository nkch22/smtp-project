#include "QuotedPrintableEncoder.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace ISXEncoding
{

bool QuotedPrintableEncoder::IsSafeChar(unsigned char c)
{
	// safe printable ASCII characters (33-60, 62-126), excluding '=' (61)
	return (c >= 33 && c <= 60) || (c >= 62 && c <= 126);
}

bool QuotedPrintableEncoder::IsHexDigit(char c)
{
	return std::isxdigit(static_cast<unsigned char>(c)) != 0;
}

int QuotedPrintableEncoder::HexToInt(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

std::string QuotedPrintableEncoder::Encode(const std::vector<uint8_t>& data)
{
	std::string input(data.begin(), data.end());
	std::ostringstream os;
	size_t line_length = 0;
	const size_t MAX_LINE_LENGTH = 76;

	for (size_t i = 0; i < input.size(); ++i)
	{
		unsigned char c = input[i];

		// check if we need to soft-break the line (max 76 chars per line)
		if (line_length >= MAX_LINE_LENGTH - 3)
		{ // 76 - 3 = 73 (to account for possible "=XX" on next char)
			os << "=\r\n";
			line_length = 0;
		}

		// safe characters can be output directly
		if (IsSafeChar(c))
		{
			os << c;
			line_length++;
		}
		// space and tab can be output directly, but not at end of line
		else if ((c == ' ' || c == '\t') && i + 1 < input.size() && input[i + 1] != '\r' && input[i + 1] != '\n')
		{
			os << c;
			line_length++;
		}
		// trailing whitespace must be encoded
		else if ((c == ' ' || c == '\t') && (i + 1 >= input.size() || input[i + 1] == '\r' || input[i + 1] == '\n'))
		{
			os << '=' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
			line_length += 3;
		}
		// CRLF is output directly
		else if (c == '\r' && i + 1 < input.size() && input[i + 1] == '\n')
		{
			os << "\r\n";
			line_length = 0;
			i++; // Skip the LF
		}
		// everything else is encoded
		else
		{
			os << '=' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
			line_length += 3;
		}

		// check for soft break if near the end of line and we're not at end of input
		if (line_length > 73 && i + 1 < input.size())
		{
			os << "=\r\n";
			line_length = 0;
		}
	}

	return os.str();
}

std::vector<uint8_t> QuotedPrintableEncoder::Decode(const std::string& encoded)
{
	std::string decoded;
	decoded.reserve(encoded.size());

	for (size_t i = 0; i < encoded.size(); ++i)
	{
		char c = encoded[i];

		if (c == '=')
		{
			// check for soft line break (RFC 2045: "=" at the end of a line, followed by CRLF)
			if (i + 2 < encoded.size() && encoded[i + 1] == '\r' && encoded[i + 2] == '\n')
			{
				i += 2; // Skip =CRLF
				continue;
			}

			// check for encoded character
			if (i + 2 < encoded.size())
			{
				char h1 = encoded[i + 1];
				char h2 = encoded[i + 2];

				// check if both are hex digits
				if (IsHexDigit(h1) && IsHexDigit(h2))
				{
					int value = (HexToInt(h1) << 4) | HexToInt(h2);
					decoded.push_back(static_cast<char>(value));
					i += 2; // Skip the two hex digits
					continue;
				}
				decoded.push_back(c);
			}
			else
			{
				decoded.push_back(c);
			}
		}
		else
		{
			// regular character
			decoded.push_back(c);
		}
	}

	return {decoded.begin(), decoded.end()};
}

} // namespace ISXEncoding
