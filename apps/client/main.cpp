#include <QApplication>

#include "Client.hpp"
#include "MainWindow.h"

int main(int argc, char* argv[])
{
	SMTP::Client::Init();
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	client->set_username("username");
	client->set_password("password");

	QApplication app{argc, argv};
	UserInterface::MainWindow window{};
	window.show();

	QObject::connect(&app, &QApplication::aboutToQuit, [] { SMTP::Client::Shutdown(); });

	app.exec();
}
