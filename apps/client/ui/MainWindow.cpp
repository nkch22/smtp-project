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

	m_configuration_widget = new ConfigurationWidget{central_widget};
	m_message_sending_widget = new MessageSendingWidget{central_widget};
	m_message_sending_widget->setEnabled(false);

	connect(m_configuration_widget, &ConfigurationWidget::OnSuccessfulLogin, this, &MainWindow::OnSuccessfulLogin);

	layout->addWidget(m_configuration_widget);
	layout->addWidget(m_message_sending_widget);
}

void MainWindow::OnSuccessfulLogin()
{
	m_configuration_widget->setEnabled(false);
	m_message_sending_widget->setEnabled(true);
}
