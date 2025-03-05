#include <iostream>

#include "Client.hpp"

int main()
{
	SMTP::Client::Init();
	std::cout << "Client initialized" << std::endl;

	SMTP::Client* client = SMTP::Client::get_instance();
	client->set_username("username");
	client->set_password("password");

	client->Connect("127.0.0.1", 2525);
	SMTP::Mail mail("Hello", "Daniil", {"Daniil"}, "Hello, world!");
	client->SendMail(mail);

	SMTP::Client::Shutdown();

	return 1;
}