#ifndef CURSORWIDGET_H
#define CURSORWIDGET_H

#include "lgl.h"
#define CURSORANIMATIONINTERVAL 200
#define CURSORMOVEINTERVAL 40
#define CURSORMOVEPIXEL 20

class CursorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CursorWidget(QWidget *parent = 0);
    void moveToPointOfView(int x,int y);
    void moveToPointOfViewWithAnimation(int x,int y);//用于动画结束之后定位到某个点上
//    void hide();
//    void show();
    int getViewPointX(){return currentPointX;}
    int getViewPointY(){return currentPointY;}
signals:
    void arrivedDestination();
    
public slots:
    //void moveFollowAI(DIRECTION,int,int);
private:
    int flashTimerID;
    int moveTimerID;
    int currentPointX;
    int currentPointY;
    int moveEndX;
    int moveEndY;
    QPixmap cursor;
    bool cursorIsBig;
    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);

    
};

#endif // CURSORWIDGET_H
