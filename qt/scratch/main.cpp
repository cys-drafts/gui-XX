#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setApplicationName("Scratch");
	QCoreApplication::setApplicationVersion("0.1");
	QCoreApplication::setOrganizationName("IOStar Studio");

	MainWindow w;
	w.show();

	return a.exec();
}
