#include "RegistersPanel.h"
#include "ui_RegistersPanel.h"
#include "Debugger.h"
#include "DebuggerHelper.h"

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
	QList<QTreeWidgetItem*> items;
#define ADD_REGISTER(name, value) \
	items.append(new QTreeWidgetItem(QStringList() << name << value));

#ifdef _AMD64_
	if(debugger->isWow64())
	{
		WOW64_CONTEXT* ctxt = debuggger->wow64Context;

		ADD_REGISTER("eax",		QString().sprintf("%08X", ctxt->Eax));
		ADD_REGISTER("ecx",		QString().sprintf("%08X", ctxt->Ecx));
		ADD_REGISTER("edx",		QString().sprintf("%08X", ctxt->Edx));
		ADD_REGISTER("ebx",		QString().sprintf("%08X", ctxt->Ebx));
		ADD_REGISTER("esp",		QString().sprintf("%08X", ctxt->Esp));
		ADD_REGISTER("ebp",		QString().sprintf("%08X", ctxt->Ebp));
		ADD_REGISTER("esi",		QString().sprintf("%08X", ctxt->Esi));
		ADD_REGISTER("edi",		QString().sprintf("%08X", ctxt->Edi));
		ADD_REGISTER("eip",		QString().sprintf("%08X", ctxt->Eip));

		ADD_REGISTER("dr0",		QString().sprintf("%016X", ctxt->Dr0));
		ADD_REGISTER("dr1",		QString().sprintf("%016X", ctxt->Dr1));
		ADD_REGISTER("dr2",		QString().sprintf("%016X", ctxt->Dr2));
		ADD_REGISTER("dr3",		QString().sprintf("%016X", ctxt->Dr3));
		ADD_REGISTER("dr6",		QString().sprintf("%016X", ctxt->Dr6));
		ADD_REGISTER("dr7",		QString().sprintf("%016X", ctxt->Dr7));

		ADD_REGISTER("cs",		QString().sprintf("%04X", ctxt->SegCs));
		ADD_REGISTER("ds",		QString().sprintf("%04X", ctxt->SegDs));
		ADD_REGISTER("es",		QString().sprintf("%04X", ctxt->SegEs));
		ADD_REGISTER("fs",		QString().sprintf("%04X", ctxt->SegFs));
		ADD_REGISTER("gs",		QString().sprintf("%04X", ctxt->SegGs));
		ADD_REGISTER("ss",		QString().sprintf("%04X", ctxt->SegSs));
		ADD_REGISTER("eflags",	QString().sprintf("%08X", ctxt->EFlags));
	}
	else
	{
		CONTEXT* ctxt = &debugger->context;

		//
		// Integer registers.
		//
		ADD_REGISTER("rax",		QString().sprintf("%016X", ctx->Rax));
		ADD_REGISTER("rcx",		QString().sprintf("%016X", ctx->Rcx));
		ADD_REGISTER("rdx",		QString().sprintf("%016X", ctx->Rdx));
		ADD_REGISTER("rbx",		QString().sprintf("%016X", ctx->Rbx));
		ADD_REGISTER("rsp",		QString().sprintf("%016X", ctx->Rsp));
		ADD_REGISTER("rbp",		QString().sprintf("%016X", ctx->Rbp));
		ADD_REGISTER("rsi",		QString().sprintf("%016X", ctx->Rsi));
		ADD_REGISTER("rdi",		QString().sprintf("%016X", ctx->Rdi));
		ADD_REGISTER("r8",		QString().sprintf("%016X", ctx->R8));
		ADD_REGISTER("r9",		QString().sprintf("%016X", ctx->R9));
		ADD_REGISTER("r10",		QString().sprintf("%016X", ctx->R10));
		ADD_REGISTER("r11",		QString().sprintf("%016X", ctx->R11));
		ADD_REGISTER("r12",		QString().sprintf("%016X", ctx->R12));
		ADD_REGISTER("r13",		QString().sprintf("%016X", ctx->R13));
		ADD_REGISTER("r14",		QString().sprintf("%016X", ctx->R14));
		ADD_REGISTER("r15",		QString().sprintf("%016X", ctx->R15));

		//
		// Debug registers
		//
		ADD_REGISTER("dr0",		QString().sprintf("%016X", ctxt->Dr0));
		ADD_REGISTER("dr1",		QString().sprintf("%016X", ctxt->Dr1));
		ADD_REGISTER("dr2",		QString().sprintf("%016X", ctxt->Dr2));
		ADD_REGISTER("dr3",		QString().sprintf("%016X", ctxt->Dr3));
		ADD_REGISTER("dr6",		QString().sprintf("%016X", ctxt->Dr6));
		ADD_REGISTER("dr7",		QString().sprintf("%016X", ctxt->Dr7));

		//
		// Segment Registers and processor flags.
		//
		ADD_REGISTER("cs",		QString().sprintf("%08X", ctxt->SegCs));
		ADD_REGISTER("ss",		QString().sprintf("%08X", ctxt->SegSs));
		ADD_REGISTER("ds",		QString().sprintf("%08X", ctxt->SegDs));
		ADD_REGISTER("es",		QString().sprintf("%08X", ctxt->SegEs));
		ADD_REGISTER("gs",		QString().sprintf("%08X", ctxt->SegGs));
		ADD_REGISTER("fs",		QString().sprintf("%08X", ctxt->SegFs));

		ADD_REGISTER("cs",		QString().sprintf("%04X", ctxt->SegCs));
		ADD_REGISTER("ds",		QString().sprintf("%04X", ctxt->SegDs));
		ADD_REGISTER("es",		QString().sprintf("%04X", ctxt->SegEs));
		ADD_REGISTER("fs",		QString().sprintf("%04X", ctxt->SegFs));
		ADD_REGISTER("gs",		QString().sprintf("%04X", ctxt->SegGs));
		ADD_REGISTER("ss",		QString().sprintf("%04X", ctxt->SegSs));
		ADD_REGISTER("eflags",	QString().sprintf("%08X", ctxt->EFlags));
	}
