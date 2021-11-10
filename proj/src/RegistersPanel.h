#ifndef REGISTERSPANEL_H
#define REGISTERSPANEL_H

#include <QWidget>

namespace Ui {
class RegistersPanel;
}
class Debugger;

class RegistersPanel : public QWidget
{
	Q_OBJECT

	Ui::RegistersPanel*		ui;
	Debugger*				debugger;
public:
	explicit RegistersPanel(Debugger* dbg, QWidget* parent = 0);
	~RegistersPanel();
private slots:
	void slotDebugger_started();
	void slotDebugger_stopped();
	void slotDebugger_break();
	void slotDebugger_continue();
};

#endif // REGISTERSPANEL_H
