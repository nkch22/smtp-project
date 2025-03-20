/**
 * @file ConfigurationWidget.h
 * @author Vitaliy
 * @brief ConfigurationWidget class that contains SMTP configuration parameters
 * @version 0.1
 * @date 2025-02-27
 *
 * This file defines the `ConfigurationWidget` class, which represents SMTP configuration
 * parameters for server connection.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <QWidget>

namespace UserInterface
{
/**
 * @brief Configuration Widget which contains SMTP connection parameters
 */
class ConfigurationWidget final : public QWidget
{
	Q_OBJECT

public:
	explicit ConfigurationWidget(QWidget* parent = nullptr);

private slots:
	/**
	 * @brief Executed when authentication combo box changes its value
	 */
	void OnAuthMethodComboBoxTextChanged(const QString& text);
};
} // namespace UserInterface
