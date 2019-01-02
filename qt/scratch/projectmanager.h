#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QtWidgets>
#include "project.h"

enum PMError
{
	PME_Ok,
	PME_Exists,
	PME_Path,
	PME_Mem,
	PME_Build,
};

class ProjectManager: public QObject
{
	Q_OBJECT

public:
	ProjectManager();
	~ProjectManager();

	void writeSettings();
	Project *getCurProject() { return curProject; }

public slots:
	PMError createProject(QString name, QString path);
	bool openProject(QString &name);
	bool closeCurrentProject(bool del);
	bool deleteProject(QString &name);
	QList<ProjectConf>* listProjects();
	bool hasActiveProject() {return (curProject != nullptr) ? true : false;}

private:
	void readSettings();
	bool existProject(QString name);
	bool isActiveProject(QString &name) {if (!curProject) return false; return (name == curProject->name()) ? true : false;}

	Project *curProject;
	QList<ProjectConf> pList;
	QString defaultDbPath;
};

#endif // PROJECTMANAGER_H
