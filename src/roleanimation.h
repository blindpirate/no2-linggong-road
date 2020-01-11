/************************************
  该类实现Role的各类动画效果.调用完毕自动销毁
*************************************/

#ifndef ROLEANIMATION_H
#define ROLEANIMATION_H
#include "lgl.h"
#define ROLEANIMATION_INTERVAL 80
class RoleAnimation:public QWidget
{
    Q_OBJECT
public:
    explicit RoleAnimation(QWidget *parent,int rolePointX,int rolePointY,AnimationOperation op);
    //rolePointX指的是发生动画时间的role左上角点窗口坐标
    //~RoleAnimation();
    void start(bool endWithSignal);
signals:
    void animationEnd();
private:
    QPixmap image;
    int frameNum;
    int currentFrameNum;
    int animationTimerID;
    int x;
    int y;//这是本widget的绘图原点
    int frameSize;
    AnimationOperation type;
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
};

inline void RoleAnimation::start(bool endWithSignal)
{
    assert(!image.isNull());
    assert(image.width()%image.height()==0);
    assert(animationTimerID==0);
    this->raise();
    frameNum=image.width()/image.height();
    frameSize=image.height();
    x-=(frameSize-BASEWIDTH)/2;
    y-=(frameSize-BASEHEIGHT)/2;
    animationTimerID=startTimer(ROLEANIMATION_INTERVAL);
    if(endWithSignal)
        connect(this,SIGNAL(animationEnd()),this->parent(),SLOT(startAnimation()));
    this->update();
}





/*
class RoleSweat : public QWidget
{//父窗口是roleView
    Q_OBJECT
public:
    explicit RoleSweat(QWidget *parent);
    QPropertyAnimation *animation;//设置为公有是为了连接外部槽
signals:
private:
    QPixmap sweatImage;
    void paintEvent(QPaintEvent *);
};
*/
/*
class RoleAttack:public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPixmap attackImage READ getAttackImage WRITE setAttackImage)
public:
    QPropertyAnimation *animation;
    explicit RoleAttack(QWidget*parent=0);
    //~RoleAttack(){delete attackImage1;delete attackImage2;}
private:
    //QPixmap *attackImage1;
    //QPixmap *attackImage2;
    //QPixmap attackImage;
    void paintEvent(QPaintEvent *);
};
*/
/*
class RoleRecovery:public QWidget
{
    Q_OBJECT
public:
    explicit RoleRecovery(QWidget*parent=0);
};
class RoleHurt:public QWidget
{
    Q_OBJECT
public:
    explicit RoleHurt(QWidget *parent=0);
};
class AnimationWidget:public QWidget
{
    Q_OBJECT
public:
    explicit AnimationWidget(QWidget *parent,int rolePointX,int rolePointY,QString const &fileName);
    void start();
    ~AnimationWidget();
private:
    QPixmap *image;
    int frameNum;
    int currentFrameNum;
    int animationTimerID;
    int x;
    int y;
    int animationSize;
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
};
class NumberWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NumberWidget(QWidget *parent,int number,int roleViewPointX,int roleViewPointY);

signals:

public slots:
    void start();
private:
    QString numberString;
    void paintEvent(QPaintEvent *);
    QFont font;

};
*/

#endif // ROLESWEAT_H
