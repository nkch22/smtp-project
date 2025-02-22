#pragma once
#include <string>
#include <vector>

namespace SMTP
{
struct Mail
{
	std::string subject;		 // Subject of the email
	std::string from;			 // Sender's email address
	std::vector<std::string> to; // Recipients' email addresses
	std::string body;			 // Plain text body of the email

	// Constructor to initialize the basic fields of the email
	Mail(const std::string& subject, const std::string& from, const std::vector<std::string>& to,
		 const std::string& body) : subject(subject), from(from), to(to), body(body)
	{
	}
};
} // namespace SMTP