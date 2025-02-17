#include "MainWindow.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QTextEdit>
#include <QWidget>

#include "ClientGlobalVariables.inl"
#include "UiGlobalVariables.inl"

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}
{
	setWindowTitle("SMTP Client");

	m_email_validator = new QRegularExpressionValidator{G_EMAIL_REGEX, this};

	const QPointer central_widget{new QWidget{this}};
	setCentralWidget(central_widget);

	m_layout = new QGridLayout{central_widget};

	SetupConfigurationWidgets();
	SetupMessageWidgets();

	const QPointer send_button{new QPushButton{"Send mail", central_widget}};
	m_layout->addWidget(send_button, 6, 5);
	connect(send_button, &QPushButton::clicked, this, &MainWindow::OnSendButtonClicked);
}

void MainWindow::SetupConfigurationWidgets()
{
	const QPointer configuration_group{new QGroupBox{"SMTP Configuration", centralWidget()}};
	const QPointer configuration_layout{new QGridLayout{configuration_group}};
	const QPointer server_label{new QLabel{"SMTP Server", configuration_group}};
	const QPointer server_line_edit{new QLineEdit{configuration_group}};
	const QPointer port_label{new QLabel{"Port", configuration_group}};
	const QPointer port_line_edit{new QLineEdit{configuration_group}};
	const QPointer auth_check_box{new QCheckBox{"Use authentication", configuration_group}};
	connect(auth_check_box, &QCheckBox::checkStateChanged, this, &MainWindow::OnAuthCheckBoxStateChanged);
	const QPointer user_label{new QLabel{"SMTP User", configuration_group}};
	m_user_line_edit = new QLineEdit{configuration_group};
	m_user_line_edit->setEnabled(false);
	const QPointer password_label{new QLabel{"Password", configuration_group}};
	m_password_line_edit = new QLineEdit{configuration_group};
	m_password_line_edit->setEnabled(false);
	m_password_line_edit->setEchoMode(QLineEdit::Password);

	configuration_layout->addWidget(server_label, 0, 0);
	configuration_layout->addWidget(server_line_edit, 0, 1, 1, 3);
	configuration_layout->addWidget(port_label, 0, 4);
	configuration_layout->addWidget(port_line_edit, 0, 5);
	configuration_layout->addWidget(auth_check_box, 1, 0);
	configuration_layout->addWidget(user_label, 1, 1);
	configuration_layout->addWidget(m_user_line_edit, 1, 2, 1, 2);
	configuration_layout->addWidget(password_label, 1, 4);
	configuration_layout->addWidget(m_password_line_edit, 1, 5);

	configuration_group->setLayout(configuration_layout);
	m_layout->addWidget(configuration_group, 0, 0, 1, 6);
}

void MainWindow::OnAuthCheckBoxStateChanged(Qt::CheckState NewCheckState)
{
	const bool CHECKED{NewCheckState == Qt::CheckState::Checked};
	m_user_line_edit->setEnabled(CHECKED);
	m_password_line_edit->setEnabled(CHECKED);
}

void MainWindow::SetupMessageWidgets()
{
	const QPointer email_group{new QGroupBox{"Email", centralWidget()}};
	const QPointer email_layout{new QGridLayout{email_group}};
	const QPointer from_label{new QLabel{"From", email_group}};
	const QPointer from_line_edit{new QLineEdit{email_group}};
	connect(from_line_edit, &QLineEdit::editingFinished, this, &MainWindow::OnEmailAddressEditingFinished);
	connect(from_line_edit, &QLineEdit::textEdited, this, &MainWindow::OnEmailAddressEdited);
	const QPointer to_label{new QLabel{"To", email_group}};
	const QPointer to_line_edit{new QLineEdit{email_group}};
	connect(to_line_edit, &QLineEdit::editingFinished, this, &MainWindow::OnEmailAddressEditingFinished);
	connect(to_line_edit, &QLineEdit::textEdited, this, &MainWindow::OnEmailAddressEdited);
	const QPointer subject_label{new QLabel{"Subject", email_group}};
	const QPointer subject_line_edit{new QLineEdit{email_group}};
	const QPointer body_label{new QLabel{"Body", email_group}};
	const QPointer body_text_edit{new QTextEdit{email_group}};

	email_layout->addWidget(from_label, 2, 0);
	email_layout->addWidget(from_line_edit, 2, 1, 1, 5);
	email_layout->addWidget(to_label, 3, 0);
	email_layout->addWidget(to_line_edit, 3, 1, 1, 5);
	email_layout->addWidget(subject_label, 4, 0);
	email_layout->addWidget(subject_line_edit, 4, 1, 1, 5);
	email_layout->addWidget(body_label, 5, 0);
	email_layout->addWidget(body_text_edit, 5, 1, 2, 5);

	email_group->setLayout(email_layout);
	m_layout->addWidget(email_group, 1, 0, 1, 6);
}

void MainWindow::OnEmailAddressEditingFinished()
{
	const QPointer email_line_edit{qobject_cast<QLineEdit*>(sender())};
	Q_ASSERT(!email_line_edit.isNull());

	QValidator::State state{GetEmailLineEditState(email_line_edit, m_email_validator)};
	if (state == QValidator::Acceptable)
	{
		email_line_edit->setStyleSheet(G_VALID_EMAIL_STYLE_SHEET);
	}
	else
	{
		email_line_edit->setStyleSheet(G_INVALID_EMAIL_STYLE_SHEET);
	}
}

void MainWindow::OnEmailAddressEdited()
{
	const QPointer email_line_edit{qobject_cast<QLineEdit*>(sender())};
	Q_ASSERT(!email_line_edit.isNull());

	QValidator::State state{GetEmailLineEditState(email_line_edit, m_email_validator)};
	if (state == QValidator::Acceptable)
	{
		email_line_edit->setStyleSheet(G_VALID_EMAIL_STYLE_SHEET);
	}
}

QValidator::State MainWindow::GetEmailLineEditState(const QLineEdit* line_edit,
													const QRegularExpressionValidator* validator)
{
	int pos{};
	QString email_text{line_edit->text()};
	return validator->validate(email_text, pos);
}

void MainWindow::OnSendButtonClicked() {}
