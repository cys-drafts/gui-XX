#ifndef PROJECTNEWDIALOG_H
#define PROJECTNEWDIALOG_H

#include "ui_projectnew.h"

class ProjectNewDialog : public QDialog, public Ui::ProjectNew
{
	Q_OBJECT

public:
	ProjectNewDialog(QWidget *parent);
	QString projName();
	QString projPath();

public slots:
	void selectRootPath();
	void validateInput();
};

#endif // PROJECTNEWDIALOG_H
