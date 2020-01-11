#include "fadescene.h"

FadeScene::FadeScene(QWidget *parent) :
    QWidget(parent),
    fadeTimerID(startTimer(FADEINTERVAL)),
    currentAlpha(255)
{
    this->setGeometry(0,0,parent->width(),parent->height());
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
}

void FadeScene::timerEvent(QTimerEvent *)
{
    //qDebug()<<currentAlpha;
    if(currentAlpha<50)
    {
        killTimer(fadeTimerID);
        fadeTimerID=0;
        //this->parentWidget()->setFocus();
        this->close();
    }
    else if(currentAlpha<100)
        currentAlpha-=30;
    else
        currentAlpha-=10;
    this->update();
}

void FadeScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0,0,0,currentAlpha)));
    painter.drawRect(0,0,this->width(),this->height());
}
