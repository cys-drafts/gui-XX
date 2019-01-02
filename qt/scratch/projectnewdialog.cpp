#include <QtWidgets>

#include "projectnewdialog.h"

ProjectNewDialog::ProjectNewDialog(QWidget *parent):
	QDialog(parent)
{
	QRegExpValidator *v = new QRegExpValidator(QRegExp("[-_A-Za-z0-9]+"));

	setupUi(this);
	nameText->setValidator(v);
	connect(dirSelect, SIGNAL(clicked()), this, SLOT(selectRootPath()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(validateInput()));
}

void ProjectNewDialog::selectRootPath()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath());
	if (!dir.isEmpty()) {
		dirText->setText(dir);
	}
}

void ProjectNewDialog::validateInput()
{
	if (nameText->text() != "" && dirText->text() != "")
		accept();
	else
		reject();
}

QString ProjectNewDialog::projName()
{
	return nameText->text();
}

QString ProjectNewDialog::projPath()
{
	return dirText->text();
}
