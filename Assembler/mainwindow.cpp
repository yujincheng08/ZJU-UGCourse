#include "mainwindow.h"
#include "editor.h"
#include "highlighter.h"
#include "RamTable/ramtable.h"
#include "phraser.h"
#include <QAction>
#include <QMenuBar>
#include <QApplication>
#include <QToolBar>
#include <QLabel>
#include <QStatusBar>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QSettings>
#include <QObject>
#include <QStringList>
#include <QPrinter>
#include <QPrintDialog>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QFontDatabase::addApplicationFont(":/fonts/SourceCodePro.ttf");

    phraser = new Phraser(this);
	createActions();
	createMenus();
	createEditor();
    createToolbars();
    createDockWindows();
	createConnections();
	setCentralWidget(editor);
	createStatusBar();

	readSettings();
	setCurFile("");


}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
	newAction = new QAction(tr("&New"),this);
	newAction->setIcon(QIcon(":/icons/new.png"));
	newAction->setShortcut(QKeySequence::New);
	newAction->setStatusTip(tr("Create a New File."));

	openAction = new QAction(tr("&Open"),this);
	openAction->setShortcut(QKeySequence::Open);
	openAction->setIcon(QIcon(":/icons/open.png"));

	saveAction = new QAction(tr("&Save"),this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setIcon(QIcon(":/icons/save.png"));

	saveAsAction = new QAction(tr("Save &As"),this);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	saveAsAction->setIcon(QIcon(":/icons/save.png"));

	printAction = new QAction(tr("&Print"),this);
	printAction->setShortcut(QKeySequence::Print);
	printAction->setIcon(QIcon(":/icons/print.png"));

	exitAction = new QAction(tr("E&xit"),this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setIcon(QIcon(":/icons/exit.png"));

	undoAction = new QAction(tr("&Undo"),this);
	undoAction->setShortcut(QKeySequence::Undo);
	undoAction->setIcon(QIcon(":/icons/undo.png"));
	undoAction->setEnabled(false);

	redoAction = new QAction(tr("&Redo"),this);
	redoAction->setShortcut(QKeySequence::Redo);
	redoAction->setIcon(QIcon(":/icons/redo.png"));
	redoAction->setEnabled(false);

	cutAction = new QAction(tr("Cu&t"),this);
	cutAction->setShortcut(QKeySequence::Cut);
	cutAction->setIcon(QIcon(":/icons/cut.png"));
	cutAction->setEnabled(false);

	copyAction = new QAction(tr("&Copy"),this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setIcon(QIcon(":/icons/copy.png"));
	copyAction->setEnabled(false);

	pasteAction = new QAction(tr("&Paste"),this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setIcon(QIcon(":/icons/paste.png"));

	deleteAction = new QAction(tr("De&lete"),this);
	deleteAction->setShortcut(QKeySequence::Delete);
	deleteAction->setIcon(QIcon(":/icons/delete.png"));
	deleteAction->setEnabled(false);

	findAction = new QAction(tr("&Find"),this);
	findAction->setShortcut(QKeySequence::Find);
	findAction->setIcon(QIcon(":/icons/find.png"));

	replaceAction = new QAction(tr("&Replace"),this);
	replaceAction->setShortcut(QKeySequence::Replace);
	replaceAction->setIcon(QIcon(":/icons/find.png"));

	gotoAction = new QAction(tr("&Goto"),this);
	gotoAction->setShortcut(tr("Ctrl+G"));
	gotoAction->setIcon(QIcon(":/icons/goto.png"));

	selectAllAction = new QAction(tr("Select &All"),this);
	selectAllAction->setShortcut(QKeySequence::SelectAll);
	selectAllAction->setIcon(QIcon(":/icons/selectAll.png"));

    buildAction = new QAction(tr("Build"), this);
    buildAsmAction = new QAction(tr("Build &Bin"),this);
	buildAsmAction->setIcon(QIcon(":/icons/asm.png"));

	buildCoeAction = new QAction(tr("Build &Coe"),this);
	buildCoeAction->setIcon(QIcon(":/icons/coe.png"));

	disassemblyAction = new QAction(tr("&Disassembly"),this);
	disassemblyAction->setIcon(QIcon(":/icons/disassembly.png"));

	stepAction = new QAction(tr("&Step"),this);
	stepAction->setIcon(QIcon(":/icons/step.png"));

	jumpStepAction = new QAction(tr("Step (Skip &Jal)"),this);
	jumpStepAction->setIcon(QIcon(":/icons/jumpstep.png"));

	runtoAction = new QAction(tr("&Run to..."),this);
	runtoAction->setIcon(QIcon(":/icons/goto.png"));

	stopAction = new QAction(tr("St&op"),this);
	stopAction->setIcon(QIcon(":/icons/stop.png"));


	aboutAction = new QAction(tr("&About"),this);
	aboutAction->setIcon(QIcon(":/icons/about.png"));
	aboutQtAction = new QAction(tr("About &Qt"),this);
	aboutQtAction->setIcon(QIcon(":/icons/qt.png"));

    baseGroup = new QActionGroup(this);
    baseGroup->setExclusive(true);
    setHexAction = baseGroup->addAction(tr("&Hex"));
    setHexAction->setData(16);
    //setHexAction->setIcon
    setBinaryAction = baseGroup->addAction(tr("&Binary"));
    setBinaryAction->setData(2);
    setHexAction->setCheckable(true);
    setBinaryAction->setCheckable(true);
    setHexAction->setChecked(true);

	for(int i = 0; i<MaxRecentFiles; ++i)
	{
		recentFileActions[i] = new QAction(this);
		recentFileActions[i]->setParent(this);
		recentFileActions[i]->setVisible(false);
	}

}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(printAction);
	separatorAction = fileMenu->addSeparator();
	fileMenu->addSeparator();
	for(int i = 0; i < MaxRecentFiles; ++i)
		fileMenu->addAction(recentFileActions[i]);
	fileMenu->addAction(exitAction);
	editMenu = menuBar()->addMenu("&Edit");
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addSeparator();
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(deleteAction);
	editMenu->addAction(selectAllAction);
	editMenu->addSeparator();
	editMenu->addAction(findAction);
	editMenu->addAction(replaceAction);
	editMenu->addAction(gotoAction);

	viewMenu = menuBar()->addMenu("&View");
    baseMenu = viewMenu->addMenu("&Base");
    viewMenu->addSeparator();

    //baseMenu->addActions(baseGroup);
    baseMenu->addAction(setHexAction);
    baseMenu->addAction(setBinaryAction);

	buildMenu = menuBar()->addMenu("&Bulid");
    buildMenu->addAction(buildAction);
	buildMenu->addAction(buildAsmAction);
	buildMenu->addAction(buildCoeAction);
	buildMenu->addAction(disassemblyAction);

	debugMenu = menuBar()->addMenu("&Debug");
	debugMenu->addAction(stepAction);
	debugMenu->addAction(jumpStepAction);
	debugMenu->addAction(runtoAction);
	debugMenu->addAction(stopAction);

	aboutMenu = menuBar()->addMenu("&About");
	aboutMenu->addAction(aboutAction);
	aboutMenu->addAction(aboutQtAction);
}

void MainWindow::createConnections()
{

	connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

	connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(undoAction,SIGNAL(triggered()),editor,SLOT(undo()));
	connect(redoAction,SIGNAL(triggered()),editor,SLOT(redo()));
	connect(cutAction,SIGNAL(triggered()),editor,SLOT(cut()));
	connect(copyAction,SIGNAL(triggered()),editor,SLOT(copy()));
	connect(pasteAction,SIGNAL(triggered()),editor,SLOT(paste()));
	connect(deleteAction,SIGNAL(triggered()),editor,SLOT(removeSelectedText()));
	connect(selectAllAction,SIGNAL(triggered()),editor,SLOT(selectAll()));
	connect(editor,SIGNAL(undoAvailable(bool)),undoAction,SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(redoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(copyAvailable(bool)),copyAction,SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(copyAvailable(bool)),cutAction,SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(copyAvailable(bool)),deleteAction,SLOT(setEnabled(bool)));

	connect(newAction,SIGNAL(triggered()),this,SLOT(newFile()));
	connect(openAction,SIGNAL(triggered()),this,SLOT(open()));
	connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
	connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));
	connect(editor,SIGNAL(modificationChanged(bool)),this,SLOT(setWindowModified(bool)));

	for(int i = 0; i < MaxRecentFiles; ++i)
		connect(recentFileActions[i], SIGNAL(triggered()),this,SLOT(openRecentFile()));

	connect(printAction,SIGNAL(triggered()),this,SLOT(print()));

    connect(buildAction,SIGNAL(triggered()),this,SLOT(Build()));

    connect(baseGroup,SIGNAL(triggered(QAction*)),this,SLOT(setBase(QAction*)));
    connect(phraser,SIGNAL(BuildDone()),ramTable,SLOT(refresh()));
}

void MainWindow::createEditor()
{
	editor = new Editor(this);
	highlighter = new Highlighter(editor->document());
	editor->addAction(undoAction);
	editor->addAction(redoAction);
	editor->addAction(cutAction);
	editor->addAction(copyAction);
	editor->addAction(pasteAction);
	editor->addAction(deleteAction);
	editor->addAction(selectAllAction);
	editor->setContextMenuPolicy(Qt::ActionsContextMenu);
    phraser->setDocument(editor->document());
}

void MainWindow::createDockWindows()
{
	dock = new QDockWidget(tr("Ram"),this);
	ramTable = new RamTable(dock);
	dock->setWidget(ramTable);
    dock->resize(dock->sizeHint());
	dock->setObjectName("RamTable");
	addDockWidget(Qt::RightDockWidgetArea,dock);
	viewMenu->addAction(dock->toggleViewAction());
    ramTable->refresh();
}

void MainWindow::createToolbars()
{
	fileToolbar = addToolBar(fileMenu->title());
	fileToolbar->setObjectName("FileToolBar");
	fileToolbar->addAction(newAction);
	fileToolbar->addAction(openAction);
	fileToolbar->addAction(saveAction);
	fileToolbar->addAction(printAction);

	editToolbar = addToolBar(editMenu->title());
	editToolbar->addActions(editMenu->actions());
	editToolbar->setObjectName("EditToolBar");

	addToolBarBreak();
	buildToolbar = addToolBar(buildMenu->title());
	buildToolbar->addActions(buildMenu->actions());
	buildToolbar->setObjectName("BuildToolBar");

	debugToolbar = addToolBar(debugMenu->title());
	debugToolbar->addActions(debugMenu->actions());
	debugToolbar->setObjectName("DebugToolBar");

}

void MainWindow::createStatusBar()
{
	//status = new QLabel;
	//status->setAlignment(Qt::AlignLeft);
	//status->setMinimumSize(status->sizeHint());

	statusBar();
}

void MainWindow::writeSettings()
{
    QSettings settings("Jincheng YU", "Assembler");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles",recentFiles);
    settings.setValue("state", saveState());
}

void MainWindow::readSettings()
{
	QSettings settings("Jincheng YU", "Assembler");

	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	recentFiles = settings.value("recentFiles").toStringList();
	updateRecentFileActions();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(fileSaved())
	{
		writeSettings();
		event->accept();
	}
	else
		event->ignore();
}

bool MainWindow::saveFile(const QString &fileName)
{
	if(!editor->saveFile(fileName))
	{
		statusBar()->showMessage(tr("Saving canceled"),2000);
		return false;
	}
	setCurFile(fileName);
	statusBar()->showMessage(tr("File saved"),2000);
	return true;
}

void MainWindow::setCurFile(const QString &fileName)
{
	curFile = fileName;
	setWindowModified(false);

	QString showName = tr("Untitled");
	if(!curFile.isEmpty())
	{
		showName = QFileInfo(curFile).fileName();
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}

	setWindowTitle(tr("%1[*] - %2").arg(showName)
				   .arg(tr("Assembler")));
}

bool MainWindow::fileSaved()
{
	if(isWindowModified())
	{
		int r = QMessageBox::warning(this,
							tr("Assembler"),
							tr("The document has been modified.\n"
							   "Do you want to save your changed?"),
							QMessageBox::Yes|QMessageBox::No|
									 QMessageBox::Cancel);
		if(r == QMessageBox::Yes)
			return save();
		else if(r == QMessageBox::Cancel)
			return false;
	}
	return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
	if(!editor->loadFile(fileName))
	{
		statusBar()->showMessage(tr("Loading canceled"),2000);
		return false;
	}
	statusBar()->showMessage(tr("Loading successed"),2000);
	setCurFile(fileName);
	return true;
}

void MainWindow::updateRecentFileActions()
{
	QMutableStringListIterator i(recentFiles);
	while(i.hasNext())
	{
		if(!QFile::exists(i.next()))
			i.remove();
	}

	for(int j = 0; j < MaxRecentFiles; ++j)
	{
		if(j < recentFiles.count())
		{
            QString text = tr("&%1| %2")
					.arg(j + 1)
					.arg(QFileInfo(recentFiles[j]).fileName());
			recentFileActions[j]->setText(text);
			recentFileActions[j]->setData(recentFiles[j]);
			recentFileActions[j]->setVisible(true);
		}
		else
			recentFileActions[j]->setVisible(false);

		separatorAction->setVisible(!recentFiles.isEmpty());
	}
}

void MainWindow::openRecentFile()
{
	if(fileSaved())
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if(action)
			loadFile(action->data().toString());
    }
}

