/*******
  在战斗场景中显示操作栏,不响应键盘
********/
#ifndef OPERATIONWIDGET_H
#define OPERATIONWIDGET_H

#include "lgl.h"
#define OPERATIONICONW 24
#define OPERATIONICONH 24

class OperationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OperationWidget(QWidget *parent);
    //这个pointX和pointY指的是role的左上角点的窗口坐标
    void show(int roleViewPointX,int roleViewPointY,int index);//rolePointxy指的是view坐标
    void hide();
//    void setIndex(int value);
    
signals:
    
public slots:
private:
    int x;
    int y;//这是绘图原点
    int currentSelectedIndex;
    void paintEvent(QPaintEvent *);
//    int explainationTimerID;
//    void timerEvent(QTimerEvent *);
    QPixmap walk;
    QPixmap attack;
    QPixmap recover;
    QPixmap goods;
    
};

#endif // OPERATIONWIDGET_H
