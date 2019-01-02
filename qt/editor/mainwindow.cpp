#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    editor = new QPlainTextEdit();

    setCentralWidget(editor);
    createActions();
    createStatusBar();

    setCurrentFile(QString());
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Editor", QString("cannot open file %1:\n%2").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage("file loaded", 2000);
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *newAct = new QAction("&New", this);
    newAct->setStatusTip("create new file");
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    QAction *openAct = new QAction("&open", this);
    openAct->setStatusTip("open existing file");
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    QAction *xchgAct = new QAction("&Xchg", this);
    xchgAct->setStatusTip("exchange buffers");
    connect(xchgAct, &QAction::triggered, this, &MainWindow::xchg);
    fileMenu->addAction(xchgAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::xchg()
{

}

void MainWindow::setCurrentFile(const QString &file)
{
    curFile = file;
    editor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty()) {
        shownName = "untitled";
    }
    setWindowFilePath(shownName);
    setWindowTitle("Editor: " + shownName);
}

void MainWindow::newFile()
{
    setCurrentFile(QString());
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}
