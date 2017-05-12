#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Phraser;
class Editor;
class Highlighter;
class QLabel;
class RamTable;
class QActionGroup;
class MainWindow : public QMainWindow
{
    Q_OBJECT
	const static int MaxRecentFiles = 5;
private:
	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *printAction;
	QAction *exitAction;

	QAction *undoAction;
	QAction *redoAction;
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *deleteAction;
	QAction *findAction;
	QAction *replaceAction;
	QAction *gotoAction;
	QAction *selectAllAction;

    QAction *buildAction;
	QAction *buildAsmAction;
	QAction *buildCoeAction;
	QAction *disassemblyAction;

	QAction *stepAction;
	QAction *jumpStepAction;
	QAction *runtoAction;
	QAction *stopAction;

	QAction *aboutAction;
	QAction *aboutQtAction;

	QAction *recentFileActions[MaxRecentFiles];
	QAction *separatorAction;

    QActionGroup *baseGroup;
    QAction *setHexAction;
    QAction *setBinaryAction;
private:
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *buildMenu;
	QMenu *debugMenu;
	QMenu *aboutMenu;
    QMenu *baseMenu;

private:
	QToolBar *fileToolbar;
	QToolBar *editToolbar;
	QToolBar *buildToolbar;
	QToolBar *debugToolbar;

private:
	QLabel *status;

private:
	Editor *editor;
	RamTable *ramTable;
	QDockWidget *dock;
	Highlighter *highlighter;
	QString curFile;
	QStringList recentFiles;

private:
    Phraser *phraser;
private:
	void createActions();
	void createMenus();
	void createConnections();
	void createEditor();
	void createToolbars();
	void createStatusBar();
	void createDockWindows();


	void writeSettings();
	void readSettings();
protected:
	void closeEvent(QCloseEvent *event) override;

private:
	bool saveFile(const QString &fileName);
	void setCurFile(const QString &fileName);
	bool fileSaved();
	bool loadFile(const QString &fileName);
	void updateRecentFileActions();

private slots:
	void openRecentFile();
    void setBase(QAction *action);
    void Build();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void open();
	void newFile();
	bool save();
	bool saveAs();
	void print();
};

#endif // MAINWINDOW_H
