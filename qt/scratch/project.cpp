#include "project.h"

Project::Project(ProjectConf &pc): conf(pc)
{
	cref = new CrossRef();
}

void Project::readSettings()
{
	QSettings sets(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	sets.beginGroup(conf.name);
	sets.endGroup();
}

void Project::writeSettings()
{
	QSettings sets(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	sets.beginGroup(conf.name);
	sets.endGroup();
}

bool Project::open()
{
	return true;
}

bool Project::close(bool del)
{
	return true;
}

bool Project::remove(QString &dbpath)
{
	return CrossRef::remove(dbpath);
}

bool Project::build()
{
	QDir dir(conf.dbpath);
	if (!dir.exists()) {
		if (!dir.mkpath(conf.dbpath)) {
			qDebug() << "Fatal: cannot create db path: " + conf.dbpath;
			return false;
		}
	}
	return cref->build(conf.dbpath, conf.srcpath);
}
