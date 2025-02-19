#include "ConfigurationWidget.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>

ConfigurationWidget::ConfigurationWidget(QWidget* parent) : QWidget{parent}
{
	const QPointer main_layout{new QVBoxLayout{this}};
	const QPointer form_layout{new QFormLayout};
	const QPointer configuration_group_box{new QGroupBox{"SMTP Configuration", this}};
	configuration_group_box->setLayout(form_layout);
	main_layout->addWidget(configuration_group_box);

	const QPointer server_line_edit{new QLineEdit{configuration_group_box}};
	const QPointer port_line_edit{new QLineEdit{configuration_group_box}};

	const QPointer auth_check_box{new QCheckBox{"Use authentication", configuration_group_box}};
	connect(auth_check_box, &QCheckBox::checkStateChanged, this, &ConfigurationWidget::OnAuthCheckBoxStateChanged);

	m_user_line_edit = new QLineEdit{configuration_group_box};
	m_user_line_edit->setEnabled(false);

	m_password_line_edit = new QLineEdit{configuration_group_box};
	m_password_line_edit->setEnabled(false);
	m_password_line_edit->setEchoMode(QLineEdit::Password);

	form_layout->addRow("SMTP Server", server_line_edit);
	form_layout->addRow("Port", port_line_edit);
	form_layout->addRow(auth_check_box);
	form_layout->addRow("SMTP User", m_user_line_edit);
	form_layout->addRow("Password", m_password_line_edit);
}

void ConfigurationWidget::OnAuthCheckBoxStateChanged(Qt::CheckState NewCheckState)
{
	const bool CHECKED{NewCheckState == Qt::CheckState::Checked};
	m_user_line_edit->setEnabled(CHECKED);
	m_password_line_edit->setEnabled(CHECKED);
}
