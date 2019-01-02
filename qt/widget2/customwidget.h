#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
class CustomWidget;
}

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWidget(QWidget *parent = 0);
    ~CustomWidget();

private slots:
    void itemClicked(QListWidgetItem *item);
    void updateItem();

private:
    QListWidget *m_widget;
    QLineEdit *m_edit;
    QPushButton *m_button;

private:
    Ui::CustomWidget *ui;
};

#endif // CUSTOMWIDGET_H
