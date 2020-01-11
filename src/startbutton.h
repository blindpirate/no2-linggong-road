#ifndef STARTBUTTON_H
#define STARTBUTTON_H

#include "lgl.h"
#define ANIMATIONTIMERINTERVAL 50

class StartButton : public QWidget
{
    Q_OBJECT
public:
    explicit StartButton(QWidget*,const QString &,QString const &,int ,int );
    bool getPressed()const{return isPressed;}
signals:
    void clicked();
    
public slots:
private:
    QPixmap pressed;
    QPixmap normal;
//    QPixmap lgl;
    bool isPressed;
//    int animationTimerID;
//    int currentDisplayHeight;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
//    void timerEvent(QTimerEvent *);
};

#endif // STARTBUTTON_H
