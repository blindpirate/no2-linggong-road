/*该类提供血量显示(包括数字)和血量增减的动画显示,父窗口是scene窗口*/
#ifndef HPSLOTWIDGET_H
#define HPSLOTWIDGET_H

#include "lgl.h"
#include "roleanimation.h"

class HPSlotWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentHP READ getCurrentHP WRITE setCurrentHP)
    Q_PROPERTY(QRect numRect READ getNumRect WRITE setNumRect)

public:
    explicit HPSlotWidget(QWidget *parent,int rolePointX,int rolePointY,int startHP,int deltaHP,int fullHP,AnimationOperation op);
    //rolePointXY是roleView左上角点在视图中的坐标
signals:    
public slots:
    int getCurrentHP()const{return currentHP;}
    void setCurrentHP(int newHP){currentHP=newHP;this->update();}
    QRect getNumRect()const{return numRect;}
    void setNumRect(QRect rect){numRect=rect;this->update();}
    void start(bool endWithSignal);
private:
    AnimationOperation op;
    int x;
    int y;//绘图原点
    int startHP;
    int deltaHP;
    int fullHP;
    int currentHP;
    QRect numRect;
    QFont font;
    //int delayTimerID;
    void paintEvent(QPaintEvent *);
    //void timerEvent(QTimerEvent *);
};

#endif // HPSLOTWIDGET_H
