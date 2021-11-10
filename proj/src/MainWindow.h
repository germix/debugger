#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class RecentFilesMenu;
class QPlainTextEdit;
class RegistersPanel;

#include "Debugger.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow* ui;

	QString lastDirectory;
	QString currentExecutable;

	QHash<QDockWidget*, QString> docks;

	RecentFilesMenu*				recentFiles;

	Debugger						debugger;

	QPlainTextEdit*					logWidget;
	RegistersPanel*					registersPanel;

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
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);
private slots:
	void slotAction();

	void slotDebugger_log(const QString& s);
	void slotDebugger_starting();
	void slotDebugger_stopped();
	void slotDebugger_break();
	void slotDebugger_continue();

	void slotRecentFiles_fileTriggered(const QString& fileName);

};

#endif // MAINWINDOW_H
