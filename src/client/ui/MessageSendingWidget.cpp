#include "MessageSendingWidget.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include "ClientGlobalVariables.inl"
#include "UiGlobalVariables.inl"

MessageSendingWidget::MessageSendingWidget(QWidget* parent) : QWidget{parent}
{
	m_email_validator = new QRegularExpressionValidator{Regexes::G_EMAIL_REGEX, this};

	const QPointer main_layout{new QVBoxLayout{this}};
	const QPointer form_layout{new QFormLayout};
	const QPointer email_group_box{new QGroupBox{"Email", this}};
	email_group_box->setLayout(form_layout);
	main_layout->addWidget(email_group_box);

	const QPointer from_line_edit{new QLineEdit{email_group_box}};
	connect(from_line_edit, &QLineEdit::editingFinished, this, &MessageSendingWidget::OnEmailAddressEditingFinished);
	connect(from_line_edit, &QLineEdit::textEdited, this, &MessageSendingWidget::OnEmailAddressEdited);

	const QPointer to_line_edit{new QLineEdit{email_group_box}};
	connect(to_line_edit, &QLineEdit::editingFinished, this, &MessageSendingWidget::OnEmailAddressEditingFinished);
	connect(to_line_edit, &QLineEdit::textEdited, this, &MessageSendingWidget::OnEmailAddressEdited);

	const QPointer subject_line_edit{new QLineEdit{email_group_box}};
	const QPointer body_text_edit{new QTextEdit{email_group_box}};

	const QPointer send_button{new QPushButton{"Send mail", email_group_box}};
	connect(send_button, &QPushButton::clicked, this, &MessageSendingWidget::OnSendButtonClicked);

	form_layout->addRow("From", from_line_edit);
	form_layout->addRow("To", to_line_edit);
	form_layout->addRow("Subject", subject_line_edit);
	form_layout->addRow("Body", body_text_edit);
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

QValidator::State MessageSendingWidget::GetEmailLineEditState(const class QLineEdit* line_edit,
															  const QRegularExpressionValidator* validator)
{
	int pos{};
	QString email_text{line_edit->text()};
	return validator->validate(email_text, pos);
}

void MessageSendingWidget::OnSendButtonClicked() {}
