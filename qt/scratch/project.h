#ifndef PROJECT_H
#define PROJECT_H

#include <QtCore>
#include "crossref.h"

struct ProjectConf
{
	QString name;
	QString srcpath;
	QString dbpath;
};

class Project
{
public:
	Project(ProjectConf &pc);
	QString name() { return conf.name; }
	QString srcpath() { return conf.srcpath + '/'; }
	QString dbpath() { return conf.dbpath + '/'; }

	bool open();
	bool close(bool del);
	static bool remove(QString &dbpath);
	//bool openFile(QString& fileName);

	bool build();
	//bool gotoDef(QString& tok);
	//bool findRef(QString& tok);
	//bool grep(QString& tok);

private:
	void readSettings();
	void writeSettings();

	ProjectConf conf;
	CrossRef *cref;
};

#endif // PROJECT_H
