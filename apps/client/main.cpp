#include <QApplication>

#include "Client.hpp"
#include "MainWindow.h"

int main(int argc, char* argv[])
{
	SMTP::Client::Init();
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	const std::string server_address{"localhost"};
	client->Connect(server_address, 465);

	QApplication app{argc, argv};
	UserInterface::MainWindow window{};
	window.show();

	QObject::connect(&app, &QApplication::aboutToQuit, [] { SMTP::Client::Shutdown(); });

	app.exec();
}
