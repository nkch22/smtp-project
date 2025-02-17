#pragma once

#include <QMainWindow>
#include <QRegularExpression>

class QGroupBox;
class QGridLayout;
class QLabel;
class QLineEdit;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
	void OnAuthCheckBoxStateChanged(Qt::CheckState NewCheckState);

	// When email address LineEdit loses focus – no longer being edited
	void OnEmailAddressEditingFinished();

	// When email address LineEdit changes
	void OnEmailAddressEdited();

	void OnSendButtonClicked();

private:
	void SetupConfigurationWidgets();

	void SetupMessageWidgets();

	const QString VALID_EMAIL_STYLE_SHEET{""};
	const QString INVALID_EMAIL_STYLE_SHEET{"QLineEdit { background-color: #FFCCCC; border: 2px solid red; }"};
	QRegularExpression m_email_regex;
	class QRegularExpressionValidator* m_email_validator;

	QGridLayout* m_layout;

	QLineEdit* m_user_line_edit;
	QLineEdit* m_password_line_edit;
};
