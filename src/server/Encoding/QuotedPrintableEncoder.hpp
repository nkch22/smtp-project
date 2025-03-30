#pragma once

#include "IEncoder.hpp"

namespace ISXEncoding
{
class QuotedPrintableEncoder : public IEncoder
{
public:
	std::string Encode(const std::vector<uint8_t>& data) override;
	std::vector<uint8_t> Decode(const std::string& encoded) override;

	std::string GetName() const override { return "quoted-printable"; }

private:
	static bool IsSafeChar(unsigned char c);
	static bool IsHexDigit(char c);
	static int HexToInt(char c);
};

} // namespace ISXEncoding
