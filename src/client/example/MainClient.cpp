#include "Client.hpp"

int main()
{
	SMTP::Client client;
	client.Init();
	client.Connect("127.0.0.1", 2525);
	SMTP::Mail mail("Hello", "Daniil", {"Daniil"}, "Hello, world!");
	client.SendMail(mail);

	while (true)
	{
		/* code */
	}

	return 1;
}