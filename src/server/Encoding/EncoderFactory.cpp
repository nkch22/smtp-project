#include "EncoderFactory.hpp"

#include <algorithm>
#include <cctype>

#include "Base64Encoder.hpp"
#include "QuotedPrintableEncoder.hpp"

namespace ISXEncoding
{

namespace
{
std::string ToLower(const std::string& str)
{
	std::string lower = str;
	std::ranges::transform(lower, lower.begin(), [](unsigned char c) { return std::tolower(c); });
	return lower;
}
} // namespace

std::unique_ptr<IEncoder> EncoderFactory::CreateEncoder(const std::string& encodingName)
{
	std::string name = ToLower(encodingName);

	if (name == "base64") return std::make_unique<Base64Encoder>();

	if (name == "quoted-printable") return std::make_unique<QuotedPrintableEncoder>();

	return nullptr;
}

bool EncoderFactory::IsSupported(const std::string& encodingName)
{
	std::string name = ToLower(encodingName);
	return name == "base64" || name == "quoted-printable";
}

} // namespace ISXEncoding
