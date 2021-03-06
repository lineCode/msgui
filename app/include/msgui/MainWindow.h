#pragma once

#include "msgui/LogConfig.h"
#include "msgui/Project.h"
#include "msgui/Error.h"
#include "msgui/Frame.h"
#include "msgui/Backtrace.h"
#include "msgui/Callgraph.h"
#include "msgui/Log.h"

#include <msglib/CmdParser.h>
#include <msglib/Cmd.h>
#include <msglib/Process.h>
#include <mredit/Editor.h>
#include <history_line_edit.hpp>

#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>
#include <QTreeWidget>
#include <QListWidget>
#include <QMenu>

#include <log4qt/logger.h>

Q_DECLARE_METATYPE(msglib::cmd::frame_base::ptr)
Q_DECLARE_METATYPE(msglib::cmd::backtrace::ptr)
Q_DECLARE_METATYPE(msglib::cmd::call_graph::ptr)

namespace msgui {

class MainWindow : public QMainWindow
{
	Q_OBJECT
	LOG4QT_DECLARE_QCLASS_LOGGER
public:
	MainWindow(const QString &filename = "");
	~MainWindow();

	enum class cmdmode_t
	{
		inactive,
		immediate,
		metadebug,
		preprocessordebug,
	};

	void loadFile(const QString &fileName);
protected:
	void closeEvent(QCloseEvent *event) override;
private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void documentWasModified();

	// widget
	void cmdExecute(QString cmd);

	void onLog(const Log4Qt::LoggingEvent &);

	void showCode(const QString &code);
	void showFileAndLine(const QString &fileAndFile);
	void showFrame(msglib::cmd::base::ptr frame);
	void showBacktrace(msglib::cmd::backtrace::ptr backtrace);
	void showCallgraph(msglib::cmd::call_graph::ptr callgraph);
	void showFilenameList(msglib::cmd::filename_list_base::ptr filename_list);

	void openSourceFile(const QString &filename);
	void markSourceFile(int row, int col);

	// process
	void processRun();
	void processRestart();
	void processStateChanged(QProcess::ProcessState newState);

	void processSendCommand(const QJsonDocument &line);
	void processStandardOutput(const QString &line);
	void processStandardError(const QString &line);
	void processError(const QString &message);

	void processCommand(const QJsonDocument &cmd);
	void processPrompt(const QJsonDocument &cmd);

	// menus
	void menuFileSettings();
	void menuFileRecent();

	void menuProjectSettings();

	void menuInformationEnvironment();
	void menuInformationEnvironmentReload();
	void menuInformationEnvironmentSysincludes();
	void menuInformationEnvironmentMacroNames();
	void menuInformationEnvironmentMacros();

	void menuDebugStepOnStart();
	void menuDebugForwardtraceOnStart();
	void menuDebugStart();
	void menuDebugStop();
	void menuDebugContinue();
	void menuDebugStepInto();
	void menuDebugStepOver();
	void menuDebugStepOut();
	void menuDebugStepIntoBack();
	void menuDebugStepOverBack();
	void menuDebugBacktrace();
	void menuDebugForwardtrace();

	void menuHelpLog();
	void menuHelpAbout();

	void updateRecentFileActions();
private:
	void processCommandClass(msglib::cmd::base::ptr cc);

	void closeProject();
	void loadProject();

	void createProcess();

	void createActions();
	void createStatusBar();
	void createWidgets();
	void createDockedWidgets();

	void readSettings();
	void writeSettings();
	void readProjectSettings();
	void writeProjectSettings();
	bool maybeSave();
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);

	void setEnabledCmd(bool enabled);
	void setCmdMode(cmdmode_t mode);
	void updateEnabled();
	void initLog();

	struct SFileAndLine
	{
		QString filename;
		int row;
		int col;
	};
	SFileAndLine parseFileAndLine(const QString &fileAndLine);

	// widgets
	HistoryLineEdit *_cmd;
	QLabel *_cmdprompt;
	mredit::Editor *_editor;
	QString _editor_current_file;
	int _editor_bmgroup_showpos;

	Log *_log;

	QLabel *_st_cmdmode;
	QLabel *_st_process;

	Error *_error;
	Frame *_frame;
	Backtrace *_backtrace;
	Callgraph *_callgraph;

	// menu
	QMenu *_viewWindowMenu;

	// Actions
	QAction *_debugStepOnStart;
	QAction *_debugForwardtraceOnStart;
	QAction *_debugStartMenu;
	QAction *_debugStopMenu;
	QAction *_debugContinueMenu;
	QAction *_debugStepIntoMenu;
	QAction *_debugStepOverMenu;
	QAction *_debugStepOutMenu;
	QAction *_debugStepIntoBackMenu;
	QAction *_debugStepOverBackMenu;
	QAction *_debugBacktracetMenu;
	QAction *_debugForwardtracetMenu;

	QAction *_informationEnvironmentMenu;
	QAction *_informationEnvironmentReloadMenu;
	QAction *_informationEnvironmentSysincludesMenu;
	QAction *_informationEnvironmentMacroNamesMenu;
	QAction *_informationEnvironmentMacrosMenu;

	enum { MaxRecentFiles = 5 };
	QAction *recentFileSeparator;
	QAction *recentFileActs[MaxRecentFiles];

	// config
	QString _windowtitle;

	LogConfig *_logconfig;

	QString curFile;
	Project *_project;
	msglib::Process *_process;
	QString _program;
	cmdmode_t _cmdmode;
	msglib::cmd::parser _cmdparser;
};

}
