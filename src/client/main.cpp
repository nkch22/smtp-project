#include <QApplication>

#include "Client.hpp"
#include "MainWindow.h"

int main(int argc, char* argv[])
{
	// std::thread ui_thread{(
	// 	[&]
	// 	{

	// 	})};
	// ui_thread.detach();

	SMTP::Client::Init();
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	asio::ssl::context ssl_context = asio::ssl::context{asio::ssl::context::tlsv13_client};
	ssl_context.set_verify_mode(asio::ssl::verify_peer);
	ssl_context.load_verify_file("../tools/certificates/cert.pem");
	ssl_context.set_password_callback(
		[](const std::size_t max_length, const asio::ssl::context::password_purpose& purpose) -> std::string
		{ return "hello"; });

	client->set_context(std::move(ssl_context));
	client->set_username("username");
	client->set_password("password");
	client->Connect("127.0.0.1", 465);

	QApplication app{argc, argv};
	UserInterface::MainWindow window{};
	window.show();

	QObject::connect(&app, &QApplication::aboutToQuit, [] { SMTP::Client::Shutdown(); });

	app.exec();
}
