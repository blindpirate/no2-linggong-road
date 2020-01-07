#include "startbutton.h"

//StartButton::StartButton(QObject *parent) :
//    QPushButton(parent)
//{
//}
StartButton::StartButton(QWidget*parent, const QString &pressedPixmap, const QString &normalPixmap,int x, int y):
    QWidget(parent),
    pressed(QPixmap(QApplication::applicationDirPath() +QString("/data/image/other/%1.png").arg(pressedPixmap))),
    normal(QPixmap(QApplication::applicationDirPath() +QString("/data/image/other/%1.png").arg(normalPixmap))),
//    lgl(QPixmap(QCoreApplication::applicationDirPath() +QString("/data/image/other/lgl2.png"))),
    isPressed(false)
//    animationTimerID(startTimer(ANIMATIONTIMERINTERVAL)),
//    currentDisplayHeight(0)
{
    if(pressed.isNull()||normal.isNull())quitApp(ERRORIMAGEFILENOTFOUND);
    //assert(pressed.width()==normal.width()&&pressed.height()==normal.height());
    this->setGeometry(x,y,pressed.width(),pressed.height());
    this->installEventFilter(parent);
    this->setMouseTracking(true);
    //this->setFocus();
//    QPropertyAnimation *animation=new QPropertyAnimation(this,"windowOpacity");
//    animation->setDuration(1000);
//    animation->setStartValue(0.0);
//    animation->setEndValue(1.0);
//    animation->start();
}
void StartButton::mousePressEvent(QMouseEvent *e)
{
//    if(animationTimerID)
//    {
//        killTimer(animationTimerID);
//        animationTimerID=0;
//    }
    isPressed=true;
    update();
}

void StartButton::mouseReleaseEvent(QMouseEvent *e)
{
//    if(animationTimerID)
//    {
//        killTimer(animationTimerID);
//        animationTimerID=0;
//    }
    if(!isPressed)return;
    isPressed=false;
    this->update();
    emit clicked();
}

void StartButton::mouseMoveEvent(QMouseEvent *e)
{
    if((e->x())>(this->rect().width())
            ||(e->x())<0
            ||(e->y())>(this->rect().height())
            ||(e->y())<0)
        isPressed=false;
    else isPressed=true;
    this->update();
}
void StartButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(isPressed)
        painter.drawPixmap(0,0,pressed);
    else painter.drawPixmap(0,0,normal);
}
//void StartButton::timerEvent(QTimerEvent *)
//{//默认是动画定时器
//    if(currentDisplayHeight>lgl.height())
//    {
//        killTimer(animationTimerID);
//        animationTimerID=0;
//        return;
//    }
//    currentDisplayHeight+=lgl.height()/10.0;
//}
