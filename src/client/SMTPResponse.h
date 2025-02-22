#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace SMTP
{
// Enum for SMTP result codes
enum ResultCode
{
	NONE = 0,
	READY = 220,
	GOODBYE = 221,
	AUTH_SUCCESSFUL = 235,
	OKAY = 250,
	ACCEPTED = 334,
	DATA = 354,
	TLS_NOT_AVAILABLE = 454,
	SYNTAX_ERROR = 501,
};

// Struct to hold SMTP responses
struct SMTPResponse
{
	SMTPResponse() : message(""), code(ResultCode::NONE) {};

	SMTPResponse(std::string message, ResultCode code) : message(message), code(code) {}

	SMTPResponse(SMTPResponse&& other) noexcept : message(std::move(other.message)), code(other.code) {}

	SMTPResponse(const SMTPResponse& other) : message(other.message), code(other.code) {}

	SMTPResponse& operator=(const SMTPResponse& other)
	{
		if (this != &other)
		{
			message = other.message;
			code = other.code;
		}
		return *this;
	}

	SMTPResponse& operator=(const SMTPResponse&& other)
	{
		if (this != &other)
		{
			message = std::move(other.message);
			code = other.code;
		}
		return *this;
	}

	std::string message;
	ResultCode code;
};

// Function to assert expected SMTP code
inline void AssertCode(const SMTPResponse& response, ResultCode expected)
{
	std::cout << "Response: " << response.message << std::endl;
	if (response.code != expected)
	{
		throw std::runtime_error("Unexpected SMTP response code: " + std::to_string(response.code)
								 + " expected: " + std::to_string(expected));
	}
}

} // namespace SMTP