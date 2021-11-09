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

	QString lastDirectory;
	QString currentExecutable;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
public:
private:
	void updateTitle();
	void loadExecutable(const QString& fileName);
	void unloadExecutable();
private slots:
	void slotAction();
};

#endif // MAINWINDOW_H
