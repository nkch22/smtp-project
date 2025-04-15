#include "AuthCommand.hpp"

#include <cassert>
#include <format>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "Utils.hpp"

namespace SMTP
{

namespace Protocol
{

AuthCommand::AuthCommand(
	const AuthentificationMethod auth_method, 
	const std::optional<std::string> encoded_username_password
)
	: m_auth_method{auth_method}
	, m_username{TryReceiveUsername(auth_method, encoded_username_password)}
	, m_password{TryReceivePassword(auth_method, encoded_username_password)}
{
}

AuthCommand::AuthCommand(
	const AuthentificationMethod auth_method, 
	const std::optional<std::string> encoded_username, 
	const std::optional<std::string> encoded_password
)
	: m_auth_method{auth_method}
	, m_username{TryReceiveUsername(auth_method, encoded_username)}
	, m_password{TryReceivePassword(auth_method, encoded_password)}
{
}

Response AuthCommand::Execute(Context& options)
{
	using enum AuthentificationMethod;

	switch(m_auth_method)
	{
	case Plain:
		return DoPlain(options);
		break;
	case Login:
		return DoLogin(options);
		break;
	default:
		throw std::runtime_error{"Undefined AuthentificationMethod member!!!"};
	}
	
	assert(false && "This should never happen");
	
	const Response response{ReplyCode::Ok};
	return response;
}

OptionalCommand AuthCommand::TryParseCommand(
	const std::string& request, const Context& options
)
{
	const auto found_auth{ToUpper(request).find(COMMAND)};

	OptionalCommand command{std::nullopt};

	if(found_auth != std::string::npos)
	{
		command = TryParseCommandStart(found_auth, request, options);
	}
	else if(options.waiting_auth.has_value())
	{
		command = TryParseCommandContinuation(request, options);
	}
	
	return command;
}

OptionalCommand AuthCommand::TryParseCommandStart(
	const std::size_t found_auth,
	const std::string& request, 
	const Context& options
)
{
	const auto plain_str{std::format("{}", AuthentificationMethod::Plain)};
	const auto login_str{std::format("{}", AuthentificationMethod::Login)};

	const auto auth_method{
		request.substr(found_auth + std::size(COMMAND) + 1)
	};

	if(const auto found_plain{ToUpper(auth_method).find(plain_str)};
		found_plain != std::string::npos
	)
	{
		const auto username_password{RemoveCRLF(
			auth_method.substr(found_plain + std::size(plain_str) + 1)
		)};

		if(!std::ranges::any_of(username_password, ::isalnum))
		{
			return std::make_unique<AuthCommand>(
				AuthentificationMethod::Plain, 
				std::nullopt
			);
		}

		return std::make_unique<AuthCommand>(
			AuthentificationMethod::Plain, 
			username_password
		);
	}
	else if(const auto found_login{ToUpper(auth_method).find(login_str)};
		found_login != std::string::npos
	)
	{
		return std::make_unique<AuthCommand>(
			AuthentificationMethod::Login, std::nullopt, std::nullopt
		);
	}

	return std::nullopt;
}

OptionalCommand AuthCommand::TryParseCommandContinuation(
	const std::string& request, 
	const Context& options
)
{
	assert(options.waiting_auth.has_value() && "This should never happen");
	const auto auth_value{options.waiting_auth.value()};

	if(auth_value == AuthentificationMethod::Login)
	{
		if(options.is_auth_login_username_received)
		{
			return std::make_unique<AuthCommand>(
				auth_value, std::nullopt, RemoveCRLF(request)
			);
		}

		return std::make_unique<AuthCommand>(
			auth_value, RemoveCRLF(request), std::nullopt
		);
	}
	else if(auth_value == AuthentificationMethod::Plain)
	{
		return std::make_unique<AuthCommand>(auth_value, RemoveCRLF(request));
	}
	return std::nullopt;
}

Response AuthCommand::DoPlain(Context& options)
{
	if(m_username.has_value() && 
		m_password.has_value())
	{
		const Response response{ReplyCode::AuthenticationSuccessful, "Authentification Successful"};
		CreateUserIfNot(options);
		return response;
	}
	const Response response{ReplyCode::ContinueWithAuth};
	return response;
}

Response AuthCommand::DoLogin(Context& options)
{
	if(!m_username.has_value() && 
		m_password.has_value())
	{
		const Response response{ReplyCode::AuthenticationSuccessful, "Authentification Successful"};
		CreateUserIfNot(options);
		return response;
	}
	else if(m_username.has_value() &&
			!m_password.has_value())
	{
		const Response response{ReplyCode::ContinueWithAuth, "Provide user password"};
		return response;
	}

	const Response response{ReplyCode::ContinueWithAuth, "Provide username"};
	return response;
}

bool AuthCommand::CreateUserIfNot(Context& options)
{
	using enum AuthentificationMethod;

	if(m_auth_method == Plain)
	{
		const auto read_user{options.database->ReadUser(m_username.value())};

		if(!read_user.has_value())
		{
			options.database->CreateUser(
				Mailbox{m_username.value()}, HashData(m_password.value())
			);
		}
	}
	else if(m_auth_method == Login)
	{
		const auto read_user{options.database->ReadUser(m_username.value())};

		if(!read_user.has_value())
		{
			options.database->CreateUser(
				Mailbox{m_username.value()}, ""
			);
		}
		else if(m_password.has_value())
		{
			const auto& user{read_user.value()};
			options.database->UpdateUser(
				user->GetID(),
				user->GetName(),
				HashData(m_password.value())
			);
		}
	}

	return false;
}

std::optional<std::string> AuthCommand::TryReceiveUsername(
		const AuthentificationMethod auth_method, 
		const std::optional<std::string> encoded_username
)
{
	using enum AuthentificationMethod;

	constexpr static char terminator{'\0'};

	if(!encoded_username.has_value())
	{
		return std::nullopt;
	}

	const auto decoded{DecodeBase64(encoded_username.value())};

	if(auth_method == Plain)
	{
		const auto first_terminator{decoded.find(terminator)};
		const auto remaining{decoded.substr(first_terminator + 1)};
		const auto last_terminator{remaining.find(terminator)};
		const auto username{remaining.substr(0, last_terminator)};
		return username;
	}

	return decoded;
}

std::optional<std::string> AuthCommand::TryReceivePassword(
        const AuthentificationMethod auth_method, 
		const std::optional<std::string> encoded_password
)
{
	using enum AuthentificationMethod;

	constexpr static char terminator{'\0'};

	if(!encoded_password.has_value())
	{
		return std::nullopt;
	}

	const auto decoded{DecodeBase64(encoded_password.value())};

	if(auth_method == Plain)
	{
		const auto first_terminator{decoded.find(terminator)};
		const auto remaining{decoded.substr(first_terminator + 1)};
		const auto last_terminator{remaining.find(terminator)};
		const auto password{remaining.substr(last_terminator + 1)};
		return password;
	}

	return decoded;
}

}

}