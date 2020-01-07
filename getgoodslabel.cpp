#include "getgoodslabel.h"
#include "assert.h"

GetGoodsLabel::GetGoodsLabel(QWidget *parent,QString const &goodsName) :
    QWidget(parent)
{
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    this->setAttribute(Qt::WA_DeleteOnClose);
    text=goodsName;
    this->show();
    this->setFocus();
    this->raise();
}
void GetGoodsLabel::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
    {
        parentWidget()->setFocus();
        this->close();
    }

}
void GetGoodsLabel::keyPressEvent(QKeyEvent *)
{
}
void GetGoodsLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::black,2));
    painter.setRenderHints(QPainter::Antialiasing
                              | QPainter::HighQualityAntialiasing, true);//反锯齿
    QRectF rectangle((WINDOWWIDTH-GETGOODSLABELW)/2,
                     (WINDOWHEIGHT-GETGOODSLABELH)/6,
                     GETGOODSLABELW,
                     GETGOODSLABELH);
    painter.drawRoundedRect(rectangle,4,4);
    painter.drawText((WINDOWWIDTH-GETGOODSLABELW)/2,
                     (WINDOWHEIGHT-GETGOODSLABELH)/6,
                     GETGOODSLABELW,
                     GETGOODSLABELH,
                     Qt::AlignCenter,
                     text);

   // QRectF rectangle(0.0, 0.0, 80.0, 60.0);

    // QPainter painter(this);
    // painter.drawRoundedRect(rectangle, 20.0, 15.0);

}
