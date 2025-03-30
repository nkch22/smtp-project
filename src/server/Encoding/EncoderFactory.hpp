#pragma once

#include <memory>
#include <string>

#include "IEncoder.hpp"

namespace ISXEncoding
{
class EncoderFactory
{
public:
	static std::unique_ptr<IEncoder> CreateEncoder(const std::string& encodingName);

	static bool IsSupported(const std::string& encodingName);
};

} // namespace ISXEncoding
