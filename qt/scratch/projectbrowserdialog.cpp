#include <QtCore>
#include "projectbrowserdialog.h"
#include "crossref.h"

int ProjectModel::rowCount(const QModelIndex &parent) const
{
	(void)parent;
	return items.count();
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() > items.size())
		return QVariant();
	if (role == Qt::DisplayRole)
		return items.at(index.row());
	else
		return QVariant();
}

bool ProjectModel::insertRows(int row, int count, const QModelIndex &parent)
{
	(void)parent;
	beginInsertRows(QModelIndex(), row, row+count-1);
	endInsertRows();
	return true;
}

void ProjectModel::appendNewLines(QStringList &strings)
{
	int rows = rowCount();
	items += strings;
	insertRows(rows, strings.count());
}

void ProjectModel::flush()
{
	beginResetModel();
	items.clear();
	endResetModel();
}

ProjectBrowserDialog::ProjectBrowserDialog(QWidget *parent, bool path, Project *p):
	QDialog(parent)
{
	isPath = path;
	pro = p;
	path = "";
	sym = "";

	timer = new QTimer(this);
	timer->setInterval(500);
	timer->setSingleShot(true);

	model = new ProjectModel();

	setupUi(this);
	setWindowTitle(tr(isPath ? "Files" : "Symbols"));
	listView->setSelectionRectVisible(true);
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	listView->setModel(model);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateResult()));
	connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(inputChanged(QString)));
	connect(&cref, SIGNAL(newLines(QStringList&)), model, SLOT(appendNewLines(QStringList&)));
	connect(listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doSelected(const QModelIndex&)));

	setupEngine(pro, "");
}

void ProjectBrowserDialog::inputChanged(QString)
{
	if (timer->isActive()) {
		timer->stop();
		qDebug() << "restart timer";
	}
	while (cref.state() != QProcess::NotRunning) {
		cref.terminate();
	}
	timer->start();
}

void ProjectBrowserDialog::updateResult()
{
	qDebug() << "requery";
	model->flush();
	setupEngine(pro, lineEdit->text());
}

bool ProjectBrowserDialog::setupEngine(Project *p, const QString &pattern)
{
	if (isPath) {
		cref.findFiles(p->dbpath(), p->srcpath(), pattern);
	} else {
		cref.findDef(p->dbpath(), p->srcpath(), pattern);
	}
	return true;
}

void ProjectBrowserDialog::doSelected(const QModelIndex &index)
{
	QString selectionText;

	selectionText = index.data(Qt::DisplayRole).toString();
	if (isPath) {
		path = pro->srcpath() + selectionText;
	} else {
		sym = selectionText;
	}
	accept();
}
