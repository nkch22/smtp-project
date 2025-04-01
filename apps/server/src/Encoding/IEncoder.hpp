#pragma once

#include <memory>
#include <string>
#include <vector>

namespace ISXEncoding
{

class IEncoder
{
public:
	virtual ~IEncoder() = default;

public:
	virtual std::string Encode(const std::vector<uint8_t>& data) = 0;
	virtual std::vector<uint8_t> Decode(const std::string& encoded) = 0;

	virtual std::string GetName() const = 0;
};

} // namespace ISXEncoding
