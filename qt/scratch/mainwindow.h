#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "projectmanager.h"

class Editor;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event) override;

private slots:
	void newFile();
	void open();
	bool openFile(const QString& name);
	bool loadFile(const QString& name);
	void updateWindowMenu();

	void createProject();
	void openProject();
	void delProject();
	void listProjects();
	void openProjectFile();
	void browseSymbols();
	void gotoDefDialog();

private:
	void readSettings();
	void writeSettings();
	void createActions();
	void createDocks();
	Editor *activeEditor();
	QMdiSubWindow *findEditorWindow(const QString& name);

	void gotoDef(QString sym);

	QMdiArea *mdi;
	ProjectManager *pm;

	QMenu *winMenu;
	QMenu *projMenu;
	QMenu *navMenu;

	QAction *newAct;
	QAction *loadAct;

	QAction *projNewAct;
	QAction *projOpenAct;
	QAction *projDelAct;
	QAction *projListAct;

	QAction *fileBrowseAct;
	QAction *symBrowseAct;
	QAction *gotoDefAct;
};

#endif // MAINWINDOW_H
