#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>
#include <QDockWidget>
#include <QPlainTextEdit>
#include <QDebug>
#include <QMessageBox>
#include <QMimeData>
#include "AboutDialog.h"
#include "RecentFilesMenu.h"
#include "RegistersPanel.h"

#define TITLE "Debugger"
#define SETTINGS_APPLICATION "Debugger"
#define SETTINGS_ORGANIZATION "Germix"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	initToolbar();
	initPanels();

	//
	// Recent files menu
	//
	recentFiles = new RecentFilesMenu(20, 10, tr("Recent files"), QIcon(":/icons/broom.png"));
	connect(recentFiles, SIGNAL(onFileTriggered(QString)), this, SLOT(slotRecentFiles_fileTriggered(QString)));
	recentFiles->restoreState(s.value("RecentFiles").toByteArray());
	ui->menu_File->insertMenu(ui->actionExit, recentFiles);
	ui->menu_File->insertSeparator(ui->actionExit);

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
	
	// Save recent files
	s.setValue("RecentFiles", recentFiles->saveState());

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

		// Remove from recent files
		recentFiles->removeFile(currentExecutable);

		// Update title and actions
		updateTitle();
		updateActions();
	}
}

void MainWindow::unloadExecutable()
{
	if(!currentExecutable.isEmpty())
	{
		// Add to recent files
		recentFiles->addFile(currentExecutable);

		// Clear executable
		currentExecutable.clear();

		// Update title and actions
		updateTitle();
		updateActions();
	}
}

void MainWindow::updateActions()
{
	bool hasProgram = !currentExecutable.isEmpty();
	bool isActive = debugger.isProgramActive;
	bool isRunning = debugger.isProgramRunning;

	ui->actionLoadExecutable->setEnabled(!isActive);
	ui->actionRun->setEnabled(hasProgram && !isActive);
	ui->actionStop->setEnabled(hasProgram && isActive);
	ui->actionInterrupt->setEnabled(hasProgram && isActive && isRunning);
	ui->actionContinue->setEnabled(hasProgram && isActive && !isRunning);
}

void MainWindow::initToolbar()
{
	ui->menu_View->addAction(ui->mainToolBar->toggleViewAction());
	ui->menu_View->addSeparator();
}

void MainWindow::initPanels()
{
	connect(&debugger, SIGNAL(onLog(QString)), this, SLOT(slotDebugger_log(QString)));
	connect(&debugger, SIGNAL(onStarting()), this, SLOT(slotDebugger_starting()));
	connect(&debugger, SIGNAL(onStopped()), this, SLOT(slotDebugger_stopped()));
	connect(&debugger, SIGNAL(onBreak()), this, SLOT(slotDebugger_break()));
	connect(&debugger, SIGNAL(onContinue()), this, SLOT(slotDebugger_continue()));

	//
	// Log panel
	//
	logWidget = new QPlainTextEdit();
	logWidget->setReadOnly(true);
	createDockPanel(logWidget, "log", tr("Log"), "log", true, Qt::BottomDockWidgetArea, Qt::AllDockWidgetAreas);

	//
	// Registers panel
	//
	registersPanel = new RegistersPanel(&debugger);
	createDockPanel(registersPanel, "registers", tr("Registers"), "registers", true, Qt::BottomDockWidgetArea, Qt::AllDockWidgetAreas);
}

QDockWidget* MainWindow::createDockPanel(QWidget* widget, const QString& name, const QString& title, const char* rawTitle, bool visible, Qt::DockWidgetArea initialArea, Qt::DockWidgetAreas allowedAreas)
{
	//
	// Create the dock widget
	//
	QDockWidget* dock = new QDockWidget(title, this);
	dock->setObjectName(name);
	dock->setWidget(widget);
	dock->setAllowedAreas(allowedAreas);
	if(!restoreDockWidget(dock))
	{
		dock->setVisible(visible);
		addDockWidget(initialArea, dock);
	}
	docks.insert(dock, rawTitle);

	//
	// Get the action and add it to the 'windows' menu
	//
	QAction* action = dock->toggleViewAction();

	ui->menu_View->addAction(action);

	return dock;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if(debugger.isProgramActive)
	{
		e->ignore();
		QMessageBox::information(this, tr("Info"), tr("Can't exit when the debugger is active"), QMessageBox::Ok);
		return;
	}
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	// Save executable
	s.setValue("Executable", currentExecutable);
}

void MainWindow::dropEvent(QDropEvent* e)
{
	QString s;
	const QList<QUrl> urls = e->mimeData()->urls();

	if(urls.size() == 1)
	{
		s = urls.at(0).toLocalFile();

		loadExecutable(s);

		e->acceptProposedAction();
	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
	if(e->mimeData()->hasFormat("text/uri-list"))
	{
		e->acceptProposedAction();
	}
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
	else if(action == ui->actionRun)
	{
		debugger.programRun(currentExecutable);
	}
	else if(action == ui->actionStop)
	{
		debugger.programKill();
	}
	else if(action == ui->actionInterrupt)
	{
		debugger.programBreak();
	}
	else if(action == ui->actionContinue)
	{
		debugger.programContinue();
	}
	else if(action == ui->actionAbout)
	{
		AboutDialog().exec();
	}
}

void MainWindow::slotDebugger_log(const QString& s)
{
	QTextCursor textCursor = QTextCursor(logWidget->document());
	textCursor.movePosition(QTextCursor::End);
	logWidget->setTextCursor(textCursor);
	logWidget->insertPlainText(s);
}

void MainWindow::slotDebugger_starting()
{
	logWidget->clear();
	updateActions();
}

void MainWindow::slotDebugger_stopped()
{
	updateActions();
}

void MainWindow::slotDebugger_break()
{
	updateActions();
}

void MainWindow::slotDebugger_continue()
{
	updateActions();
}

void MainWindow::slotRecentFiles_fileTriggered(const QString& fileName)
{
	loadExecutable(fileName);
}
