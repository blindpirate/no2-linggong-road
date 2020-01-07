#include "battleaimwidget.h"

BattleAimWidget::BattleAimWidget(QWidget *parent,QString &win,QString &fail) :
    QWidget(parent),
    winRequirement(win),
    failRequirement(fail)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    //this->raise();
    this->show();
    this->setFocus();
}
void BattleAimWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0,51,114,192)));
    painter.drawRect(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    painter.setPen(Qt::white);
    painter.drawText(0,0,WINDOWWIDTH,WINDOWHEIGHT,Qt::AlignCenter,
                     QString("胜利条件:%1\n失败条件:%2\n").arg(winRequirement).arg(failRequirement));
}
void BattleAimWidget::keyPressEvent(QKeyEvent *){}
void BattleAimWidget::keyReleaseEvent(QKeyEvent *)
{
    this->parentWidget()->setFocus();
    this->close();
}
