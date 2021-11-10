#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class QPlainTextEdit;

#include "Debugger.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow* ui;

	QString lastDirectory;
	QString currentExecutable;

	QHash<QDockWidget*, QString> docks;

	Debugger						debugger;

	QPlainTextEdit*					logWidget;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
public:
private:
	void updateTitle();
	void loadExecutable(const QString& fileName);
	void unloadExecutable();

	void updateActions();

	void initToolbar();

	void initPanels();

	QDockWidget* createDockPanel(QWidget* widget, const QString& name, const QString& title, const char* rawTitle, bool visible, Qt::DockWidgetArea initialArea, Qt::DockWidgetAreas allowedAreas = Qt::AllDockWidgetAreas);

private:
	void closeEvent(QCloseEvent* e);
private slots:
	void slotAction();
	void slotDebugger_starting();
	void slotDebugger_stopped();
	void slotDebugger_log(const QString& s);
};

#endif // MAINWINDOW_H
