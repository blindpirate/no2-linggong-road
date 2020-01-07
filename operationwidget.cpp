#include "operationwidget.h"

OperationWidget::OperationWidget(QWidget *parent):
    QWidget(parent),
    currentSelectedIndex(0),
//    explainationTimerID(0),
    x(0),
    y(0)
{
    this->hide();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    walk.load(QApplication::applicationDirPath()+QString("/data/image/other/walk.png"));
    attack.load(QApplication::applicationDirPath()+QString("/data/image/other/attack.png"));
    recover.load(QApplication::applicationDirPath()+QString("/data/image/other/recover.png"));
    goods.load(QApplication::applicationDirPath()+QString("/data/image/other/goods.png"));
    if(walk.isNull()||
            attack.isNull()||
            recover.isNull()||
            goods.isNull())quitApp(ERRORIMAGEFILENOTFOUND);
}

void OperationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing,true);
    painter.setPen(QPen(QBrush(QColor(Qt::gray)),2));
    painter.setBrush(QBrush(QColor(7,82,121,224)));//靛蓝
    painter.drawRoundedRect(x,y,100,28,4,4);
    //画外框
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(157,41,50)));//胭脂

    painter.drawRoundedRect(x+2+24*currentSelectedIndex,y+2,24,24,2,2);
    //画选中框

    painter.drawPixmap(x+2,y+2,24,24,attack);
    painter.drawPixmap(x+26,y+2,24,24,walk);
    painter.drawPixmap(x+50,y+2,24,24,recover);
    painter.drawPixmap(x+74,y+2,24,24,goods);
}


//void OperationWidget::setIndex(int value)
//{
//    assert(this->isVisible());
//    currentSelectedIndex=value;
//    if(explainationTimerID!=0)killTimer(explainationTimerID);
//    explainationTimerID=startTimer(EXPLAINATIONINTERVAL);
//    showExplaination=false;
//    this->update();
//}
//void OperationWidget::timerEvent(QTimerEvent *event)
//{
//    assert(this->isVisible());
//    showExplaination=true;
//    killTimer(explainationTimerID);
//    explainationTimerID=0;
//    this->update();
//}

void OperationWidget::show(int rolePointX,int rolePointY,int index)
{
//    assert(explainationTimerID==0);
//    explainationTimerID=startTimer(EXPLAINATIONINTERVAL);
    currentSelectedIndex=index;
//    showExplaination=false;

    x=rolePointX;
    y=rolePointY+36;

    this->setVisible(true);
    this->raise();
    this->update();
}

void OperationWidget::hide()
{
    //assert(this->isVisible());
//    killTimer(explainationTimerID);
//    explainationTimerID=0;
    this->setVisible(false);
    this->update();
}
