#include "saveloadwidget.h"

SaveLoadWidget::SaveLoadWidget(QWidget *parent, KeyboardControl *key,
                               SaveOrLoad saveOrLoad)
    : QWidget(parent), bar(NULL), key(key), currentSelectIndex(0),
      type(saveOrLoad), saveFileCount(0), currentBottomIndex(0),
      currentTopIndex(0) {
  // this->setWidget(fileWidget);
  this->show();
  this->setFocus();
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setGeometry(0, 0, WINDOWWIDTH, WINDOWHEIGHT * 2);

  QDir directory(QApplication::applicationDirPath());
  this->setMouseTracking(true);
  if (!directory.exists("save")) {
    directory.mkdir("save");
    saveFileCount = 0;
  } else {
    directory.cd("save");
    directory.setFilter(QDir::Files);
    directory.setSorting(QDir::Time);
    QStringList filters;
    filters << "*.sav";
    directory.setNameFilters(filters);
    // const QFileInfoList *list =
    // d.entryInfoList(filters,QDir::Files,QDir::Name);
    QFileInfoList list = directory.entryInfoList();
    int roleNum;
    QString time;
    for (int i = 0; i < list.size(); ++i) {
      DBIO dbio;
      QFileInfo fileInfo = list.at(i);
      if (dbio.isASaveFile(fileInfo.fileName(), time, roleNum) &&
          saveFileCount < 101) {
        saveFileCount++;
        saveFileVec.push_back(SaveFileData(fileInfo.fileName(), time, roleNum));
      }
    }
    //        QFileInfoList::iterator it( *list );
    //        QFileInfo *fi;
    //        DBIO dbio();
    //        while ( (fi = it.current()) != 0 )
    //        {
    //            if(dbio.isASaveFile(fi->fileName()))qDebug()<<fi->fileName();
    //        }
  }
  currentTopIndex = 0;
  currentBottomIndex = (saveFileCount + static_cast<int>(type) >=
                        WINDOWHEIGHT / SINGLEFILEHEIGHT)
                           ? (WINDOWHEIGHT / SINGLEFILEHEIGHT - 1)
                           : (saveFileCount + static_cast<int>(type) - 1);
  if (currentBottomIndex < 0) // loadWidget当存档文件为0的时候直接返回
  {
    return;
  }
  bar = new QScrollBar(this);
  bar->setGeometry(WINDOWWIDTH - 20, 0, 20, WINDOWHEIGHT);
  bar->setMinimum(0);
  bar->setMaximum(saveFileCount + static_cast<int>(type) - 1);
  bar->setValue(0);
  if (saveFileCount + static_cast<int>(type) + 1 >
      WINDOWHEIGHT / SINGLEFILEHEIGHT)
    bar->show();
  connect(bar, SIGNAL(valueChanged(int)), this, SLOT(onScrollBarChanged(int)));
  //    qDebug()<<saveFileCount;
  //    qDebug()<<saveFileVec[0];
}
void SaveLoadWidget::onScrollBarChanged(int value) {
  //只有存档数量多于单个窗口显示数量时候才存在滚动条
  if (!(bar->isSliderDown()))
    return; //证明不是鼠标事件引起的sliderValue change
  currentSelectIndex = value;
  currentTopIndex = value + WINDOWHEIGHT / SINGLEFILEHEIGHT - 1 <
                            saveFileCount + static_cast<int>(type)
                        ? (value)
                        : (saveFileCount + static_cast<int>(type) -
                           WINDOWHEIGHT / SINGLEFILEHEIGHT);
  currentBottomIndex = currentTopIndex + WINDOWHEIGHT / SINGLEFILEHEIGHT - 1;
  this->update();
}
void SaveLoadWidget::indexSelected() {
  if (type == OperationSave) {
    if (currentSelectIndex < saveFileCount) //说明不是空档位
    {
      if (QMessageBox::warning(this, "警告", "存档已存在,是否覆盖?",
                               QMessageBox::Yes | QMessageBox::No) ==
          QMessageBox::Yes) {
        // qDebug()<<QApplication::applicationDirPath()+QString("/save/")+saveFileVec[currentSelectIndex].fileName;
        if (!QFile::remove(QApplication::applicationDirPath() +
                           QString("/save/") +
                           saveFileVec[currentSelectIndex].fileName)) {
          QMessageBox::warning(this, "警告", "覆盖存档失败,请重试!",
                               QMessageBox::Yes);
          return;
        }
      }
    }
    emit saveToFile();
  } else {
    // qDebug()<<saveFileVec[currentSelectIndex];
    if (saveFileVec.size() != 0)
      emit loadFrom(saveFileVec[currentSelectIndex].fileName);
  }
  // this->parentWidget()->setFocus();
  // this->clearFocus();
  this->close();
}
SaveLoadWidget::~SaveLoadWidget() { delete bar; }
void SaveLoadWidget::keyPressEvent(QKeyEvent *event) {}
void SaveLoadWidget::indexDecrease() {
  if (currentSelectIndex == currentTopIndex) {
    if (currentTopIndex == 0) {
      currentBottomIndex = saveFileCount + static_cast<int>(type) - 1;
      currentSelectIndex = currentBottomIndex;
      currentTopIndex = (saveFileCount + static_cast<int>(type) >=
                         WINDOWHEIGHT / SINGLEFILEHEIGHT)
                            ? (saveFileCount + static_cast<int>(type) -
                               WINDOWHEIGHT / SINGLEFILEHEIGHT)
                            : (0);
    } else {
      currentTopIndex--;

      assert(saveFileCount + static_cast<int>(type) >
             WINDOWHEIGHT / SINGLEFILEHEIGHT);
      currentBottomIndex--;
      currentSelectIndex = currentTopIndex;
    }
  } else {
    currentSelectIndex--;
  }
  bar->setValue(currentSelectIndex);
}
void SaveLoadWidget::indexIncrease() {
  if (currentSelectIndex == currentBottomIndex) {
    if (currentBottomIndex == saveFileCount + static_cast<int>(type) - 1) {
      currentTopIndex = 0;
      currentSelectIndex = currentTopIndex;
      currentBottomIndex = (saveFileCount + static_cast<int>(type) >=
                            WINDOWHEIGHT / SINGLEFILEHEIGHT)
                               ? (WINDOWHEIGHT / SINGLEFILEHEIGHT - 1)
                               : (saveFileCount + static_cast<int>(type) - 1);
    } else {

      currentTopIndex++;
      currentBottomIndex++;
      currentSelectIndex = currentBottomIndex;
    }
  } else {
    currentSelectIndex++;
  }
  bar->setValue(currentSelectIndex);
}
void SaveLoadWidget::keyReleaseEvent(QKeyEvent *event) {
  if (currentBottomIndex < 0) // loadWidget当存档文件为0的时候直接返回
  {
    emit cancel();
    this->parentWidget()->setFocus();
    this->close();
    return;
  }
  if (event->isAutoRepeat())
    return;
  if (event->key() == key->upKey) {
    this->indexDecrease();
  } else if (event->key() == key->downKey) {
    this->indexIncrease();
  } else if (event->key() == key->actionKey) {
    this->indexSelected();
  } else if (event->key() == key->cancelKey) {
    this->parentWidget()->setFocus();
    emit cancel();
    this->close();
  }
  //    qDebug()<<currentTopIndex<<" "<<currentBottomIndex;
  //    qDebug()<<currentSelectIndex;
  this->update();
}
void SaveLoadWidget::paintEvent(QPaintEvent *) {

  // qDebug()<<currentSelectIndex<<" "<<currentTopIndex<<"
  // "<<currentBottomIndex;

  QPainter painter(this);
  painter.setBrush(QBrush(QColor(0, 51, 114, 224)));
  painter.drawRect(this->rect());
  if (currentBottomIndex < 0) // loadWidget当存档文件为0的时候直接返回
  {
    painter.setPen(QPen(QBrush(QColor(Qt::white)), 4));
    painter.setRenderHints(
        QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);
    painter.drawText(0, 0, WINDOWWIDTH, WINDOWHEIGHT, Qt::AlignCenter,
                     "没有存档");

    return;
  }

  assert(currentSelectIndex >= currentTopIndex &&
         currentSelectIndex <= currentBottomIndex);

  painter.setPen(QPen(QBrush(QColor(Qt::white)), 4));
  painter.setBrush(Qt::NoBrush);
  painter.setRenderHints(
      QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);
  for (int i = currentTopIndex; i < currentBottomIndex + 1; ++i) {
    painter.drawRoundedRect(10, (i - currentTopIndex) * SINGLEFILEHEIGHT + 10,
                            WINDOWWIDTH - 40, SINGLEFILEHEIGHT - 20, 5, 5);
    if (i < saveFileVec.size()) {
      painter.drawText(0, (i - currentTopIndex) * SINGLEFILEHEIGHT, WINDOWWIDTH,
                       SINGLEFILEHEIGHT / 2, Qt::AlignCenter,
                       QString("存档%1").arg(i + 1));
      painter.drawText(
          0, (i - currentTopIndex) * SINGLEFILEHEIGHT + SINGLEFILEHEIGHT / 2,
          WINDOWWIDTH, SINGLEFILEHEIGHT / 2, Qt::AlignHCenter | Qt::AlignTop,
          QString("存档时间:%1\n英雄数量:%2")
              .arg(saveFileVec[i].time)
              .arg(saveFileVec[i].roleNum));
    }
  }
  painter.setPen(QPen(QBrush(QColor(233, 255, 85)), 4));
  painter.drawRoundedRect(
      5, (currentSelectIndex - currentTopIndex) * SINGLEFILEHEIGHT + 5,
      WINDOWWIDTH - 30, SINGLEFILEHEIGHT - 10, 5, 5);
  //    painter.setPen(QPen(QColor(255,255,255)));
  //    for(int i=0;i<10;++i)
  //    {
  //        painter.drawText(0,i*96,96,96,Qt::AlignCenter,QString("%1").arg(i));
  //    }
  //    QPainter(this->fileWidget);
  //    this->fileWidget->setGeometry(0,0,FILEWIDGETWIDTH,(saveFileCount+1)*SINGLEFILEHEIGHT);
  //    this->ensureVisible();
}
void SaveLoadWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  currentSelectIndex =
      (currentTopIndex + (event->pos().y()) / SINGLEFILEHEIGHT <
       (saveFileCount + static_cast<int>(type)))
          ? (currentTopIndex + (event->pos().y()) / SINGLEFILEHEIGHT)
          : (saveFileCount + static_cast<int>(type) - 1);
  this->indexSelected();
}
void SaveLoadWidget::mouseMoveEvent(QMouseEvent *event) {
  /// qDebug()<<event->pos().x()<<" "<<event->pos().y();
  currentSelectIndex =
      (currentTopIndex + (event->pos().y()) / SINGLEFILEHEIGHT <
       (saveFileCount + static_cast<int>(type)))
          ? (currentTopIndex + (event->pos().y()) / SINGLEFILEHEIGHT)
          : (saveFileCount + static_cast<int>(type) - 1);
  this->update();
}
void SaveLoadWidget::wheelEvent(QWheelEvent *event) {
  // qDebug()<<currentTopIndex;
  if (event->delta() > 0 && currentTopIndex != 0)
    this->indexDecrease();
  else if (event->delta() < 0 &&
           currentBottomIndex != saveFileCount + static_cast<int>(type) - 1)
    this->indexIncrease();
  else if (event->delta() < 0 &&
           currentSelectIndex < saveFileCount + static_cast<int>(type) - 1) {
    currentSelectIndex++;
    bar->setValue(currentSelectIndex);
  }

  // currentSelectIndex=currentTopIndex+(event->pos().y())/SINGLEFILEHEIGHT;
  this->update();
  // qDebug()<<bar->isSliderDown();
}
