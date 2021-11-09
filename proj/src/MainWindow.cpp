#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>

#define TITLE "Debugger"
#define SETTINGS_APPLICATION "Debugger"
#define SETTINGS_ORGANIZATION "Germix"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	//
	// Restore state
	//
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());

	//
	// Load last executable
	//
	loadExecutable(s.value("Executable").toString());
}

MainWindow::~MainWindow()
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	// Save state
	s.setValue("WindowState", saveState());
	s.setValue("WindowGeometry", saveGeometry());

	delete ui;
}

void MainWindow::updateTitle()
{
	QString s;
	s = TITLE;
	if(!currentExecutable.isEmpty())
	{
		s += " - " + currentExecutable;
	}
	setWindowTitle(s);
}

void MainWindow::loadExecutable(const QString& fileName)
{
	QFileInfo fi(fileName);

	unloadExecutable();
	if(fi.exists())
	{
		currentExecutable = fileName;
		lastDirectory = QFileInfo(fileName).absoluteDir().absolutePath();

		updateTitle();
	}
}

void MainWindow::unloadExecutable()
{
	if(!currentExecutable.isEmpty())
	{
		// Clear executable
		currentExecutable.clear();

		// Update title
		updateTitle();
	}
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	// Save executable
	s.setValue("Executable", currentExecutable);
}

void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionExit)
	{
		close();
	}
	else if(action == ui->actionLoadExecutable)
	{
		QString filters;

		filters += tr("EXE files") + " (*.exe);;";

		QString fileName = QFileDialog::getOpenFileName(
					this,
					tr("Load executable"),
					lastDirectory,
					filters);
		if(!fileName.isEmpty())
		{
			loadExecutable(fileName);
		}
	}
}
