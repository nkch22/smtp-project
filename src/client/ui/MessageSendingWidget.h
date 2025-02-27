/**
 * @file ConfigurationWidget.h
 * @author Vitaliy
 * @brief MessageSendingWidget class that contains Message configuration
 * @version 0.1
 * @date 2025-02-27
 *
 * This file defines the `MessageSendingWidget` class, which represents Message configuration
 * parameters such as author, recepient and body.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <QPointer>
#include <QValidator>
#include <QWidget>

class QLineEdit;

namespace UI
{
/**
 * @brief Message Sending Widget which contains Message parameters
 */
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
	static QValidator::State GetEmailLineEditState(const QLineEdit* line_edit,
												   const QRegularExpressionValidator* validator);

	QPointer<QRegularExpressionValidator> m_email_validator;
};
} // namespace UI
