
#include "battlelabel.h"

BattleLabel::BattleLabel(QWidget *parent,QString const&text) :
    QWidget(parent),
    text(text),
    font(QString("微软雅黑"),30)
{
    this->setGeometry((WINDOWWIDTH-BATTLELABELWIDTH)/2,
                      (WINDOWHEIGHT-BATTLELABELHEIGHT)/2,
                      BATTLELABELWIDTH,
                      BATTLELABELHEIGHT);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
    propertyAnimation=new QPropertyAnimation(this,"currentY",this);
    propertyAnimation->setDuration(2000);
    propertyAnimation->setStartValue(-BATTLELABELHEIGHT);
    propertyAnimation->setKeyValueAt(0.2,-BATTLELABELHEIGHT);
    propertyAnimation->setKeyValueAt(0.4,(WINDOWHEIGHT-BATTLELABELHEIGHT)/2);
    propertyAnimation->setKeyValueAt(0.6,(WINDOWHEIGHT-BATTLELABELHEIGHT)/2);
    propertyAnimation->setKeyValueAt(0.8,WINDOWHEIGHT);
    propertyAnimation->setEndValue(WINDOWHEIGHT);
    connect(propertyAnimation,SIGNAL(finished()),this,SLOT(close()));
    propertyAnimation->start();
    this->raise();
}

void BattleLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.setPen(QPen(QBrush(Qt::red),5));
    painter.setFont(font);
    painter.drawText(this->rect(),Qt::AlignCenter,text);
}
