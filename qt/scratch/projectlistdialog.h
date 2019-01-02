#ifndef PROJECTLISTDIALOG_H
#define PROJECTLISTDIALOG_H

#include "ui_projectlist.h"
#include "projectmanager.h"

class ProjectListDialog : public QDialog, Ui::ProjectList
{
	Q_OBJECT

public:
	ProjectListDialog(QWidget *parent, ProjectManager *pm);

public slots:
	void openProject();
	void deleteProject();

private:
	ProjectManager *pm;

	void updateModel();
	QString getSelectionText();
};

#endif // PROJECTLISTDIALOG_H
