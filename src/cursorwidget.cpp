#include "cursorwidget.h"

CursorWidget::CursorWidget(QWidget *parent)
    : QWidget(parent), flashTimerID(startTimer(CURSORANIMATIONINTERVAL)),
      moveTimerID(0), cursorIsBig(true),
      currentPointX(WINDOWWIDTH / BASEWIDTH / 2 * BASEWIDTH),
      currentPointY(WINDOWHEIGHT / BASEHEIGHT / 2 * BASEHEIGHT), moveEndX(0),
      moveEndY(0) {
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setGeometry(WINDOWWIDTH / BASEWIDTH / 2 * BASEWIDTH,
                    WINDOWWIDTH / BASEHEIGHT / 2 * BASEWIDTH, BASEWIDTH,
                    BASEHEIGHT);
  cursor.load(QApplication::applicationDirPath() +
              QString("/data/image/other/cursor.png"));
  if (cursor.isNull())
    quitApp(ERRORIMAGEFILENOTFOUND);
}

void CursorWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  if (cursorIsBig)
    painter.drawPixmap(0, 0, BASEWIDTH, BASEHEIGHT, cursor);
  else
    painter.drawPixmap(1, 1, BASEWIDTH - 2, BASEHEIGHT - 2, cursor);
}

void CursorWidget::timerEvent(QTimerEvent *event) {
  if (event->timerId() == flashTimerID) {
    cursorIsBig = !cursorIsBig;
  } else if (event->timerId() == moveTimerID) {
    int dis = sqrt(static_cast<double>(
        ((currentPointX - moveEndX) * (currentPointX - moveEndX) +
         (currentPointY - moveEndY) * (currentPointY - moveEndY))));

    if (dis < CURSORMOVEPIXEL) {
      currentPointX = moveEndX;
      currentPointY = moveEndY;
      emit arrivedDestination();
      killTimer(moveTimerID);
      moveTimerID = 0;
    } else {
      currentPointX += (currentPointX > moveEndX ? -1 : 1) *
                       abs(currentPointX - moveEndX) * CURSORMOVEPIXEL / dis;
      currentPointY += (currentPointY > moveEndY ? -1 : 1) *
                       abs(currentPointY - moveEndY) * CURSORMOVEPIXEL / dis;
    }
    this->setGeometry(currentPointX, currentPointY, BASEWIDTH, BASEHEIGHT);
  }
  this->update();
}

void CursorWidget::moveToPointOfView(int x, int y) {
  currentPointX = x;
  currentPointY = y;
  this->setGeometry(x, y, BASEWIDTH, BASEHEIGHT);
  this->raise();
  this->update();
}

void CursorWidget::moveToPointOfViewWithAnimation(int x, int y) {
  this->raise();
  assert(x >= 0 && x <= WINDOWWIDTH - BASEWIDTH);
  assert(y >= 0 && y <= WINDOWHEIGHT - BASEHEIGHT);
  assert(moveTimerID == 0);
  if (this->pos() == QPoint(x, y)) {
    emit arrivedDestination();
    return;
  }
  moveEndX = x;
  moveEndY = y;
  moveTimerID = startTimer(CURSORMOVEINTERVAL);
}

// void CursorWidget::moveFollowAI(DIRECTION dir,int x,int y)
//{
//    moveToPointOfView(x,y);
//}

// void CursorWidget::show()
//{
////    if(timerID==0)startTimer(CURSORANIMATIONINTERVAL);
//    this->setVisible(true);
//}

// void CursorWidget::hide()
//{
////    killTimer(timerID);
////    timerID=0;
//    this->setVisible(false);
//}
