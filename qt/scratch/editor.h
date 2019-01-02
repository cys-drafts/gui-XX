#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>

class Editor : public QPlainTextEdit
{
	Q_OBJECT

public:
	Editor();
	~Editor();

	void newFile();
	bool loadFile(const QString& name);
	QString fileName();
	QString canName();

private:
	QString curFile;

	void setCurrentFile(const QString& name);
};

#endif // EDITOR_H
