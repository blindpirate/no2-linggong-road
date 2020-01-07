#ifndef FADESCENE_H
#define FADESCENE_H

#include "lgl.h"
#define FADEINTERVAL 40

class FadeScene : public QWidget
{
    Q_OBJECT
public:
    explicit FadeScene(QWidget *parent = 0);
    
signals:
    
public slots:
private:
    int fadeTimerID;
    uchar currentAlpha;
    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);
    
};

#endif // FADESCENE_H
