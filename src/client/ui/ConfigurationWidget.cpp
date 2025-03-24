#include "ConfigurationWidget.h"

#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPointer>
#include <QPushButton>

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

	m_user_line_edit = new QLineEdit{configuration_group_box};
	m_password_line_edit = new QLineEdit{configuration_group_box};
	m_password_line_edit->setEchoMode(QLineEdit::Password);

	const QPointer login_button{new QPushButton{"Login", configuration_group_box}};
	connect(login_button, &QPushButton::clicked, this, &ConfigurationWidget::OnLoginButtonClicked);
	const QPointer register_button{new QPushButton{"Register", configuration_group_box}};
	connect(register_button, &QPushButton::clicked, this, &ConfigurationWidget::OnRegisterButtonClicked);

	form_layout->addRow("Authentication method", auth_method_edit);
	form_layout->addRow("SMTP User", m_user_line_edit);
	form_layout->addRow("Password", m_password_line_edit);
	form_layout->addRow(login_button);
	form_layout->addRow(register_button);
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

void ConfigurationWidget::OnLoginButtonClicked()
{
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	const std::string username{m_user_line_edit->text().toUtf8().constData()};
	const std::string password{m_password_line_edit->text().toUtf8().constData()};
	if (client->Login(username, password))
	{
		Q_EMIT OnSuccessfulLogin();
	}
}

void ConfigurationWidget::OnRegisterButtonClicked()
{
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	const std::string username{m_user_line_edit->text().toUtf8().constData()};
	const std::string password{m_password_line_edit->text().toUtf8().constData()};
	if (client->Register(username, password))
	{
		[[likely]] if (client->Login(username, password))
		{
			Q_EMIT OnSuccessfulLogin();
		}
	}
}
