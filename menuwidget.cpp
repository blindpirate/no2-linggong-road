#include "menuwidget.h"

MenuWidget::MenuWidget(QWidget *parent, KeyboardControl *key,
                       StoryMenuOrBattleMenu which)
    : QWidget(parent), currentIndex(0), key(key) {
  this->show();
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setGeometry(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
  this->setFocus();
  if (which == StoryMenu) {
    indexNum = 4;
    font = QFont(QString("悦黑 - yolan"), 16);
  } else {
    font = QFont(QString("悦黑 - yolan"), 14);
    indexNum = 5;
  }
  // this->setGeometry((WINDOWWIDTH-MENUWIDTH)/2,(WINDOWHEIGHT-MENUHEIGHT)/2,MENUWIDTH,MENUHEIGHT);
}

void MenuWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  if (indexNum == 4) {
    painter.setBrush(QBrush(QColor(46, 78, 125, 192)));
    painter.setPen(QPen(QBrush(QColor(62, 237, 232)), 2));
    painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::HighQualityAntialiasing,
                           true); //反锯齿
    painter.drawRoundedRect((WINDOWWIDTH - MENUWIDTH) / 2,
                            (WINDOWHEIGHT - MENUHEIGHT) / 2, MENUWIDTH,
                            MENUHEIGHT, 5, 5);
    painter.setPen(QPen(QColor(241, 252, 254)));
    painter.setFont(font);
    painter.drawText((WINDOWWIDTH - MENUWIDTH) / 2,
                     (WINDOWHEIGHT - MENUHEIGHT) / 2, MENUWIDTH, MENUHEIGHT,
                     Qt::AlignCenter, QString("状态\n存档\n读档\n回主菜单"));
    painter.setPen(QColor(Qt::white));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect((WINDOWWIDTH - MENUWIDTH) / 2 + 5,
                            (WINDOWHEIGHT - MENUHEIGHT) / 2 +
                                currentIndex * (MENUHEIGHT / 4 - 5) + 10,
                            MENUWIDTH - 10, MENUHEIGHT / 4 - 6, 3, 3);
  } else {
    painter.setBrush(QBrush(QColor(46, 78, 125, 192)));
    painter.setPen(QPen(QBrush(QColor(62, 237, 232)), 2));
    painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::HighQualityAntialiasing,
                           true); //反锯齿
    painter.drawRoundedRect((WINDOWWIDTH - MENUWIDTH) / 2,
                            (WINDOWHEIGHT - MENUHEIGHT) / 2, MENUWIDTH,
                            MENUHEIGHT, 5, 5);
    painter.setPen(QPen(QColor(241, 252, 254)));
    painter.setFont(font);
    painter.drawText((WINDOWWIDTH - MENUWIDTH) / 2,
                     (WINDOWHEIGHT - MENUHEIGHT) / 2, MENUWIDTH, MENUHEIGHT,
                     Qt::AlignCenter,
                     QString("回合结束\n存档\n读档\n战斗目的\n打不过跑"));
    painter.setPen(QColor(Qt::white));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect((WINDOWWIDTH - MENUWIDTH) / 2 + 5,
                            (WINDOWHEIGHT - MENUHEIGHT) / 2 +
                                currentIndex * (MENUHEIGHT / 4 - 8) + 5,
                            MENUWIDTH - 10, MENUHEIGHT / 4 - 6, 3, 3);
  }
}

void MenuWidget::keyPressEvent(QKeyEvent *) {}
void MenuWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    return;
  if (event->key() == key->upKey) {
    if (currentIndex == 0)
      currentIndex = indexNum - 1;
    else
      currentIndex--;
  } else if (event->key() == key->downKey) {
    if (currentIndex == indexNum - 1)
      currentIndex = 0;
    else
      currentIndex++;
  } else if (event->key() == key->actionKey) {
    emit selectIndex(currentIndex);
    // this->parentWidget()->setFocus();
    this->close();
  } else if (event->key() == key->cancelKey || event->key() == key->menuKey) {
    this->parentWidget()->setFocus();
    this->close();
  }
  this->update();
}
