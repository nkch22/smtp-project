#include "Utils.hpp"

#include <algorithm>

#include "libbase64.h"

namespace SMTP
{

namespace Protocol
{

std::string ToUpper(const std::string_view src)
{
    std::string upper_copy{};
    upper_copy.resize(std::size(src));
    std::transform(std::begin(src), std::end(src), std::begin(upper_copy), ::toupper);
    return upper_copy;
}

std::string RemoveWhitespaces(const std::string_view src)
{
	std::string copy{src};
	auto removed_elements{std::remove_if(std::begin(copy), std::end(copy), ::isspace)};
	copy.erase(removed_elements, std::end(copy));
	return copy;
}

std::string RemoveCRLF(const std::string_view src)
{
    const auto found_crlf{src.find("\r\n")};
    const auto clean_string{src.substr(0, found_crlf)};
    return std::string{clean_string};
}

std::string DecodeBase64(const std::string_view src)
{
    std::string decoded_result(std::size(src), '\0');
	std::size_t decoded_result_size{};
	base64_decode(
		std::data(src), std::size(src), 
		std::data(decoded_result), &decoded_result_size, 0
	);
	decoded_result.resize(decoded_result_size);
    return decoded_result;
}


std::string EncodeBase64(const std::string_view src)
{
    constexpr static double multiply_factor{1.5};
    std::string encoded_result(
        static_cast<std::size_t>(std::size(src) * multiply_factor), '\0'
    );
    std::size_t encoded_result_size{};
    base64_encode(
        std::data(src), std::size(src),
        std::data(encoded_result), &encoded_result_size, 0
    );
    encoded_result.resize(encoded_result_size);
    return encoded_result;
}

std::string HashData(const std::string_view src)
{
    return std::to_string(
        std::hash<std::string>{}(std::string{src})
    );
}

}

}