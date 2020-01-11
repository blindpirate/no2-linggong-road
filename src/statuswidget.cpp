#include "statuswidget.h"
//注意,武器剩下最后一个的时候不允许卸下

StatusWidget::StatusWidget(QWidget *parent, Role **roleInfo,
                           KeyboardControl *key, multiset<int> &carriedArticle,
                           map<int, ArticleInfo> &articleIDToInfoMap,
                           int &carriedCash)
    : QWidget(parent), roleInfo(roleInfo), key(key),
      pArticleIDToInfoMap(&articleIDToInfoMap),
      pCarriedArticleSet(&carriedArticle),
      infoWidget(new RoleInfoWidget(this, roleInfo)), currentRoleID(1),
      currentTabIndex(StatusRoleInfo), focus(StatusFocusOnTabIndex),
      currentItemSelectedIndex(0), currentItemIndex(0),
      currentSelectedItemID(0), currentEquipCount(0), currentAvailableCount(0),
      carriedCash(carriedCash) {
  this->show();
  this->setFocus();
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setGeometry(0, 0, WINDOWWIDTH, WINDOWHEIGHT);

  upArrow[0] = QPoint(10, 20);
  upArrow[1] = QPoint(5, 25);
  upArrow[2] = QPoint(15, 25); //向上的箭头
  downArrow[0] = QPoint(5, 30);
  downArrow[1] = QPoint(15, 30);
  downArrow[2] = QPoint(10, 35); //向下的箭头

  this->resetAvailableArticle();
}
void StatusWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setFont(QFont("微软雅黑", 12));
  painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::HighQualityAntialiasing,
                         true); //反锯齿
  painter.setBrush(QBrush(QColor(0, 51, 114, 240)));
  painter.drawRect(this->rect()); //先画个背景

  painter.setPen(Qt::yellow);
  painter.drawText(10, 430, 200, 40, Qt::AlignVCenter | Qt::AlignLeft,
                   QString("金钱 %1").arg(carriedCash));

  if (currentTabIndex == StatusRoleInfo) { //画白色上下箭头,否则画灰色上下箭头
    painter.setBrush(QBrush(Qt::white));
  } else {
    painter.setBrush(QBrush(Qt::gray));
  }
  painter.setPen(Qt::NoPen);
  painter.drawPolygon(upArrow, 3);
  painter.drawPolygon(downArrow, 3);

  painter.setPen(Qt::yellow);
  painter.drawText(20, 20, 60, 20, Qt::AlignCenter, "人物");
  painter.drawText(220, 20, 60, 20, Qt::AlignCenter, "武器");
  painter.drawText(280, 20, 60, 20, Qt::AlignCenter, "防具");
  painter.drawText(340, 20, 60, 20, Qt::AlignCenter, "药物");
  painter.drawText(400, 20, 60, 20, Qt::AlignCenter, "特殊");
  //画出来菜单
  painter.setPen(QPen(Qt::white, 3));
  painter.setBrush(Qt::NoBrush);
  if (currentTabIndex == StatusRoleInfo) {
    painter.drawRoundedRect(20, 20, 60, 20, 5, 5);
  } else {
    if (focus != StatusFocusOnTabIndex) {
      painter.setPen(Qt::gray);
    }
    painter.drawRoundedRect(160 + static_cast<int>(currentTabIndex) * 60, 20,
                            60, 20, 5, 5);
  }

  //画一个框
  painter.setPen(Qt::white);
  painter.setBrush(Qt::NoBrush);
  painter.drawRoundedRect(220, 55, 240, 150, 5, 5);  //已经装备的物品区
  painter.drawRoundedRect(220, 210, 240, 150, 5, 5); //携带的物品区
  painter.drawRoundedRect(220, 365, 240, 90, 5, 5);  //描述

  switch (currentTabIndex) {
  case StatusRoleInfo:
  case StatusWeapon: {
    painter.setPen(QColor(255, 32, 33));
    painter.drawText(230, 60, 140, 30, Qt::AlignCenter, QString("名称"));
    painter.drawText(350, 60, 40, 30, Qt::AlignCenter, QString("攻击"));
    painter.drawText(390, 60, 40, 30, Qt::AlignCenter, QString("命中"));

    painter.drawText(230, 215, 110, 30, Qt::AlignCenter, QString("名称"));
    painter.drawText(340, 215, 40, 30, Qt::AlignCenter, QString("攻击"));
    painter.drawText(380, 215, 40, 30, Qt::AlignCenter, QString("命中"));
    painter.drawText(420, 215, 30, 30, Qt::AlignCenter, QString("数量"));

    int currentPaintWeaponID = 0;
    painter.setPen(Qt::white);
    for (int i = 0; i < MAXWEAPONCOUNT; ++i) {
      currentPaintWeaponID = roleInfo[currentRoleID - 1]->weapon[i];
      if (!currentPaintWeaponID)
        break;
      painter.drawText(230, 90 + i * 30, 140, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintWeaponID].name);
      painter.drawText(
          350, 90 + i * 30, 40, 30, Qt::AlignCenter,
          QString("%1").arg(
              (*pArticleIDToInfoMap)[currentPaintWeaponID].attack));
      painter.drawText(
          390, 90 + i * 30, 40, 30, Qt::AlignCenter,
          QString("%1").arg(
              (*pArticleIDToInfoMap)[currentPaintWeaponID].accurcy));
    }
    //画武器框结束

    set<int>::iterator iter = availableArticleBeginIter;
    for (int i = 0; i < 4; ++i, ++iter) {
      if (availableArticleSet.size() < AVAILABLEARTICLECOUNT) {
        if (i == availableArticleSet.size())
          break;
      } else {
        if (iter == availableArticleSet.end())
          iter = availableArticleSet.begin();
      }
      currentPaintWeaponID = *iter;
      painter.drawText(230, 245 + i * 30, 110, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintWeaponID].name);
      painter.drawText(
          340, 245 + i * 30, 40, 30, Qt::AlignCenter,
          QString("%1").arg(
              (*pArticleIDToInfoMap)[currentPaintWeaponID].attack));
      painter.drawText(
          380, 245 + i * 30, 40, 30, Qt::AlignCenter,
          QString("%1").arg(
              (*pArticleIDToInfoMap)[currentPaintWeaponID].accurcy));
      painter.drawText(
          420, 245 + i * 30, 30, 30, Qt::AlignCenter,
          QString("%1").arg(pCarriedArticleSet->count(currentPaintWeaponID)));
    }
    //画可用武器区结束
    painter.setPen(QColor(242, 190, 70));
    if (currentSelectedItemID)
      painter.drawText(
          225, 365, 230, 90, Qt::TextWordWrap,
          (*pArticleIDToInfoMap)[currentSelectedItemID].description);

    //画武器描述区结束
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    if (focus == StatusFocusOnEquipment || focus == StatusFocusOnItemSelected) {
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) //在上面的框里
      {
        assert(currentItemIndex < 5);
        painter.drawRoundedRect(230, 65 + (currentItemIndex)*30, 220, 20, 2, 2);
        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] = QPoint(445, 75 + (currentItemIndex)*30);
        rightArrow[1] = QPoint(440, 70 + (currentItemIndex)*30);
        rightArrow[2] = QPoint(440, 80 + (currentItemIndex)*30);
        painter.drawPolygon(rightArrow, 3);
        // painter.drawText(225,365,230,90,Qt::TextWordWrap,pArticleIDToInfoMap[roleInfo[currentItemIndex-1]].description);
      } else {

        painter.drawRoundedRect(
            230, 220 + (currentItemIndex - currentEquipCount) * 30, 220, 20, 2,
            2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] =
            QPoint(448, 230 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[1] =
            QPoint(443, 235 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[2] =
            QPoint(443, 225 + (currentItemIndex - currentEquipCount) * 30);
        painter.drawPolygon(rightArrow, 3);
      }
      //画选中框
    }
    if (focus == StatusFocusOnItemSelected) {
      painter.setBrush(QBrush(QColor(23, 124, 176)));
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) {
        painter.drawRoundedRect(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                                4, 4);
        if (currentItemSelectedCount == 0)
          painter.setPen(Qt::gray);
        else
          painter.setPen(Qt::red);
        painter.drawText(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                         Qt::AlignCenter, "卸下");
      } else {
        painter.drawRoundedRect(
            380, 230 + (currentItemIndex - currentEquipCount) * 30, 80, 30, 2,
            2);
        if (currentItemSelectedCount == 0)
          painter.setPen(Qt::gray);
        else
          painter.setPen(Qt::red);
        painter.drawText(380, 230 + (currentItemIndex - currentEquipCount) * 30,
                         80, 30, Qt::AlignCenter, "装备");
      }
    } //画"装备/卸下菜单"
  } break;
  case StatusShield: {
    painter.setPen(QColor(255, 32, 33));
    painter.drawText(230, 60, 160, 30, Qt::AlignCenter, QString("名称"));
    painter.drawText(390, 60, 60, 30, Qt::AlignCenter, QString("防御"));

    painter.drawText(230, 215, 140, 30, Qt::AlignCenter, QString("名称"));
    painter.drawText(370, 215, 40, 30, Qt::AlignCenter, QString("防御"));
    painter.drawText(410, 215, 40, 30, Qt::AlignCenter, QString("数量"));

    painter.setPen(Qt::white);
    int currentPaintShieldID = 0;
    currentPaintShieldID = roleInfo[currentRoleID - 1]->shield;
    if (currentPaintShieldID) {

      painter.drawText(230, 90, 160, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintShieldID].name);
      painter.drawText(
          390, 90, 60, 30, Qt::AlignCenter,
          QString("%1").arg(
              (*pArticleIDToInfoMap)[currentPaintShieldID].attack));
    }
    //画武器框结束

    set<int>::iterator iter = availableArticleBeginIter;
    for (int i = 0; i < 4; ++i, ++iter) {
      if (availableArticleSet.size() < AVAILABLEARTICLECOUNT) {
        if (i == availableArticleSet.size())
          break;
      } else {
        if (iter == availableArticleSet.end())
          iter = availableArticleSet.begin();
      }

      currentPaintShieldID = *iter;
      painter.drawText(230, 245 + i * 30, 140, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintShieldID].name);
      painter.drawText(
          370, 245 + i * 30, 40, 30, Qt::AlignCenter,
          QString("%1").arg(
              (*pArticleIDToInfoMap)[currentPaintShieldID].attack));
      painter.drawText(
          410, 245 + i * 30, 40, 30, Qt::AlignCenter,
          QString("%1").arg(pArticleIDToInfoMap->count(currentPaintShieldID)));
    }
    //画可用武器区结束
    painter.setPen(QColor(242, 190, 70));
    if (currentSelectedItemID)
      painter.drawText(
          225, 365, 230, 90, Qt::TextWordWrap,
          (*pArticleIDToInfoMap)[currentSelectedItemID].description);
    //画武器描述区结束
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    if (focus == StatusFocusOnEquipment || focus == StatusFocusOnItemSelected) {
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) //在上面的框里
      {
        painter.drawRoundedRect(230, 65 + (currentItemIndex)*30, 220, 20, 2, 2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] = QPoint(445, 75 + (currentItemIndex)*30);
        rightArrow[1] = QPoint(440, 70 + (currentItemIndex)*30);
        rightArrow[2] = QPoint(440, 80 + (currentItemIndex)*30);
        painter.drawPolygon(rightArrow, 3);
      } else {
        painter.drawRoundedRect(
            230, 220 + (currentItemIndex - currentEquipCount) * 30, 220, 20, 2,
            2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] =
            QPoint(445, 230 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[1] =
            QPoint(440, 225 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[2] =
            QPoint(440, 235 + (currentItemIndex - currentEquipCount) * 30);
        painter.drawPolygon(rightArrow, 3);
      }

      //画选中框
    }
    if (focus == StatusFocusOnItemSelected) {
      painter.setBrush(QBrush(QColor(23, 124, 176)));
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) {
        painter.drawRoundedRect(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                                4, 4);
        if (currentItemSelectedCount == 0)
          painter.setPen(Qt::gray);
        else
          painter.setPen(Qt::red);
        painter.drawText(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                         Qt::AlignCenter, "卸下");
      } else {
        painter.drawRoundedRect(
            380, 230 + (currentItemIndex - currentEquipCount) * 30, 80, 30, 2,
            2);
        if (currentItemSelectedCount == 0)
          painter.setPen(Qt::gray);
        else
          painter.setPen(Qt::red);
        painter.drawText(380, 230 + (currentItemIndex - currentEquipCount) * 30,
                         80, 30, Qt::AlignCenter, "装备");
      }
    } //画"装备/卸下菜单"
  } break;
  case StatusMedicine: {
    painter.setPen(QColor(255, 32, 33));
    painter.drawText(230, 60, 220, 30, Qt::AlignCenter, QString("名称"));

    painter.drawText(230, 215, 160, 30, Qt::AlignCenter, QString("名称"));
    painter.drawText(390, 215, 60, 30, Qt::AlignCenter, QString("数量"));

    int currentPaintMedicineID = 0;
    painter.setPen(Qt::white);
    for (int i = 0; i < MAXMEDICINECOUNT; ++i) {
      currentPaintMedicineID = roleInfo[currentRoleID - 1]->medicine[i];
      if (!currentPaintMedicineID)
        break;
      painter.drawText(230, 90 + i * 30, 220, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintMedicineID].name);
    }
    //画武器框结束

    set<int>::iterator iter = availableArticleBeginIter;
    for (int i = 0; i < MAXMEDICINECOUNT; ++i, ++iter) {
      if (availableArticleSet.size() < AVAILABLEARTICLECOUNT) {
        if (i == availableArticleSet.size())
          break;
      } else {
        if (iter == availableArticleSet.end())
          iter = availableArticleSet.begin();
      }
      currentPaintMedicineID = *iter;
      painter.drawText(230, 245 + i * 30, 160, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintMedicineID].name);
      painter.drawText(
          390, 245 + i * 30, 60, 30, Qt::AlignCenter,
          QString("%1").arg(pCarriedArticleSet->count(currentPaintMedicineID)));
    }
    //画可用武器区结束
    painter.setPen(QColor(242, 190, 70));
    if (currentSelectedItemID)
      painter.drawText(
          225, 365, 230, 90, Qt::TextWordWrap,
          (*pArticleIDToInfoMap)[currentSelectedItemID].description);
    //画武器描述区结束
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    if (focus == StatusFocusOnEquipment || focus == StatusFocusOnItemSelected) {
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) //在上面的框里
      {
        painter.drawRoundedRect(230, 95 + (currentItemIndex - 1) * 30, 220, 20,
                                2, 2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] = QPoint(445, 75 + (currentItemIndex)*30);
        rightArrow[1] = QPoint(440, 70 + (currentItemIndex)*30);
        rightArrow[2] = QPoint(440, 80 + (currentItemIndex)*30);
        painter.drawPolygon(rightArrow, 3);
      } else {
        painter.drawRoundedRect(
            230, 220 + (currentItemIndex - currentEquipCount) * 30, 220, 20, 2,
            2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] =
            QPoint(445, 230 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[1] =
            QPoint(440, 225 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[2] =
            QPoint(440, 235 + (currentItemIndex - currentEquipCount) * 30);
        painter.drawPolygon(rightArrow, 3);
      }

      //画选中框
    }
    if (focus == StatusFocusOnItemSelected) {
      painter.setBrush(QBrush(QColor(23, 124, 176)));
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) {
        painter.drawRoundedRect(380, 105 + (currentItemIndex - 1) * 30, 80, 60,
                                4, 4);
        if (currentItemSelectedIndex == 0 &&
            currentItemSelectedCount == 2) { //两个菜单条目,都处于可选状态
          painter.setPen(Qt::red);
          painter.drawText(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                           Qt::AlignCenter, "卸下");
          painter.setPen(Qt::white);
          painter.drawText(380, 135 + (currentItemIndex - 1) * 30, 80, 30,
                           Qt::AlignCenter, "使用");
        } else if (currentItemSelectedIndex == 0 &&
                   currentItemSelectedCount ==
                       1) { //两个菜单条目,只有一个可选,另一个是灰色
          painter.setPen(Qt::red);
          painter.drawText(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                           Qt::AlignCenter, "卸下");
          painter.setPen(Qt::gray);
          painter.drawText(380, 135 + (currentItemIndex - 1) * 30, 80, 30,
                           Qt::AlignCenter, "使用");
        } else {
          painter.setPen(Qt::white);
          painter.drawText(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                           Qt::AlignCenter, "卸下");
          painter.setPen(Qt::red);
          painter.drawText(380, 135 + (currentItemIndex - 1) * 30, 80, 30,
                           Qt::AlignCenter, "使用");
        }
      } else {
        painter.drawRoundedRect(
            380, 230 + (currentItemIndex - currentEquipCount) * 30, 80, 60, 2,
            2);
        if (currentItemSelectedCount == 0) {
          painter.setPen(Qt::gray);
          painter.drawText(380,
                           230 + (currentItemIndex - currentEquipCount) * 30,
                           80, 30, Qt::AlignCenter, "装备");
          painter.setPen(Qt::gray);
          painter.drawText(380,
                           260 + (currentItemIndex - currentEquipCount) * 30,
                           80, 30, Qt::AlignCenter, "使用");
        } else if (currentItemSelectedCount ==
                   2) { //两个菜单条目,都处于可选状态
          if (currentItemSelectedIndex == 0) {
            painter.setPen(Qt::red);
            painter.drawText(380,
                             230 + (currentItemIndex - currentEquipCount) * 30,
                             80, 30, Qt::AlignCenter, "装备");
            painter.setPen(Qt::white);
            painter.drawText(380,
                             260 + (currentItemIndex - currentEquipCount) * 30,
                             80, 30, Qt::AlignCenter, "使用");
          } else {
            painter.setPen(Qt::white);
            painter.drawText(380,
                             230 + (currentItemIndex - currentEquipCount) * 30,
                             80, 30, Qt::AlignCenter, "装备");
            painter.setPen(Qt::red);
            painter.drawText(380,
                             260 + (currentItemIndex - currentEquipCount) * 30,
                             80, 30, Qt::AlignCenter, "使用");
          }

        } else if (
            currentEquipCount ==
            MAXMEDICINECOUNT) { //两个菜单条目,只有一个可选,另一个是灰色,这时候的itemIndex肯定是0,如果装备区是满的,证明现在亮的应该是使用
          painter.setPen(Qt::gray);
          painter.drawText(380,
                           230 + (currentItemIndex - currentEquipCount) * 30,
                           80, 30, Qt::AlignCenter, "装备");
          painter.setPen(Qt::red);
          painter.drawText(380,
                           260 + (currentItemIndex - currentEquipCount) * 30,
                           80, 30, Qt::AlignCenter, "使用");
        } else { //否则亮起来的应该是装备
          painter.setPen(Qt::red);
          painter.drawText(380,
                           230 + (currentItemIndex - currentEquipCount) * 30,
                           80, 30, Qt::AlignCenter, "装备");
          painter.setPen(Qt::gray);
          painter.drawText(380,
                           260 + (currentItemIndex - currentEquipCount) * 30,
                           80, 30, Qt::AlignCenter, "使用");
        }
      }
    } //画"装备/卸下菜单"
  } break;
  case StatusArticle: {
    painter.setPen(QColor(255, 32, 33));
    painter.drawText(230, 60, 220, 30, Qt::AlignCenter, QString("名称"));

    painter.drawText(230, 215, 160, 30, Qt::AlignCenter, QString("名称"));
    painter.drawText(390, 215, 60, 30, Qt::AlignCenter, QString("数量"));

    painter.setPen(Qt::white);
    int currentPaintArticleID = 0;
    currentPaintArticleID = roleInfo[currentRoleID - 1]->article;
    // qDebug()<<(*pArticleIDToInfoMap)[currentPaintArticleID].name;
    if (currentPaintArticleID) {
      painter.drawText(230, 90, 220, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintArticleID].name);
    }
    //画武器框结束

    set<int>::iterator iter = availableArticleBeginIter;
    for (int i = 0; i < 4; ++i, ++iter) {
      if (availableArticleSet.size() < AVAILABLEARTICLECOUNT) {
        if (i == availableArticleSet.size())
          break;
      } else {
        if (iter == availableArticleSet.end())
          iter = availableArticleSet.begin();
      }
      currentPaintArticleID = *iter;
      painter.drawText(230, 245 + i * 30, 160, 30, Qt::AlignCenter,
                       (*pArticleIDToInfoMap)[currentPaintArticleID].name);
      painter.drawText(
          390, 245 + i * 30, 60, 30, Qt::AlignCenter,
          QString("%1").arg(pArticleIDToInfoMap->count(currentPaintArticleID)));
    }
    //画可用武器区结束
    painter.setPen(QColor(242, 190, 70));
    if (currentSelectedItemID)
      painter.drawText(
          225, 365, 230, 90, Qt::TextWordWrap,
          (*pArticleIDToInfoMap)[currentSelectedItemID].description);
    //画武器描述区结束
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    if (focus == StatusFocusOnEquipment || focus == StatusFocusOnItemSelected) {
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) //在上面的框里
      {
        painter.drawRoundedRect(230, 65 + (currentItemIndex)*30, 220, 20, 2, 2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] = QPoint(445, 75 + (currentItemIndex)*30);
        rightArrow[1] = QPoint(440, 70 + (currentItemIndex)*30);
        rightArrow[2] = QPoint(440, 80 + (currentItemIndex)*30);
        painter.drawPolygon(rightArrow, 3);
      } else {
        painter.drawRoundedRect(
            230, 220 + (currentItemIndex - currentEquipCount) * 30, 220, 20, 2,
            2);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(QBrush(Qt::white));
        rightArrow[0] =
            QPoint(445, 230 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[1] =
            QPoint(440, 225 + (currentItemIndex - currentEquipCount) * 30);
        rightArrow[2] =
            QPoint(440, 235 + (currentItemIndex - currentEquipCount) * 30);
        painter.drawPolygon(rightArrow, 3);
      }

      //画选中框
    }
    if (focus == StatusFocusOnItemSelected) {
      painter.setBrush(QBrush(QColor(23, 124, 176)));
      painter.setPen(QPen(Qt::white, 3));
      if (currentItemIndex < currentEquipCount + 1) {
        painter.drawRoundedRect(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                                4, 4);
        if (currentItemSelectedCount == 0)
          painter.setPen(Qt::gray);
        else
          painter.setPen(Qt::red);
        painter.drawText(380, 105 + (currentItemIndex - 1) * 30, 80, 30,
                         Qt::AlignCenter, "卸下");
      } else {
        painter.drawRoundedRect(
            380, 230 + (currentItemIndex - currentEquipCount) * 30, 80, 30, 2,
            2);
        if (currentItemSelectedCount == 0)
          painter.setPen(Qt::gray);
        else
          painter.setPen(Qt::red);
        painter.drawText(380, 230 + (currentItemIndex - currentEquipCount) * 30,
                         80, 30, Qt::AlignCenter, "装备");
      }
    } //画"装备/卸下菜单"
  } break;
  default:
    qDebug() << currentTabIndex;
    assert(0);
  }
}
void StatusWidget::keyPressEvent(QKeyEvent *) {}
void StatusWidget::keyReleaseEvent(QKeyEvent *e) {
  if (e->isAutoRepeat())
    return;
  if (e->key() == key->actionKey) {
    this->onActionKey();
  } else if (e->key() == key->cancelKey) {
    this->onCancelKey();
  } else if (e->key() == key->menuKey) {
    this->parentWidget()->setFocus();
    this->close();
  } else if (e->key() == key->upKey) {
    this->onDirectionKey(UP);
  } else if (e->key() == key->downKey) {
    this->onDirectionKey(DOWN);
  } else if (e->key() == key->leftKey) {
    this->onDirectionKey(LEFT);
  } else if (e->key() == key->rightKey) {
    this->onDirectionKey(RIGHT);
  }
  this->update();
}
void StatusWidget::onActionKey() { //如果装备区满了不允许进行装备操作
  switch (focus) {
  case StatusFocusOnTabIndex:
    if (currentTabIndex == StatusRoleInfo)
      break;
    if (currentEquipCount + currentAvailableCount == 0)
      break;
    focus = StatusFocusOnEquipment;
    availableArticleBeginIter = availableArticleSet.begin();
    currentItemIndex = 1;
    this->updateSelectedItemID();
    break;
  case StatusFocusOnEquipment:
    //如果光标在装备区,那么显示使用/装备/卸下等
    switch (currentTabIndex) {
    case StatusWeapon:

      if ((currentItemIndex > currentEquipCount &&
           currentEquipCount == MAXWEAPONCOUNT) ||
          (currentItemIndex < currentEquipCount + 1 &&
           currentEquipCount ==
               1)) { //在下面的框里并且武器区已满,或者只剩了一个武器
        currentItemSelectedCount = 0;
      } else
        currentItemSelectedCount = 1;
      focus = StatusFocusOnItemSelected;
      break;
    case StatusShield: //因为两个数量都是1所以合并了
    case StatusArticle:
      if (currentItemIndex > currentEquipCount &&
          currentEquipCount == MAXSHIELDCOUNT) { //在下面的框里并且武器区已满
        currentItemSelectedCount = 0;
      } else
        currentItemSelectedCount =
            1; //显示装备(在备选区中)或者卸下(在已经装备的武器区中)
      focus = StatusFocusOnItemSelected;

      break;
    case StatusMedicine:
      focus = StatusFocusOnItemSelected;
      if (currentItemIndex <= currentEquipCount) { //在上面的框里
        if ((*pArticleIDToInfoMap)[currentSelectedItemID].special > 3000)
        //这个3000是magic number,因为最小的治疗药物是3000
        { //是回复类药物,可以使用或者卸下
          currentItemSelectedCount = 2;
        } else
          currentItemSelectedCount = 1;
      } else {
        if (currentEquipCount == MAXMEDICINECOUNT) { //装备区满了
          if ((*pArticleIDToInfoMap)[currentSelectedItemID].special > 3000)
            currentItemSelectedCount = 1; //可以使用不可以装备
          else
            currentItemSelectedCount = 0; //不可以使用和装备
        } else {
          if ((*pArticleIDToInfoMap)[currentSelectedItemID].special > 3000)
            currentItemSelectedCount = 2; //可以使用和装备
          else
            currentItemSelectedCount = 1; //可以装备,不可以使用
        }
      }
      // qDebug()<<currentItemSelectedCount;
      //            if((currentItemIndex>currentEquipCount&&currentEquipCount==MAXMEDICINECOUNT)
      //                    ||(*pArticleIDToInfoMap)[currentSelectedItemID].special==0)
      //            {
      //                currentItemSelectedCount=1;//装备区满了就不允许再装备,但是可以直接使用
      //            }
      //            else
      //                currentItemSelectedCount=2;//显示装备/使用(在备选区中)或者卸下/使用(在已经装备的武器区中)
      break;
    default:
      assert(0);
    }
    currentItemSelectedIndex = 0;
    //之所以没有用if,是考虑到未来可能扩展这里
    break;
  case StatusFocusOnItemSelected:
    //执行装备/卸下/使用
    focus = StatusFocusOnEquipment;
    switch (currentTabIndex) {
    case StatusWeapon:
      assert(currentEquipCount > 0);
      if (currentItemSelectedCount == 0)
        break; //灰色显示的卸下或者装备
      if (currentItemIndex <
          currentEquipCount + 1) { //在上面的框里,执行卸下操作
        currentEquipCount--;
        for (int i = 0; i < MAXWEAPONCOUNT; ++i) {
          if (roleInfo[currentRoleID - 1]->weapon[i] ==
              currentSelectedItemID) { //定位到要卸下的物品处
            memmove(&roleInfo[currentRoleID - 1]->weapon[i],
                    &roleInfo[currentRoleID - 1]->weapon[i + 1],
                    sizeof(int) * (MAXWEAPONCOUNT - i - 1));
            // i=0移动3个字节,i=1移动2个字节
            roleInfo[currentRoleID - 1]->weapon[MAXWEAPONCOUNT - 1] = 0;
            break;
          }
        }
        pCarriedArticleSet->insert(currentSelectedItemID);
        if (currentItemIndex == currentEquipCount + 1) //因为刚刚已经减了1
        {
          currentItemIndex = 1;
        }

        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            1) { //证明刚刚卸下的东西原来不存在于装备区,因此需要刷新可用装备区,否则不需要做任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
        }
        currentSelectedItemID =
            roleInfo[currentRoleID - 1]->weapon[currentItemIndex - 1];
      } else { //执行装备操作,合法性由其他函数保证,即,如果装备区已满,那么此代码不应得到执行
        currentEquipCount++;
        for (int i = 0; i < MAXWEAPONCOUNT; ++i) {
          if (roleInfo[currentRoleID - 1]->weapon[i] ==
              0) { //定位到第一个为0的位置
            roleInfo[currentRoleID - 1]->weapon[i] = currentSelectedItemID;
            break;
          }
        }
        multiset<int>::iterator iter =
            pCarriedArticleSet->find(currentSelectedItemID);
        pCarriedArticleSet->erase(iter);
        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            0) { //刚刚装备的物品是可用装备区同等物品中的最后一件,因此需要刷新可用装备区,否则不需要任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
          if (!availableArticleSet.empty()) {
            currentItemIndex = currentEquipCount + 1;
            currentSelectedItemID = *availableArticleSet.begin();
          } else {
            focus = StatusFocusOnTabIndex;
            currentSelectedItemID = 0;
            currentItemIndex = 0;
          }
        }
      }
      break;
    case StatusShield:
      if (currentItemSelectedCount == 0)
        break; //灰色显示的卸下或者装备
      if (currentItemIndex <
          currentEquipCount + 1) { //在上面的框里,执行卸下操作
        currentEquipCount--;
        assert(roleInfo[currentRoleID - 1]->shield == currentSelectedItemID);
        roleInfo[currentRoleID - 1]->shield = 0;
        roleInfo[currentRoleID - 1]->physicalDefence -=
            (*pArticleIDToInfoMap)[currentSelectedItemID].attack;
        //因为卸下了防具,所以防御降低
        //防具的特殊效果在这里进行
        pCarriedArticleSet->insert(currentSelectedItemID);
        currentItemIndex = 0;

        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            1) { //证明刚刚卸下的东西原来不存在于装备区,因此需要刷新可用装备区,否则不需要做任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
        }
        focus = StatusFocusOnTabIndex;
        currentSelectedItemID = 0;
      } else { //执行装备操作,合法性由其他函数保证,即,如果装备区已满,那么此代码不应得到执行
        currentEquipCount++;
        roleInfo[currentRoleID - 1]->shield = currentSelectedItemID;
        roleInfo[currentRoleID - 1]->physicalDefence +=
            (*pArticleIDToInfoMap)[currentSelectedItemID].attack;
        //装备了防具,所以防御上升
        multiset<int>::iterator iter =
            pCarriedArticleSet->find(currentSelectedItemID);
        pCarriedArticleSet->erase(iter);
        //找到并删除之
        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            0) { //刚刚装备的物品是可用装备区的最后一件,因此需要刷新可用装备区,否则不需要任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
          if (!availableArticleSet.empty()) {
            currentSelectedItemID = *availableArticleSet.begin();
            currentItemIndex = currentEquipCount + 1;
          } else {
            focus = StatusFocusOnTabIndex;
            currentItemIndex = 0;
          }
        }
      }
      break;
    case StatusArticle:
      if (currentItemSelectedCount == 0)
        break; //灰色显示的卸下或者装备
      if (currentItemIndex <
          currentEquipCount + 1) { //在上面的框里,执行卸下操作
        currentEquipCount--;
        assert(roleInfo[currentRoleID - 1]->article == currentSelectedItemID);
        roleInfo[currentRoleID - 1]->article = 0;
        //这里执行物品的特殊效果,例如加经验等
        int special = (*pArticleIDToInfoMap)[currentSelectedItemID].special;
        if (special > ARTICLEDODGEUP_1 - 1 &&
            special < ARTICLEDODGEUP_10 + 1) { //将增加的躲闪减掉
          roleInfo[currentRoleID - 1]->dodge /=
              (1.0 + (special - (ARTICLEDODGEUP_1 - 1)) / 10);
        }
        if (special > ARTICLEIQUP_1 - 1 &&
            special < ARTICLEIQUP_10 + 1) { //将增加的iq减掉
          roleInfo[currentRoleID - 1]->IQ /=
              (1.0 + (special - (ARTICLEIQUP_1 - 1)) / 10);
        }
        if (special > ARTICLESPEEDUP_1 - 1 &&
            special < ARTICLESPEEDUP_10 + 1) { //将增加的移动减掉
          roleInfo[currentRoleID - 1]->speed -=
              (special - (ARTICLESPEEDUP_1 - 1));
        }

        pCarriedArticleSet->insert(currentSelectedItemID);
        currentItemIndex = 0;
        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            1) { //证明刚刚卸下的东西原来不存在于装备区,因此需要刷新可用装备区,否则不需要做任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
        }
        focus = StatusFocusOnTabIndex;
        currentSelectedItemID = 0;
      } else { //执行装备操作,合法性由其他函数保证,即,如果装备区已满,那么此代码不应得到执行
        currentEquipCount++;
        roleInfo[currentRoleID - 1]->article = currentSelectedItemID;
        //这里执行物品的特殊效果
        int special = (*pArticleIDToInfoMap)[currentSelectedItemID].special;
        if (special > ARTICLEDODGEUP_1 - 1 &&
            special < ARTICLEDODGEUP_10 + 1) { //加躲闪
          roleInfo[currentRoleID - 1]->dodge *=
              (1.0 + (special - (ARTICLEDODGEUP_1 - 1)) / 10);
        }
        if (special > ARTICLEIQUP_1 - 1 &&
            special < ARTICLEIQUP_10 + 1) { //加iq
          roleInfo[currentRoleID - 1]->IQ *=
              (1.0 + (special - (ARTICLEIQUP_1 - 1)) / 10);
        }
        if (special > ARTICLESPEEDUP_1 - 1 &&
            special < ARTICLESPEEDUP_10 + 1) { //加移动
          roleInfo[currentRoleID - 1]->speed +=
              (special - (ARTICLESPEEDUP_1 - 1));
        }

        multiset<int>::iterator iter =
            pCarriedArticleSet->find(currentSelectedItemID);
        pCarriedArticleSet->erase(iter);
        //找到并删除之
        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            0) { //刚刚装备的物品是可用装备区的最后一件,因此需要刷新可用装备区,否则不需要任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
          if (!availableArticleSet.empty()) {
            currentItemIndex = currentEquipCount + 1;
            currentSelectedItemID = *availableArticleSet.begin();
          } else {
            focus = StatusFocusOnTabIndex;
            currentSelectedItemID = 0;
            currentItemIndex = 0;
          }
        }
      }
      break;
    case StatusMedicine:
      if (currentItemIndex <
          currentEquipCount +
              1) { //卸下或者使用某种药品,当前只允许使用回复HP类药品
        currentEquipCount--;
        //使用药品的合法性由其他函数保证,即:如果使用该药品,证明"使用"按钮是可用的
        if (currentItemSelectedIndex == 1) { //使用的话加HP
          int special = (*pArticleIDToInfoMap)[currentSelectedItemID].special -
                        MEDICINEORIGIN;
          assert(special > 0 && special < 1000);
          roleInfo[currentRoleID - 1]->HP += special;
          if ((roleInfo[currentRoleID - 1]->HP) >
              (roleInfo[currentRoleID - 1]->HPMax))
            roleInfo[currentRoleID - 1]->HP =
                roleInfo[currentRoleID - 1]->HPMax;
        } else {
          pCarriedArticleSet->insert(currentSelectedItemID);
        }
        //不论是卸下还是使用,它都要被从装备里面抹掉
        // qDebug()<<currentSelectedItemID;
        for (int i = 0; i < MAXWEAPONCOUNT; ++i) {
          if (roleInfo[currentRoleID - 1]->medicine[i] ==
              currentSelectedItemID) { //定位到要卸下的物品处
            memmove(&roleInfo[currentRoleID - 1]->medicine[i],
                    &roleInfo[currentRoleID - 1]->medicine[i + 1],
                    sizeof(int) * (MAXMEDICINECOUNT - i - 1));
            // i=0移动3个字节,i=1移动2个字节
            roleInfo[currentRoleID - 1]->medicine[MAXMEDICINECOUNT - 1] = 0;
            break;
          }
        }

        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            1) { //证明刚刚卸下的东西原来不存在于装备区,因此需要刷新可用装备区,否则不需要做任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
        }

        if (currentEquipCount == 0) {
          focus = StatusFocusOnTabIndex;
          currentItemIndex = 0;
          currentSelectedItemID = 0;
        } else if (currentItemIndex ==
                   currentEquipCount +
                       1) { //如果当前选中的是已经装备区的最后一条
          currentItemIndex = 1;
          currentSelectedItemID = roleInfo[currentRoleID - 1]->medicine[0];
        } else
          currentSelectedItemID =
              roleInfo[currentRoleID - 1]->medicine[currentItemIndex - 1];

      } else { //装备或者使用某种药品
        if (currentItemSelectedCount == 0) {
          break;
        } else if ((currentItemSelectedCount == 1 &&
                    currentEquipCount == MAXMEDICINECOUNT) ||
                   (currentItemSelectedCount == 2 &&
                    currentItemSelectedIndex != 0)) { //使用的话加HP
          int special = (*pArticleIDToInfoMap)[currentSelectedItemID].special -
                        MEDICINEORIGIN;
          assert(special > 0 && special < 1000);
          roleInfo[currentRoleID - 1]->HP += special;
          if ((roleInfo[currentRoleID - 1]->HP) >
              (roleInfo[currentRoleID - 1]->HPMax))
            roleInfo[currentRoleID - 1]->HP =
                roleInfo[currentRoleID - 1]->HPMax;
        } else {
          currentEquipCount++;
          assert(roleInfo[currentRoleID - 1]->medicine[MAXMEDICINECOUNT - 1] ==
                 0);
          roleInfo[currentRoleID - 1]->medicine[currentEquipCount - 1] =
              currentSelectedItemID;
        }
        //不论是装备还是使用,它都要被从携带物品里面抹掉
        multiset<int>::iterator iter =
            pCarriedArticleSet->find(currentSelectedItemID);
        pCarriedArticleSet->erase(iter);

        if (pCarriedArticleSet->count(currentSelectedItemID) ==
            0) { //刚刚装备的物品是可用装备区的最后一件,因此需要刷新可用装备区,否则不需要任何改变
          this->resetAvailableArticle();
          availableArticleBeginIter = availableArticleSet.begin();
          if (!availableArticleSet.empty()) {
            currentItemIndex = currentEquipCount + 1;
            currentSelectedItemID = *availableArticleSet.begin();
          } else {
            focus = StatusFocusOnTabIndex;
            currentSelectedItemID = 0;
            currentItemIndex = 0;
          }
        }
      }
      break;
    default:
      qDebug() << currentTabIndex;
      assert(0);
    }
    // resetAvailableArticle();
    break;
  default:
    qDebug() << focus;
    assert(0);
  }
  this->update();
}
void StatusWidget::onCancelKey() {
  switch (focus) {
  case StatusFocusOnTabIndex:
    this->close();
    this->parentWidget()->setFocus();
    break;
  case StatusFocusOnEquipment:
    focus = StatusFocusOnTabIndex;
    currentSelectedItemID = 0;
    currentItemIndex = 0;
    break;
  case StatusFocusOnItemSelected:
    focus = StatusFocusOnEquipment;
    break;
  default:
    qDebug() << focus;
    assert(0);
  }
  this->update();
}
void StatusWidget::onDirectionKey(DIRECTION dir) {
  switch (dir) {
  case UP:
    switch (focus) {
    case StatusFocusOnTabIndex:
      if (currentTabIndex ==
          StatusRoleInfo) { //光标在人物ID栏,按上箭头向前切换角色信息
        do {
          if (currentRoleID == 1) {
            currentRoleID = ROLENUMBER;
          } else
            currentRoleID--;
        } while (roleInfo[currentRoleID - 1] == NULL);

        this->resetAvailableArticle();
        // qDebug()<<currentRoleID;
      } else {
        if (currentEquipCount + currentAvailableCount == 0)
          break;
        focus = StatusFocusOnEquipment;
        if (currentAvailableCount > AVAILABLEARTICLECOUNT) {
          currentItemIndex = currentEquipCount + 4;
          int i = AVAILABLEARTICLECOUNT;
          availableArticleBeginIter = availableArticleSet.end();
          while (i--)
            availableArticleBeginIter--;
        } else {
          availableArticleBeginIter = availableArticleSet.begin();
          currentItemIndex = currentEquipCount + currentAvailableCount;
        }
        //下面的index最大为equipCount+4;
      }

      break;
    case StatusFocusOnEquipment:
      if (currentItemIndex < currentEquipCount + 1) //在上面的框里面
      {
        currentItemIndex--;
        if (currentItemIndex == 0) {
          focus = StatusFocusOnTabIndex;
        }
      } else {
        if (currentItemIndex == currentEquipCount + 1) //下面的框最上面一条
        {
          if (availableArticleBeginIter == availableArticleSet.begin()) {
            currentItemIndex = currentEquipCount;
            if (currentItemIndex == 0) {
              focus = StatusFocusOnTabIndex;
            }
          } else {
            availableArticleBeginIter--;
          }
        } else {
          currentItemIndex--;
        }
      }
      break;
    case StatusFocusOnItemSelected:
      if (currentItemSelectedCount == 0 || currentItemSelectedIndex == 0) {
        currentItemSelectedIndex = currentItemSelectedCount - 1;
      } else
        currentItemSelectedIndex--;
      break;
    default:
      qDebug() << focus;
      assert(0);
    }
    break;
  case DOWN:
    switch (focus) {
    case StatusFocusOnTabIndex:
      if (currentTabIndex ==
          StatusRoleInfo) { //光标在人物ID栏,按下箭头向前切换角色信息
        do {
          if (currentRoleID == ROLENUMBER) {
            currentRoleID = 1;
          } else
            currentRoleID++;
        } while (roleInfo[currentRoleID - 1] == NULL);
        this->resetAvailableArticle();
      } else {
        if (currentEquipCount + currentAvailableCount == 0)
          break;
        focus = StatusFocusOnEquipment;
        availableArticleBeginIter = availableArticleSet.begin();
        currentItemIndex = 1;
      }
      break;
    case StatusFocusOnEquipment:
      if (currentItemIndex < currentEquipCount + 1) //在上面的框里面
      {
        if (currentItemIndex == currentEquipCount) {
          if (currentAvailableCount == 0) { //下面的框里面没有东西
            currentItemIndex = 0;
            focus = StatusFocusOnTabIndex;
          } else {
            currentItemIndex = currentEquipCount + 1;
            availableArticleBeginIter = availableArticleSet.begin();
          }
        } else
          currentItemIndex++;
      } else {
        //在下面的框里面
        if (currentAvailableCount > AVAILABLEARTICLECOUNT) {
          if (currentItemIndex ==
              currentEquipCount +
                  AVAILABLEARTICLECOUNT) //到达了可用栏的最后一条
          {
            int i = AVAILABLEARTICLECOUNT;
            set<int>::iterator iter = availableArticleBeginIter;
            while (i--) {
              iter++;
            }

            if (iter == availableArticleSet.end()) {
              //当前显示的最后一条已经是set的最后一条
              focus = StatusFocusOnTabIndex;
              currentItemIndex = 0;
            } else {
              availableArticleBeginIter++;
            }
          } else {
            currentItemIndex++;
          }
        } else {
          if (currentItemIndex == currentEquipCount + currentAvailableCount) {
            //到达了可用栏的最后一条
            focus = StatusFocusOnTabIndex;
            currentItemIndex = 0;
          } else
            currentItemIndex++;
        }
      }
      break;
    case StatusFocusOnItemSelected:
      if (currentItemSelectedCount == 0 ||
          currentItemSelectedIndex == currentItemSelectedCount - 1) {
        currentItemSelectedIndex = 0;
      } else
        currentItemSelectedIndex++;
      break;
    default:
      qDebug() << focus;
      assert(0);
    }
    break;
  case LEFT:
    switch (focus) {
    case StatusFocusOnTabIndex:
      if (currentTabIndex ==
          StatusRoleInfo) { //光标在人物ID栏,按左箭头切换到最右index
        currentTabIndex = StatusArticle;
      } else {
        currentTabIndex =
            static_cast<StatusTabIndex>(static_cast<int>(currentTabIndex) - 1);
      }
      currentItemIndex = 0;
      currentItemSelectedIndex = 0;
      if (currentTabIndex != StatusRoleInfo)
        resetAvailableArticle();
      break;
    case StatusFocusOnEquipment:
      break;
    case StatusFocusOnItemSelected:
      focus = StatusFocusOnEquipment;
      break;
    default:
      qDebug() << focus;
      assert(0);
    }
    break;
  case RIGHT:
    switch (focus) {
    case StatusFocusOnTabIndex:
      if (currentTabIndex ==
          StatusArticle) { //光标在人物ID栏,按左箭头切换到最右index
        currentTabIndex = StatusRoleInfo;
      } else {
        currentTabIndex =
            static_cast<StatusTabIndex>(static_cast<int>(currentTabIndex) + 1);
      }
      currentItemIndex = 0;
      currentItemSelectedIndex = 0;
      // if(currentTabIndex!=StatusRoleInfo)
      resetAvailableArticle();
      break;
    case StatusFocusOnEquipment:
      switch (currentTabIndex) {
      case StatusWeapon:
        if ((currentItemIndex > currentEquipCount &&
             currentEquipCount == MAXWEAPONCOUNT) ||
            (currentItemIndex < currentEquipCount + 1 &&
             currentEquipCount ==
                 1)) { //在下面的框里并且武器区已满,或者只剩了一个武器
          currentItemSelectedCount = 0;
        } else
          currentItemSelectedCount = 1;
        focus = StatusFocusOnItemSelected;
        break;
      case StatusShield: //因为两个数量都是1所以合并了
      case StatusArticle:
        if (currentItemIndex > currentEquipCount &&
            currentEquipCount == MAXSHIELDCOUNT) { //在下面的框里并且武器区已满
          currentItemSelectedCount = 0;
        } else
          currentItemSelectedCount =
              1; //显示装备(在备选区中)或者卸下(在已经装备的武器区中)
        focus = StatusFocusOnItemSelected;

        break;
      case StatusMedicine:
        focus = StatusFocusOnItemSelected;
        if (currentItemIndex <= currentEquipCount) { //在上面的框里
          if ((*pArticleIDToInfoMap)[currentSelectedItemID].special > 3000)
          //这个3000是magic number,因为最小的治疗药物是3000
          { //是回复类药物,可以使用或者卸下
            currentItemSelectedCount = 2;
          } else
            currentItemSelectedCount = 1;
        } else {
          if (currentEquipCount == MAXMEDICINECOUNT) { //装备区满了
            if ((*pArticleIDToInfoMap)[currentSelectedItemID].special > 3000)
              currentItemSelectedCount = 1; //可以使用不可以装备
            else
              currentItemSelectedCount = 0; //不可以使用和装备
          } else {
            if ((*pArticleIDToInfoMap)[currentSelectedItemID].special > 3000)
              currentItemSelectedCount = 2; //可以使用和装备
            else
              currentItemSelectedCount = 1; //可以装备,不可以使用
          }
        }
        break;
      default:
        assert(0);
      }
      currentItemSelectedIndex = 0;
      break;
    case StatusFocusOnItemSelected:
      break;
    default:
      qDebug() << focus;
      assert(0);
    }
    break;
  default:
    qDebug() << dir;
    assert(0);
  }

  infoWidget->changeToRoleOf(currentRoleID);
  this->updateSelectedItemID();
  this->update();
}
void StatusWidget::resetAvailableArticle() {
  currentEquipCount = 0;
  switch (currentTabIndex) {
  case StatusRoleInfo:
  case StatusWeapon:
    availableArticleSet.clear();
    for (int i = 0; i < MAXWEAPONCOUNT; ++i) {
      if (roleInfo[currentRoleID - 1]->weapon[i]) {
        if (pArticleIDToInfoMap->find(roleInfo[currentRoleID - 1]->weapon[i]) ==
            pArticleIDToInfoMap->end()) { //没找到
          DBIO dbio;
          if (!dbio.getArticleInfoFromLib(
                  roleInfo[currentRoleID - 1]->weapon[i], pArticleIDToInfoMap))
            quitApp(ERRORGETARTICLEINFOFAIL);
        }
        currentEquipCount++;
      }
    }
    for (multiset<int>::iterator iter = pCarriedArticleSet->begin();
         iter != pCarriedArticleSet->end(); ++iter) {
      int currentRoleJob =
          (static_cast<int>(roleInfo[currentRoleID - 1]->job)) / 3;
      if ((*iter) / 100 == currentRoleJob) {
        if (pArticleIDToInfoMap->find(*iter) ==
            pArticleIDToInfoMap->end()) { //没找到
          DBIO dbio;
          if (!dbio.getArticleInfoFromLib(*iter, pArticleIDToInfoMap))
            quitApp(ERRORGETARTICLEINFOFAIL);
        }
        availableArticleSet.insert(*iter);
        // allAvailableArticleCount++;
        //可用武器ID/100在数值上等于职业代码
      }
    }
    currentAvailableCount = availableArticleSet.size();
    availableArticleBeginIter = availableArticleSet.begin();
    break;
  case StatusShield:
    availableArticleSet.clear();
    if (roleInfo[currentRoleID - 1]->shield) {
      if (pArticleIDToInfoMap->find(roleInfo[currentRoleID - 1]->shield) ==
          pArticleIDToInfoMap->end()) { //没找到
        DBIO dbio;
        if (!dbio.getArticleInfoFromLib(roleInfo[currentRoleID - 1]->shield,
                                        pArticleIDToInfoMap))
          quitApp(ERRORGETARTICLEINFOFAIL);
      }
      currentEquipCount++;
    }

    for (multiset<int>::iterator iter = pCarriedArticleSet->begin();
         iter != pCarriedArticleSet->end(); ++iter) {
      if (pArticleIDToInfoMap->find(*iter) ==
          pArticleIDToInfoMap->end()) { //没找到
        DBIO dbio;
        if (!dbio.getArticleInfoFromLib(*iter, pArticleIDToInfoMap))
          quitApp(ERRORGETARTICLEINFOFAIL);
      }
      if (*iter > SHIELDDOWNBOUND && *iter < SHIELDUPBOUND)
        availableArticleSet.insert(*iter);
    }
    currentAvailableCount = availableArticleSet.size();
    availableArticleBeginIter = availableArticleSet.begin();
    break;
  case StatusMedicine:
    availableArticleSet.clear();
    for (int i = 0; i < MAXMEDICINECOUNT; ++i) {
      if (roleInfo[currentRoleID - 1]->medicine[i]) {
        if (pArticleIDToInfoMap->find(
                roleInfo[currentRoleID - 1]->medicine[i]) ==
            pArticleIDToInfoMap->end()) { //没找到
          DBIO dbio;
          if (!dbio.getArticleInfoFromLib(
                  roleInfo[currentRoleID - 1]->medicine[i],
                  pArticleIDToInfoMap))
            quitApp(ERRORGETARTICLEINFOFAIL);
        }
        currentEquipCount++;
      }
    }
    for (multiset<int>::iterator iter = pCarriedArticleSet->begin();
         iter != pCarriedArticleSet->end(); ++iter) {
      if (pArticleIDToInfoMap->find(*iter) ==
          pArticleIDToInfoMap->end()) { //没找到
        DBIO dbio;
        if (!dbio.getArticleInfoFromLib(*iter, pArticleIDToInfoMap))
          quitApp(ERRORGETARTICLEINFOFAIL);
      }
      if (*iter > MEDICINEDOWNBOUND && *iter < MEDICINEUPBOUND)
        availableArticleSet.insert(*iter);
    }
    currentAvailableCount = availableArticleSet.size();
    availableArticleBeginIter = availableArticleSet.begin();
    break;
  case StatusArticle:
    availableArticleSet.clear();
    if (roleInfo[currentRoleID - 1]->article) {
      if (pArticleIDToInfoMap->find(roleInfo[currentRoleID - 1]->article) ==
          pArticleIDToInfoMap->end()) {
        DBIO dbio;
        if (!dbio.getArticleInfoFromLib(roleInfo[currentRoleID - 1]->article,
                                        pArticleIDToInfoMap))
          quitApp(ERRORGETARTICLEINFOFAIL);
      }
      currentEquipCount++;
    }

    for (multiset<int>::iterator iter = pCarriedArticleSet->begin();
         iter != pCarriedArticleSet->end(); ++iter) {
      if (pArticleIDToInfoMap->find(*iter) ==
          pArticleIDToInfoMap->end()) { //没找到
        DBIO dbio;
        if (!dbio.getArticleInfoFromLib(*iter, pArticleIDToInfoMap))
          quitApp(ERRORGETARTICLEINFOFAIL);
      }
      if (*iter > ARTICLEDOWNBOUND && *iter < ARTICLEUPBOUND)
        availableArticleSet.insert(*iter);
    }
    currentAvailableCount = availableArticleSet.size();
    availableArticleBeginIter = availableArticleSet.begin();
    break;
  default:
    qDebug() << currentTabIndex;
    assert(0);
  }
  //    for(set<int>::iterator
  //    iter=availableArticleSet.begin();iter!=availableArticleSet.end();++iter)
  //    {
  //        qDebug()<<(*iter);
  //    }
  this->update();
}

