#include <QtWidgets>

#include "mainwindow.h"
#include "editor.h"
#include "projectmanager.h"
#include "projectnewdialog.h"
#include "projectlistdialog.h"
#include "projectbrowserdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), mdi(new QMdiArea), pm(new ProjectManager)
{
	mdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdi);

	createActions();

	readSettings();

	setWindowTitle(tr("Scratch"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	writeSettings();
	pm->writeSettings();
	event->accept();
}

void MainWindow::newFile()
{
	Editor *child = new Editor;
	mdi->addSubWindow(child);
	child->newFile();
	child->show();
}

bool MainWindow::loadFile(const QString& name)
{
	Editor *child = new Editor;
	mdi->addSubWindow(child);
	bool res = child->loadFile(name);
	if (res) {
		child->show();
	} else {
		child->close();
	}
	return res;
}

bool MainWindow::openFile(const QString &name)
{
	QMdiSubWindow *subWin = findEditorWindow(name);
	if (subWin) {
		mdi->setActiveSubWindow(subWin);
		return true;
	}
	return loadFile(name);
}

void MainWindow::open()
{
	const QString f = QFileDialog::getOpenFileName(this);
	if (!f.isEmpty()) {
		openFile(f);
	}
}

void MainWindow::updateWindowMenu()
{
	winMenu->clear();

	QList<QMdiSubWindow*> wins = mdi->subWindowList();
	for (int i = 0; i < wins.size(); i++) {
		QMdiSubWindow *subWin = wins.at(i);
		Editor *e = qobject_cast<Editor*>(subWin->widget());
		QString text = e->fileName();
		QAction *act = winMenu->addAction(text, subWin, [this, subWin](){ mdi->setActiveSubWindow(subWin); });
		act->setCheckable(true);
		//act->setChecked(e == activeEditor());
		act->setChecked(subWin == mdi->currentSubWindow());
	}
}

void MainWindow::createProject()
{
	if (!pm->closeCurrentProject(false)) {
		qDebug() << "cannot close active project\n";
		return;
	}

	ProjectNewDialog pnDialog(this);

	if (pnDialog.exec() == QDialog::Rejected) {
		return;
	}
	PMError err = pm->createProject(pnDialog.projName(), pnDialog.projPath());
	switch (err) {
	case PME_Exists:
		qDebug() << "project exists\n";
		break;
	case PME_Path:
		qDebug() << "no such path\n";
		break;
	case PME_Mem:
		qDebug() << "out of memory\n";
		break;
	case PME_Build:
		qDebug() << "cannot build database\n";
		break;
	case PME_Ok:
		// succeed
		break;
	}
}

void MainWindow::openProject()
{
	listProjects();
}

void MainWindow::delProject()
{
	listProjects();
}

void MainWindow::listProjects()
{
	ProjectListDialog plDialog(this, pm);
	plDialog.exec();
}

void MainWindow::openProjectFile()
{
	if (!pm->hasActiveProject()) {
		qDebug() << "no active project";
		return;
	}
	ProjectBrowserDialog pbDialog(this, true, pm->getCurProject());
	pbDialog.exec();
	//qDebug() << pbDialog.getPath();
	openFile(pbDialog.getPath());
}

void MainWindow::browseSymbols()
{
	if (!pm->hasActiveProject()) {
		qDebug() << "no active project";
		return;
	}
	ProjectBrowserDialog pbDialog(this, false, pm->getCurProject());
	pbDialog.exec();
	QString sym = pbDialog.getSym();
	gotoDef(sym);
}

void MainWindow::gotoDef(QString sym)
{
	qDebug() << "goto " + sym;
}

void MainWindow::gotoDefDialog()
{

}

void MainWindow::createActions()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QToolBar *toolbar = addToolBar(tr("File"));

	newAct = new QAction(tr("&New"), this);
	connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
	fileMenu->addAction(newAct);
	toolbar->addAction(newAct);
	loadAct = new QAction(tr("&Open"), this);
	connect(loadAct, &QAction::triggered, this, &MainWindow::openProjectFile);
	fileMenu->addAction(loadAct);
	toolbar->addAction(loadAct);

	projMenu = menuBar()->addMenu(tr("&Project"));
	projNewAct = new QAction(tr("New Project"), this);
	connect(projNewAct, &QAction::triggered, this, &MainWindow::createProject);
	projMenu->addAction(projNewAct);
	projOpenAct = new QAction(tr("Open Project"), this);
	connect(projOpenAct, &QAction::triggered, this, &MainWindow::openProject);
	projMenu->addAction(projOpenAct);
	projDelAct = new QAction(tr("Remove Project"), this);
	connect(projDelAct, &QAction::triggered, this, &MainWindow::delProject);
	projMenu->addAction(projDelAct);
	projListAct = new QAction(tr("List Projects"), this);
	connect(projListAct, &QAction::triggered, this, &MainWindow::listProjects);
	projMenu->addAction(projListAct);

	navMenu = menuBar()->addMenu(tr("&Navigate"));
	fileBrowseAct = new QAction(tr("Browse files"), this);
	connect(fileBrowseAct, &QAction::triggered, this, &MainWindow::openProjectFile);
	navMenu->addAction(fileBrowseAct);
	symBrowseAct = new QAction(tr("Browse symbols"), this);
	connect(symBrowseAct, &QAction::triggered, this, &MainWindow::browseSymbols);
	navMenu->addAction(symBrowseAct);
	navMenu->addSeparator();
	gotoDefAct = new QAction(tr("Goto Definition"), this);
	connect(gotoDefAct, &QAction::triggered, this, &MainWindow::gotoDefDialog);
	navMenu->addAction(gotoDefAct);

	winMenu = menuBar()->addMenu(tr("&Window"));
	connect(winMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);
}

Editor* MainWindow::activeEditor()
{
	if (QMdiSubWindow *subWin = mdi->activeSubWindow()) {
		return qobject_cast<Editor*>(subWin->widget());
	}
	return 0;
}

QMdiSubWindow *MainWindow::findEditorWindow(const QString& name)
{
	QString canName = QFileInfo(name).canonicalFilePath();
	foreach (QMdiSubWindow *win, mdi->subWindowList()) {
		Editor *e = qobject_cast<Editor*>(win->widget());
		if (e->canName() == canName) {
			return win;
		}
	}
	return 0;
}

void MainWindow::readSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	if (!geometry.isEmpty()) {
		restoreGeometry(geometry);
	}
}

void MainWindow::writeSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	settings.setValue("geometry", saveGeometry());
}

MainWindow::~MainWindow()
{

}
