#include <QtGlobal>

#include "projectmanager.h"
#include "projectnewdialog.h"

ProjectManager::ProjectManager()
{
	curProject = nullptr;
	defaultDbPath = QDir::homePath() + QDir::separator() + "." + QCoreApplication::applicationName();
	qDebug() << "default db path: " + defaultDbPath;
	QDir dir(defaultDbPath);
	if (!dir.exists()) {
		if (!dir.mkpath(defaultDbPath)) {
			qDebug() << "Fatal: cannot create db path: " + defaultDbPath;
		}
	}
	readSettings();
	foreach (ProjectConf pc, pList) {
		qDebug() << pc.name << pc.srcpath;
	}
}

PMError ProjectManager::createProject(QString name, QString path)
{
	qDebug() << name << path;

	Q_ASSERT(curProject == nullptr);

	if (existProject(name))
		return PME_Exists;

	ProjectConf pc;
	pc.name = name;
	pc.srcpath = path;
	pc.dbpath = defaultDbPath + QDir::separator() + name;

	curProject = new Project(pc);
	if (!curProject)
		return PME_Mem;
	if (!curProject->build()) {
		delete curProject;
		curProject = nullptr;
		return PME_Build;
	}

	pList.append(pc);

	writeSettings();

	return PME_Ok;
}

bool ProjectManager::openProject(QString &name)
{
	Q_ASSERT(curProject == nullptr);

	foreach (ProjectConf pc, pList) {
		if (pc.name == name) {
			curProject = new Project(pc);
			if (!curProject)
				return false;
			else
				break;
		}
	}

	if (!curProject->open()) {
		closeCurrentProject(false);
		return false;
	}
	return true;
}

bool ProjectManager::deleteProject(QString &name)
{
	if (isActiveProject(name)) {
		qDebug() << "delete active project " + name;
		closeCurrentProject(true);
	}
	for (int i = 0; i < pList.size(); i++) {
		ProjectConf pc = pList.at(i);
		if (name == pc.name) {
			Project::remove(pc.dbpath);
			pList.removeAt(i);
			break;
		}
	}
	QSettings sets(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	sets.remove(name);
	writeSettings();
	return true;
}

QList<ProjectConf>* ProjectManager::listProjects()
{
	return &pList;
}

bool ProjectManager::closeCurrentProject(bool del)
{
	if (!curProject)
		return true;

	if (!curProject->close(del)) {
		return false;
	}
	delete curProject;
	curProject = nullptr;
	return true;
}

bool ProjectManager::existProject(QString name)
{
	foreach (ProjectConf pc, pList) {
		if (pc.name == name)
			return true;
	}
	return false;
}

void ProjectManager::readSettings()
{
	QSettings sets(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	int size = sets.beginReadArray("projects");
	for (int i = 0; i < size; i++) {
		sets.setArrayIndex(i);
		ProjectConf pc;
		pc.name = sets.value("name").toString();
		pc.srcpath = sets.value("srcpath").toString();
		pc.dbpath = sets.value("dbpath").toString();
		pList.append(pc);
	}
	sets.endArray();
}

void ProjectManager::writeSettings()
{
	QSettings sets(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	sets.beginWriteArray("projects");
	for (int i = 0; i < pList.size(); i++) {
		sets.setArrayIndex(i);
		sets.setValue("name", pList[i].name);
		sets.setValue("srcpath", pList[i].srcpath);
		sets.setValue("dbpath", pList[i].dbpath);
	}
	sets.endArray();
}

ProjectManager::~ProjectManager()
{
	writeSettings();
	closeCurrentProject(false);
}