void StatusWidget::updateSelectedItemID() {
  if (focus == StatusFocusOnTabIndex)
    currentSelectedItemID = 0;
  else {
    assert(currentItemIndex != 0);
    switch (currentTabIndex) {
    case StatusRoleInfo:
    case StatusWeapon:
      if (currentItemIndex < currentEquipCount + 1) {
        assert(currentItemIndex < 5);
        currentSelectedItemID =
            roleInfo[currentRoleID - 1]->weapon[currentItemIndex - 1];
      } else {
        set<int>::iterator iter = availableArticleBeginIter;
        for (int i = currentEquipCount; i < currentItemIndex - 1; ++i) {
          ++iter;
        }
        currentSelectedItemID = *iter;
      }
      break;
    case StatusShield:
      if (currentItemIndex < currentEquipCount + 1) {
        assert(currentItemIndex == 1);
        currentSelectedItemID = roleInfo[currentRoleID - 1]->shield;
      } else {
        set<int>::iterator iter = availableArticleBeginIter;
        for (int i = currentEquipCount; i < currentItemIndex - 1; ++i) {
          ++iter;
        }
        currentSelectedItemID = *iter;
      }
      break;
    case StatusMedicine:
      if (currentItemIndex < currentEquipCount + 1) {
        assert(currentItemIndex < 5);
        currentSelectedItemID =
            roleInfo[currentRoleID - 1]->medicine[currentItemIndex - 1];
      } else {
        set<int>::iterator iter = availableArticleBeginIter;
        for (int i = currentEquipCount; i < currentItemIndex - 1; ++i) {
          ++iter;
        }
        currentSelectedItemID = *iter;
      }
      break;
    case StatusArticle:
      if (currentItemIndex < currentEquipCount + 1) {
        assert(currentItemIndex == 1);
        currentSelectedItemID = roleInfo[currentRoleID - 1]->article;
      } else {
        set<int>::iterator iter = availableArticleBeginIter;
        for (int i = currentEquipCount; i < currentItemIndex - 1; ++i) {
          ++iter;
        }
        currentSelectedItemID = *iter;
      }
      break;
    default:
      qDebug() << currentTabIndex;
      assert(0);
    }
  }
}
