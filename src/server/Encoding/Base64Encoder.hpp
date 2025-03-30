#pragma once

#include "IEncoder.hpp"

namespace ISXEncoding
{
class Base64Encoder : public IEncoder
{
public:
	std::string Encode(const std::vector<uint8_t>& data) override;
	std::vector<uint8_t> Decode(const std::string& encoded) override;

	std::string GetName() const override { return "base64"; }

private:
	static const char* GetEncodingTable();
	static const unsigned char* GetDecodingTable();
};

} // namespace ISXEncoding
