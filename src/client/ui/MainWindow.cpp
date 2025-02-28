#include "MainWindow.h"

#include <QVBoxLayout>

#include "ConfigurationWidget.h"
#include "MessageSendingWidget.h"

using namespace UserInterface;

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}
{
	setWindowTitle("SMTP Client");

	const QPointer central_widget{new QWidget{this}};
	setCentralWidget(central_widget);

	const QPointer layout{new QVBoxLayout{central_widget}};
	central_widget->setLayout(layout);

	const QPointer configuration_widget{new ConfigurationWidget{central_widget}};
	const QPointer message_sending_widget{new MessageSendingWidget{central_widget}};

	layout->addWidget(configuration_widget);
	layout->addWidget(message_sending_widget);
}
