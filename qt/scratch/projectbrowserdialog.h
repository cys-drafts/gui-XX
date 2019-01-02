#ifndef PROJECTBROWSERDIALOG_H
#define PROJECTBROWSERDIALOG_H

#include "ui_projectbrowser.h"
#include "project.h"

class ProjectModel : public QAbstractListModel
{
	Q_OBJECT

public:
	ProjectModel(QObject *parent = 0) : QAbstractListModel(parent) {}
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	void flush();

public slots:
	void appendNewLines(QStringList &strings);

private:
	QStringList items;
};

class ProjectBrowserDialog : public QDialog, public Ui::ProjectBrowser
{
	Q_OBJECT

public:
	ProjectBrowserDialog(QWidget *parent, bool path, Project *p);
	QString getPath() { return path; }
	QString getSym() { return sym; }

private slots:
	void inputChanged(QString);
	void updateResult();
	void doSelected(const QModelIndex &index);

private:
	bool isPath;
	QString path;
	QString sym;
	Project *pro;
	CrossRef cref;
	ProjectModel *model;
	QTimer *timer;
	bool setupEngine(Project *p, const QString &pattern);

};

#endif // PROJECTBROWSERDIALOG_H
