#pragma once

#include <QMainWindow>
#include <QValidator>

class QGroupBox;
class QGridLayout;
class QLabel;
class QLineEdit;

inline static const QRegularExpression G_EMAIL_REGEX{R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)",
													 QRegularExpression::CaseInsensitiveOption};

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

	static QValidator::State GetEmailLineEditState(const QLineEdit* line_edit,
												   const QRegularExpressionValidator* validator);

	const QString VALID_EMAIL_STYLE_SHEET{""};
	const QString INVALID_EMAIL_STYLE_SHEET{"QLineEdit { background-color: #FFCCCC; border: 2px solid red; }"};

	QRegularExpressionValidator* m_email_validator;

	QGridLayout* m_layout;

	QLineEdit* m_user_line_edit;
	QLineEdit* m_password_line_edit;
};
