#include "hpslotwidget.h"

HPSlotWidget::HPSlotWidget(QWidget *parent, int rolePointX, int rolePointY,
                           int startHP, int deltaHP, int fullHP,
                           AnimationOperation op)
    : QWidget(parent), startHP(startHP), deltaHP(deltaHP), fullHP(fullHP),
      currentHP(startHP),
      // delayTimerID(0),
      x(rolePointX), y(rolePointY), op(op),
      font(QFont(QString("方正舒体"), 12, QFont::Bold)) {
  assert(op == AniRoleRecover || op == AniRoleHurt);
  //    if(y<32)
  //    {
  //        pointX=x-9;
  //        pointY=y-9;
  //    }
  //    else
  //    {
  //        pointX=x-24;
  //        pointY=y+37;
  //    }

  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setGeometry(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
  this->show();
}

void HPSlotWidget::start(bool endWithSignal) {
  RoleAnimation *a = new RoleAnimation(this, x, y, op);
  a->start(false);

  x -= 14;
  y -= 20;
  //    if(rolePointX>32)
  //    {
  //        x=rolePointX-14;
  //        y=rolePointY-20;
  //    }
  //    else
  //    {
  //        x=rolePointX-14;
  //        y=rolePointY;
  //    }
  font.setLetterSpacing(QFont::AbsoluteSpacing, -1);

  if (op == AniRoleHurt) {
    QPropertyAnimation *animation =
        new QPropertyAnimation(this, "currentHP", this);
    animation->setStartValue(startHP);
    int endHP = startHP > deltaHP ? (startHP - deltaHP) : 0;
    animation->setKeyValueAt(0.4, endHP);
    animation->setEndValue(endHP);
    animation->setDuration(1000);

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

    QPropertyAnimation *rectAnimation =
        new QPropertyAnimation(this, "numRect", this);
    rectAnimation->setDuration(2000);
    rectAnimation->setStartValue(QRect(x + 14, y + 20, 32, 32));
    rectAnimation->setKeyValueAt(0.1, QRect(x + 14, y + 4, 32, 32));
    rectAnimation->setKeyValueAt(0.2, QRect(x + 14, y + 20, 32, 32));
    rectAnimation->setKeyValueAt(0.25, QRect(x + 14, y + 36, 32, 32));
    rectAnimation->setKeyValueAt(0.3, QRect(x + 14, y + 23, 32, 32));
    rectAnimation->setKeyValueAt(0.34, QRect(x + 14, y + 28, 32, 32));
    rectAnimation->setKeyValueAt(0.37, QRect(x + 14, y + 36, 32, 32));
    rectAnimation->setKeyValueAt(0.4, QRect(x + 14, y + 32, 32, 32));
    rectAnimation->setEndValue(QRect(x + 14, y + 32, 32, 32));

    connect(rectAnimation, SIGNAL(finished()), rectAnimation,
            SLOT(deleteLater()));

    connect(rectAnimation, SIGNAL(finished()), this, SLOT(close()));
    if (endWithSignal)
      connect(rectAnimation, SIGNAL(finished()), this->parent(),
              SLOT(startAnimation()));

    animation->start();
    rectAnimation->start();
  } else {
    QPropertyAnimation *animation =
        new QPropertyAnimation(this, "currentHP", this);
    animation->setStartValue(startHP);
    int endHP = startHP + deltaHP > fullHP ? (fullHP) : (startHP + deltaHP);
    animation->setKeyValueAt(0.4, endHP);
    animation->setEndValue(endHP);
    animation->setDuration(1000);

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
    if (endWithSignal)
      connect(animation, SIGNAL(finished()), this->parent(),
              SLOT(startAnimation()));
    animation->start();
  }
  this->update();
}

void HPSlotWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::HighQualityAntialiasing,
                         true); //反锯齿
  if (currentHP * 1.0 / fullHP < 0.2)
    painter.setBrush(QBrush(QColor(Qt::red)));
  else if (currentHP * 1.0 / fullHP < 0.7)
    painter.setBrush(QBrush(QColor(Qt::yellow)));
  else
    painter.setBrush(QBrush(QColor(Qt::green)));
  painter.drawRoundedRect(x + 2, y + 8, currentHP * 1.0 / fullHP * 56, 3, 1, 1);
  painter.setPen(QPen(QColor(Qt::white), 1));
  painter.setBrush(Qt::NoBrush);
  painter.drawRoundedRect(x + 2, y + 8, 56, 3, 1, 1);

  font.setPointSize(12);
  painter.setFont(font);
  painter.drawText(x + 61, y + 13, QString("%1").arg(currentHP));
  if (op == AniRoleHurt) {
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(this->numRect, Qt::AlignCenter,
                     QString("%1").arg(deltaHP));
  }
}

// void HPSlotWidget::timerEvent(QTimerEvent *)
//{
//    killTimer(delayTimerID);
//    delayTimerID=0;

//}