#else
	CONTEXT* ctxt = &debugger->context;

	ADD_REGISTER("eax",		QString().sprintf("%08X", ctxt->Eax));
	ADD_REGISTER("ecx",		QString().sprintf("%08X", ctxt->Ecx));
	ADD_REGISTER("edx",		QString().sprintf("%08X", ctxt->Edx));
	ADD_REGISTER("ebx",		QString().sprintf("%08X", ctxt->Ebx));
	ADD_REGISTER("esp",		QString().sprintf("%08X", ctxt->Esp));
	ADD_REGISTER("ebp",		QString().sprintf("%08X", ctxt->Ebp));
	ADD_REGISTER("esi",		QString().sprintf("%08X", ctxt->Esi));
	ADD_REGISTER("edi",		QString().sprintf("%08X", ctxt->Edi));
	ADD_REGISTER("eip",		QString().sprintf("%08X", ctxt->Eip));

	ADD_REGISTER("dr0",		QString().sprintf("%016X", ctxt->Dr0));
	ADD_REGISTER("dr1",		QString().sprintf("%016X", ctxt->Dr1));
	ADD_REGISTER("dr2",		QString().sprintf("%016X", ctxt->Dr2));
	ADD_REGISTER("dr3",		QString().sprintf("%016X", ctxt->Dr3));
	ADD_REGISTER("dr6",		QString().sprintf("%016X", ctxt->Dr6));
	ADD_REGISTER("dr7",		QString().sprintf("%016X", ctxt->Dr7));

	ADD_REGISTER("cs",		QString().sprintf("%04X", ctxt->SegCs));
	ADD_REGISTER("ds",		QString().sprintf("%04X", ctxt->SegDs));
	ADD_REGISTER("es",		QString().sprintf("%04X", ctxt->SegEs));
	ADD_REGISTER("fs",		QString().sprintf("%04X", ctxt->SegFs));
	ADD_REGISTER("gs",		QString().sprintf("%04X", ctxt->SegGs));
	ADD_REGISTER("ss",		QString().sprintf("%04X", ctxt->SegSs));
	ADD_REGISTER("eflags",	QString().sprintf("%08X", ctxt->EFlags));
#endif

	ui->treeWidget->addTopLevelItems(items);

#undef ADD_REGISTER
}

void RegistersPanel::slotDebugger_continue()
{
	ui->treeWidget->clear();
}
