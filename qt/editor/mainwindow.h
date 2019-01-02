#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QTextDocument;
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadFile(const QString &file);

private slots:
    void open();
    void newFile();
    void xchg();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void createActions();
    void createStatusBar();
    void setCurrentFile(const QString &file);

private:
    Ui::MainWindow *ui;
    QPlainTextEdit *editor;
    QString curFile;

};

#endif // MAINWINDOW_H
