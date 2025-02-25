/**
 * @file Command.h
 * @author Daniil
 * @brief Provides SMTP command creation functions.
 * @version 0.1
 * @date 2025-02-23
 *
 * This file defines the `Command` class, which contains static methods to
 * generate various SMTP commands. These commands are used to communicate
 * with an SMTP server for sending emails and managing the connection.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once
#include <iostream>
#include <sstream>

#include "Mail.hpp"

namespace SMTP
{

/**
 * @brief Class that generates SMTP commands.
 *
 * This class provides static methods to create various SMTP commands,
 * such as HELO, EHLO, MAIL FROM, RCPT TO, DATA, STARTTLS, and QUIT.
 * These commands are used to communicate with an SMTP server for
 * sending email messages and managing the connection.
 */
class Command
{
public:
	/**
	 * @brief Generates the HELO command.
	 *
	 * This command is used to initiate communication with the SMTP server.
	 *
	 * @param domain The domain of the sender.
	 * @return The HELO command as a string.
	 */
	static std::string HELO(const std::string& domain) { return "HELO " + domain + "\r\n"; }

	/**
	 * @brief Generates the EHLO command.
	 *
	 * This command is used to initiate communication with the SMTP server
	 * and request extended SMTP capabilities.
	 *
	 * @param domain The domain of the sender.
	 * @return The EHLO command as a string.
	 */
	static std::string EHLO(const std::string& domain) { return "EHLO " + domain + "\r\n"; }

	/**
	 * @brief Generates the MAIL FROM command.
	 *
	 * This command is used to specify the sender's email address.
	 *
	 * @param email The sender's email address.
	 * @return The MAIL FROM command as a string.
	 */
	static std::string MAIL_FROM(const std::string& email) { return "MAIL FROM:<" + email + ">\r\n"; }

	/**
	 * @brief Generates the RCPT TO command.
	 *
	 * This command is used to specify the recipient's email address.
	 *
	 * @param email The recipient's email address.
	 * @return The RCPT TO command as a string.
	 */
	static std::string RCPT_TO(const std::string& email) { return "RCPT TO:<" + email + ">\r\n"; }

	/**
	 * @brief Generates the DATA command.
	 *
	 * This command is used to initiate the process of sending the email body.
	 *
	 * @return The DATA command as a string.
	 */
	static std::string DATA() { return "DATA\r\n"; }

	/**
	 * @brief Generates the STARTTLS command.
	 *
	 * This command is used to initiate a secure connection using TLS.
	 *
	 * @return The STARTTLS command as a string.
	 */
	static std::string TLSS() { return "STARTTLS\r\n"; }

	/**
	 * @brief Generates the MAIL command with email data.
	 *
	 * This command is used to send an email message, including the sender,
	 * recipients, subject, and body.
	 *
	 * @param mail The `Mail` object containing the email details.
	 * @return The MAIL command as a string.
	 */
	static std::string MAIL(const Mail& mail)
	{
		std::ostringstream ss;
		ss << "From: <" << mail.from << ">\r\n";
		ss << "Subject: " << mail.subject << "\r\n";
		for (const auto& recipient : mail.to) ss << "To: <" << recipient << ">\r\n";
		ss << "\r\n" << mail.body << "\r\n.\r\n";
		return ss.str();
	}

	/**
	 * @brief Generates the QUIT command.
	 *
	 * This command is used to terminate the SMTP session.
	 *
	 * @return The QUIT command as a string.
	 */
	static std::string QUIT() { return "QUIT\r\n"; }
};

} // namespace SMTP
