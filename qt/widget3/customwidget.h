#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace Ui {
class CustomWidget;
}

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWidget(QWidget *parent = 0);
    ~CustomWidget();

private:
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

private:
    Ui::CustomWidget *ui;
};

#endif // CUSTOMWIDGET_H
