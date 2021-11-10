#include "RegistersPanel.h"
#include "ui_RegistersPanel.h"
#include "Debugger.h"

RegistersPanel::RegistersPanel(Debugger* dbg, QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::RegistersPanel)
	, debugger(dbg)
{
	ui->setupUi(this);

	connect(dbg, SIGNAL(onStarting()), this, SLOT(slotDebugger_started()));
	connect(dbg, SIGNAL(onStopped()), this, SLOT(slotDebugger_stopped()));
	connect(dbg, SIGNAL(onBreak()), this, SLOT(slotDebugger_break()));
	connect(dbg, SIGNAL(onContinue()), this, SLOT(slotDebugger_continue()));
}

RegistersPanel::~RegistersPanel()
{
	delete ui;
}

void RegistersPanel::slotDebugger_started()
{
	ui->treeWidget->clear();
}

void RegistersPanel::slotDebugger_stopped()
{
	ui->treeWidget->clear();
}

void RegistersPanel::slotDebugger_break()
{
	CONTEXT* ctxt = &debugger->context;
	QList<QTreeWidgetItem*> items;

	items.append(new QTreeWidgetItem(QStringList() << "eax" << QString().sprintf("%08X", ctxt->Eax)));
	items.append(new QTreeWidgetItem(QStringList() << "ecx" << QString().sprintf("%08X", ctxt->Ecx)));
	items.append(new QTreeWidgetItem(QStringList() << "edx" << QString().sprintf("%08X", ctxt->Edx)));
	items.append(new QTreeWidgetItem(QStringList() << "ebx" << QString().sprintf("%08X", ctxt->Ebx)));
	items.append(new QTreeWidgetItem(QStringList() << "esp" << QString().sprintf("%08X", ctxt->Esp)));
	items.append(new QTreeWidgetItem(QStringList() << "ebp" << QString().sprintf("%08X", ctxt->Ebp)));
	items.append(new QTreeWidgetItem(QStringList() << "esi" << QString().sprintf("%08X", ctxt->Esi)));
	items.append(new QTreeWidgetItem(QStringList() << "edi" << QString().sprintf("%08X", ctxt->Edi)));
	items.append(new QTreeWidgetItem(QStringList() << "eip" << QString().sprintf("%08X", ctxt->Eip)));
	items.append(new QTreeWidgetItem(QStringList() << "eflags" << QString().sprintf("%08X", ctxt->EFlags)));
	items.append(new QTreeWidgetItem(QStringList() << "cs" << QString().sprintf("%08X", ctxt->SegCs)));
	items.append(new QTreeWidgetItem(QStringList() << "ss" << QString().sprintf("%08X", ctxt->SegSs)));
	items.append(new QTreeWidgetItem(QStringList() << "ds" << QString().sprintf("%08X", ctxt->SegDs)));
	items.append(new QTreeWidgetItem(QStringList() << "es" << QString().sprintf("%08X", ctxt->SegEs)));
	items.append(new QTreeWidgetItem(QStringList() << "gs" << QString().sprintf("%08X", ctxt->SegGs)));
	items.append(new QTreeWidgetItem(QStringList() << "fs" << QString().sprintf("%08X", ctxt->SegFs)));

	ui->treeWidget->addTopLevelItems(items);
}

void RegistersPanel::slotDebugger_continue()
{
	ui->treeWidget->clear();
}
