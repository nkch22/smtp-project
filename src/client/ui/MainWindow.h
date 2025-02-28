/**
 * @file MainWindow.h
 * @author Vitaliy
 * @brief MainWindow class that represents SMTP client GUI
 * @version 0.1
 * @date 2025-02-27
 *
 * This file defines the `MainWindow` class, which represents user interface
 * for SMTP client and contains other widgets with which user can send messages.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <QMainWindow>

namespace UserInterface
{
/**
 * @brief Main UI window which contains other widgets
 *
 * MainWindow class represents user interface for SMTP client
 * and contains other widgets with which user can send messages.
 */
class MainWindow final : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
};
} // namespace UserInterface
