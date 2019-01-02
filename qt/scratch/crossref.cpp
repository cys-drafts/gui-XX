#include "crossref.h"

CrossRef::CrossRef()
{
	connect(this, SIGNAL(started()), this, SLOT(startedSlot()));
	connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finishedSlot(int,QProcess::ExitStatus)));
	connect(this, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(errorOccurredSlot(QProcess::ProcessError)));
	connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(stdoutReadySlot()));
	connect(this, SIGNAL(readyReadStandardError()), this, SLOT(stderrReadySlot()));
}

bool CrossRef::build(QString &dbpath, QString &srcpath)
{
	setWorkingDirectory(srcpath);
	start("gtags " + dbpath);
	//waitForFinished();
	return true;
}

void CrossRef::findDef(QString dbpath, QString srcpath, QString prefix)
{
	if (state() != QProcess::NotRunning) {
		qDebug() << "still running";
		return;
	}
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("GTAGSROOT", srcpath);
	env.insert("GTAGSDBPATH", dbpath);
	setProcessEnvironment(env);
	setWorkingDirectory(srcpath);
	stdouts.clear();
	start("global -c " + prefix);
}

void CrossRef::gotoDef(QString dbpath, QString srcpath, QString prefix)
{
	if (state() != QProcess::NotRunning) {
		qDebug() << "still running";
		return;
	}
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("GTAGSROOT", srcpath);
	env.insert("GTAGSDBPATH", dbpath);
	setProcessEnvironment(env);
	setWorkingDirectory(srcpath);
	stdouts.clear();
	start("global -x -d " + prefix);
}

void CrossRef::findRef(QString dbpath, QString srcpath, QString token)
{

}

void CrossRef::findFiles(QString dbpath, QString srcpath, const QString &pattern)
{
	if (state() != QProcess::NotRunning) {
		qDebug() << "still running";
		return;
	}
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("GTAGSROOT", srcpath);
	env.insert("GTAGSDBPATH", dbpath);
	setProcessEnvironment(env);
	setWorkingDirectory(srcpath);
	stdouts.clear();
	start("global --match-part all -P " + pattern);
}

bool CrossRef::remove(QString &dbpath)
{
	QDir dir(dbpath);
	QStringList files = dir.entryList();
	foreach (QString f, files) {
		dir.remove(f);
	}
	dir.rmdir(dbpath);
	return true;
}

void CrossRef::startedSlot()
{
	qDebug() << "started";
}

void CrossRef::finishedSlot(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (exitStatus == QProcess::NormalExit) {
		qDebug() << QString("finished normally: %1").arg(exitCode);
	} else {
		qDebug() << "crashed";
	}
}

void CrossRef::errorOccurredSlot(QProcess::ProcessError error)
{
	qDebug() << QString("failed: %1").arg(error);
}

void CrossRef::stdoutReadySlot()
{
	stdouts += readAllStandardOutput();
	stdouts.remove('\r');
	QStringList strings = stdouts.split('\n');
	stdouts = "";
	emit newLines(strings);
}

void CrossRef::stderrReadySlot()
{
	qDebug() << readAllStandardError();
}
