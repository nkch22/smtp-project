#include "ConfigurationWidget.h"

#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPointer>

#include "Client.hpp"

using namespace UserInterface;

static const QString AuthLoginString{"LOGIN"};
static const QString AuthPlainString{"PLAIN"};

ConfigurationWidget::ConfigurationWidget(QWidget* parent) : QWidget{parent}
{
	const QPointer main_layout{new QVBoxLayout{this}};
	const QPointer form_layout{new QFormLayout};
	const QPointer configuration_group_box{new QGroupBox{"SMTP Configuration", this}};
	configuration_group_box->setLayout(form_layout);
	main_layout->addWidget(configuration_group_box);

	const QPointer auth_method_edit{new QComboBox{configuration_group_box}};
	auth_method_edit->addItems(QStringList{AuthLoginString, AuthPlainString});
	connect(auth_method_edit, &QComboBox::currentTextChanged, this,
			&ConfigurationWidget::OnAuthMethodComboBoxTextChanged);

	const QPointer user_line_edit{new QLineEdit{configuration_group_box}};
	const QPointer password_line_edit{new QLineEdit{configuration_group_box}};
	password_line_edit->setEchoMode(QLineEdit::Password);

	form_layout->addRow("Authentication method", auth_method_edit);
	form_layout->addRow("SMTP User", user_line_edit);
	form_layout->addRow("Password", password_line_edit);
}

void ConfigurationWidget::OnAuthMethodComboBoxTextChanged(const QString& text)
{
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	std::unique_ptr<SMTP::IAuthenticator> authenticator;
	if (comparesEqual(text, AuthLoginString))
	{
		authenticator = std::make_unique<SMTP::AuthLogin>();
	}
	else if (comparesEqual(text, AuthPlainString))
	{
		authenticator = std::make_unique<SMTP::AuthPlain>();
	}
	else
		return;
	client->set_authenticator(std::move(authenticator));
}
