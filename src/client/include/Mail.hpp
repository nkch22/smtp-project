/**
 * @file Mail.h
 * @author Daniil
 * @brief Defines the structure for an email in the SMTP protocol.
 * @version 0.1
 * @date 2025-02-23
 *
 * This file defines the `Mail` struct, which is used to represent an email
 * message with its basic components, including subject, sender, recipients,
 * and body. It is useful for managing and sending emails using the SMTP protocol.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once
#include <string>
#include <vector>

namespace SMTP
{

/**
 * @brief Struct representing an email message.
 *
 * This struct contains the essential components of an email: subject, sender,
 * recipients, and body. It is used to store the email details before sending
 * via the SMTP protocol.
 */
struct Mail
{
	std::string subject;		 ///< The subject of the email.
	std::string from;			 ///< The sender's email address.
	std::vector<std::string> to; ///< List of recipient email addresses.
	std::string body;			 ///< The plain text body of the email.

	/**
	 * @brief Constructs a Mail object with the given details.
	 * @param subject The subject of the email.
	 * @param from The sender's email address.
	 * @param to The list of recipient email addresses.
	 * @param body The plain text body of the email.
	 */
	Mail(const std::string& subject, const std::string& from, const std::vector<std::string>& to,
		 const std::string& body) : subject(subject), from(from), to(to), body(body)
	{
	}
};

} // namespace SMTP
