#include <QtWidgets>

#include "editor.h"

Editor::Editor()
{
	setAttribute(Qt::WA_DeleteOnClose);
}

void Editor::newFile()
{
	static int seq = 1;

	curFile = tr("%1.txt").arg(seq);
	setWindowTitle(curFile);
	seq++;
}

bool Editor::loadFile(const QString& name)
{
	QFile f(name);
	if (!f.open(QFile::ReadOnly | QFile::Text)) {
		return false;
	}
	QTextStream in(&f);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(name);

	return true;
}

QString Editor::fileName()
{
	return QFileInfo(curFile).fileName();
}

QString Editor::canName()
{
	return curFile;
}

void Editor::setCurrentFile(const QString& name)
{
	curFile = QFileInfo(name).canonicalFilePath();
	setWindowTitle(fileName());
}

Editor::~Editor()
{

}
