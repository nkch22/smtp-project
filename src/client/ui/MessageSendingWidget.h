#pragma once

#include <QPointer>
#include <QValidator>
#include <QWidget>

class MessageSendingWidget final : public QWidget
{
	Q_OBJECT

public:
	explicit MessageSendingWidget(QWidget* parent = nullptr);

private slots:
	// When email address LineEdit loses focus – no longer being edited
	void OnEmailAddressEditingFinished();

	// When email address LineEdit changes
	void OnEmailAddressEdited();

	void OnSendButtonClicked();

private:
	static QValidator::State GetEmailLineEditState(const class QLineEdit* line_edit,
												   const QRegularExpressionValidator* validator);

	QPointer<QRegularExpressionValidator> m_email_validator;
};
