#pragma once

#include <string>

namespace SMTP
{

namespace Protocol
{

std::string ToUpper(const std::string_view src);
std::string RemoveWhitespaces(const std::string_view src);
std::string RemoveCRLF(const std::string_view src);
std::string DecodeBase64(const std::string_view src);
std::string EncodeBase64(const std::string_view src);
std::string HashData(const std::string_view src);

}

}