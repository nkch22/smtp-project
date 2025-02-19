#pragma once

#include <QPointer>
#include <QWidget>

class QLineEdit;

class ConfigurationWidget final : public QWidget
{
	Q_OBJECT

public:
	explicit ConfigurationWidget(QWidget* parent = nullptr);

private slots:
	void OnAuthCheckBoxStateChanged(Qt::CheckState NewCheckState);

private:
	QPointer<QLineEdit> m_user_line_edit;
	QPointer<QLineEdit> m_password_line_edit;
};
