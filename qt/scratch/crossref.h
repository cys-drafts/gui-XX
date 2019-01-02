#ifndef CROSSREF_H
#define CROSSREF_H

#include <QtCore>

struct location {
	QString file;
	QString tag;
	qint32 line;
	QString image;
};

class CrossRef : public QProcess
{
	 Q_OBJECT

public:
	CrossRef();

	static bool remove(QString &dbpath);

	bool build(QString &dbpath, QString &srcpath);
	void findDef(QString dbpath, QString srcpath, QString token);
	void findRef(QString dbpath, QString srcpath, QString token);
	void findFiles(QString dbpath, QString srcpath, const QString &pattern);

public slots:
	void startedSlot();
	void finishedSlot(int, QProcess::ExitStatus);
	void errorOccurredSlot(QProcess::ProcessError);
	void stdoutReadySlot();
	void stderrReadySlot();

signals:
	void newLines(QStringList &strings);

private:
	QString stdouts;
};

#endif // CROSSREF_H
