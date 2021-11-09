#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow* ui;

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
public:

};

#endif // MAINWINDOW_H
