#pragma once
#include <iostream>
#include <sstream>

#include "Mail.hpp"

namespace SMTP
{
class Command
{
public:
	static std::string HELO(const std::string& domain) { return "HELO " + domain + "\r\n"; }

	static std::string EHLO(const std::string& domain) { return "EHLO " + domain + "\r\n"; }

	static std::string MAIL_FROM(const std::string& email) { return "MAIL FROM:<" + email + ">\r\n"; }

	static std::string RCPT_TO(const std::string& email) { return "RCPT TO:<" + email + ">\r\n"; }

	static std::string DATA() { return "DATA\r\n"; }

	static std::string TLSS() { return "STARTTLS\r\n"; }

	static std::string MAIL(const Mail& mail)
	{
		std::ostringstream ss;
		ss << "From: <" << mail.from << ">\r\n";
		ss << "Subject: " << mail.subject << "\r\n";
		for (const auto& recipient : mail.to) ss << "To: <" << recipient << ">\r\n";
		ss << "\r\n" << mail.body << "\r\n.\r\n";
		return ss.str();
	}

	static std::string QUIT() { return "QUIT\r\n"; }
};
} // namespace SMTP