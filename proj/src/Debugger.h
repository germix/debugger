#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <QThread>
#include <QHash>
#include <windows.h>

class Debugger : public QThread
{
	Q_OBJECT

	QString					program;

	HANDLE					hProcess;
	DWORD					dwProcessId;
	QHash<LPVOID, QString>	dllNameMap;

	BOOL					bContinueDebugging;
public:
	BOOL					isProgramActive;
	bool					isProgramRunning;

public:
	explicit Debugger(QObject* parent = nullptr);
	~Debugger();
public:
	bool programRun(const QString& fileName);
	void programKill();
private:
	void run();
	DWORD OnExceptionDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnCreateThreadDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnCreateProcessDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnExitThreadDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnExitProcessDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnLoadDllDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnUnloadDllDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnOutputDebugStringEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnRipEvent(const DEBUG_EVENT& debugEvent);
signals:
	void starting();
	void stopped();
	void log(const QString& s);

public slots:
};

#endif // DEBUGGER_H
