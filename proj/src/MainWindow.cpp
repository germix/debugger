#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileInfo>
#include <QFileDialog>

#define TITLE "Debugger"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
		updateTitle();
	}
}

void MainWindow::unloadExecutable()
{
	currentExecutable.clear();
	updateTitle();
}

void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionLoadExecutable)
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
			lastDirectory = QFileInfo(fileName).absoluteDir().absolutePath();
		}
	}
}