void MainWindow::setBase(QAction *action)
{
    ramTable->setBase(action->data().toInt());
}

void MainWindow::Build()
{
    ramTable->reset();
    phraser->Build(ramTable->rawData());
}

void MainWindow::open()
{
	if(fileSaved())
	{
		QString fileName =
				QFileDialog::getOpenFileName(this,
							 tr("Open ASM file"),".",
							 tr("ASM files (*.asm)"));
		if(!fileName.isEmpty())
			loadFile(fileName);
	}
}

void MainWindow::newFile()
{
	if(fileSaved())
	{
		editor->clear();
		setCurFile("");
	}
}

bool MainWindow::save()
{
	if(curFile.isEmpty())
		return saveAs();
	else
		return saveFile(curFile);
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this,
									tr("Save ASM File"),".",
									tr("ASM File (*.asm)"));
	if(fileName.isEmpty())
		return false;
	else
		return saveFile(fileName);
}

void MainWindow::print()
{
#ifndef Q_OS_ANDROID
	QPrinter printer;

	QPrintDialog *dialog = new QPrintDialog(&printer,this);
	dialog->setWindowTitle(tr("Print Document"));
	if(editor->textCursor().hasSelection())
		dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
	if(dialog->exec() != QDialog::Accepted)
		return;
	editor->print(&printer);
#endif
}

