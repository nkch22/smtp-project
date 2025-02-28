#include "MessageSendingWidget.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QTextEdit>

#include "Client.hpp"
#include "ClientGlobalVariables.inl"
#include "Mail.hpp"
#include "UiGlobalVariables.inl"

using namespace UserInterface;

MessageSendingWidget::MessageSendingWidget(QWidget* parent) : QWidget{parent}
{
	m_email_validator = new QRegularExpressionValidator{Regexes::G_EMAIL_REGEX, this};

	const QPointer main_layout{new QVBoxLayout{this}};
	const QPointer form_layout{new QFormLayout};
	const QPointer email_group_box{new QGroupBox{"Email", this}};
	email_group_box->setLayout(form_layout);
	main_layout->addWidget(email_group_box);

	m_from_line_edit = new QLineEdit{email_group_box};
	connect(m_from_line_edit, &QLineEdit::editingFinished, this, &MessageSendingWidget::OnEmailAddressEditingFinished);
	connect(m_from_line_edit, &QLineEdit::textEdited, this, &MessageSendingWidget::OnEmailAddressEdited);

	m_to_line_edit = new QLineEdit{email_group_box};
	connect(m_to_line_edit, &QLineEdit::editingFinished, this, &MessageSendingWidget::OnEmailAddressEditingFinished);
	connect(m_to_line_edit, &QLineEdit::textEdited, this, &MessageSendingWidget::OnEmailAddressEdited);

	m_subject_line_edit = new QLineEdit{email_group_box};
	m_body_text_edit = new QTextEdit{email_group_box};

	const QPointer send_button{new QPushButton{"Send mail", email_group_box}};
	connect(send_button, &QPushButton::clicked, this, &MessageSendingWidget::OnSendButtonClicked);

	form_layout->addRow("From", m_from_line_edit);
	form_layout->addRow("To", m_to_line_edit);
	form_layout->addRow("Subject", m_subject_line_edit);
	form_layout->addRow("Body", m_body_text_edit);
	form_layout->addRow(send_button);
}

void MessageSendingWidget::OnEmailAddressEditingFinished()
{
	const QPointer email_line_edit{qobject_cast<QLineEdit*>(sender())};
	Q_ASSERT(!email_line_edit.isNull());

	QValidator::State state{GetEmailLineEditState(email_line_edit, m_email_validator)};
	if (state == QValidator::Acceptable)
	{
		email_line_edit->setStyleSheet(LineEditStyles::G_VALID_EMAIL_STYLE_SHEET);
	}
	else
	{
		email_line_edit->setStyleSheet(LineEditStyles::G_INVALID_EMAIL_STYLE_SHEET);
	}
}

void MessageSendingWidget::OnEmailAddressEdited()
{
	const QPointer email_line_edit{qobject_cast<QLineEdit*>(sender())};
	Q_ASSERT(!email_line_edit.isNull());

	QValidator::State state{GetEmailLineEditState(email_line_edit, m_email_validator)};
	if (state == QValidator::Acceptable)
	{
		email_line_edit->setStyleSheet(LineEditStyles::G_VALID_EMAIL_STYLE_SHEET);
	}
}

QValidator::State MessageSendingWidget::GetEmailLineEditState(const QLineEdit* line_edit,
															  const QRegularExpressionValidator* validator)
{
	int pos{};
	QString email_text{line_edit->text()};
	return validator->validate(email_text, pos);
}

std::vector<std::string> MessageSendingWidget::GetRecipientsEmails() const
{
	std::vector<std::string> out_recipients{};

	const QString recipients{m_from_line_edit->text()};
	QStringList recipients_list{recipients.split(';', Qt::SkipEmptyParts, Qt::CaseInsensitive)};
	out_recipients.reserve(recipients_list.size());
	for (const QString& string : recipients_list)
	{
		out_recipients.push_back(string.toUtf8().constData());
	}

	return out_recipients;
}

void MessageSendingWidget::OnSendButtonClicked()
{
	SMTP::Client* client{SMTP::Client::get_instance()};
	assert(client);

	const std::string from{m_from_line_edit->text().toUtf8().constData()};
	const std::string to{m_to_line_edit->text().toUtf8().constData()};
	const std::string subject{m_subject_line_edit->text().toUtf8().constData()};
	const std::string body{m_body_text_edit->toPlainText().toUtf8().constData()};

	const std::vector recipients_array{GetRecipientsEmails()};
	SMTP::Mail mail{subject, from, recipients_array, body};
	client->SendMail(mail);
}
