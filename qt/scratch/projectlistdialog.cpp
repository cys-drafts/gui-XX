#include <QStringListModel>
#include "projectlistdialog.h"

ProjectListDialog::ProjectListDialog(QWidget *parent, ProjectManager *pm):
	QDialog(parent)
{
	this->pm = pm;

	setupUi(this);

	pListView->setSelectionRectVisible(true);
	pListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(openButton, SIGNAL(clicked()), this, SLOT(openProject()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(deleteProject()));

	updateModel();
}

void ProjectListDialog::openProject()
{
	QString selectedText = getSelectionText();
	if (selectedText == "") {
		return;
	}
	pm->openProject(selectedText);
	// close the dialog
	accept();
}

void ProjectListDialog::deleteProject()
{
	QString selectedText = getSelectionText();
	if (selectedText == "") {
		return;
	}
	qDebug() << "try to delete " + selectedText;
	pm->deleteProject(selectedText);
	//update the view
	updateModel();
	// will not close the dialog
}

QString ProjectListDialog::getSelectionText()
{
	const QModelIndex idx = pListView->selectionModel()->currentIndex();
	if (!idx.isValid())
		return "";
	return idx.data(Qt::DisplayRole).toString().split(QRegExp("\t"))[0];
}

void ProjectListDialog::updateModel()
{
	QItemSelectionModel *smodel = pListView->selectionModel();
	QStringListModel *model = new QStringListModel();
	QStringList list;
	QList<ProjectConf> *plist = pm->listProjects();
	for (int i = 0; i < plist->size(); i++) {
		ProjectConf pc = plist->at(i);
		list << pc.name + "\t" + pc.srcpath;
	}
	model->setStringList(list);
	pListView->setModel(model);
	delete smodel;
}
