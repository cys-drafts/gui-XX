#include "mainwindow.h"
#include "customwidget.h"

#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    MainWindow w;
    w.show();
#else
    QScopedPointer<QWidget> w(new CustomWidget());
    w->resize(240, 120);
    w->show();
#endif

    return a.exec();
}
