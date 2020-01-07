#include "lgl.h"

const int range_1[4][2] = { //十字格
    0, -1, 0, 1, -1, 0, 1, 0};
const int range_2[8][2] = { // 8方格
    0, -1, 0, 1, -1, 0, 1, 0, 1, 1, 1, -1, -1, 1, -1, -1};
const int range_3[8][2] = { //射程2的十字格
    0, -2, 0, -1, 0, 1, 0, 2, -1, 0, -2, 0, 1, 0, 2, 0};
const int range_4[12][2] = { // manhattan距离2
    0, -2, 0, -1, 0, 1, 0, 2,  -1, 0, -2, 0,
    1, 0,  2, 0,  1, 1, 1, -1, -1, 1, -1, -1};
const int range_5[24][2] = {0,  -3, 0,  -2, 0,  -1,    0,  1,  0,  2,  0, 3,
                            -1, 0,  -2, 0,  -3, 0,     1,  0,  2,  0,  3, 0,
                            1,  1,  1,  -1, -1, 1 - 1, -1, -1, -2, -1, 2, 1,
                            -2, 1,  1,  2,  1,  -2,    1,  2,  -1, -2, -1};
const int range_6[28][2] = {
    0, -3, 0, -2, 0, -1, 0,  1,  0,  2,  0,     3,  -1, 0,  -2, 0,  -3, 0,  1,
    0, 2,  0, 3,  0, 1,  1,  1,  -1, -1, 1 - 1, -1, -1, -2, -1, 2,  1,  -2, 1,
    1, 2,  1, -2, 1, 2,  -1, -2, -1, 2,  2,     2,  -2, -2, 2,  -2, -2};
const int range_7[36][2] = {
    0,  -3, 0, -2, 0, -1, 0,  1, 0,  2,  0,  3,  -1, 0,  -2, 0,  -3, 0,
    1,  0,  2, 0,  3, 0,  1,  1, 1,  -1, -1, 1,  -1, -1, -1, -2, -1, 2,
    1,  -2, 1, 1,  2, 1,  -2, 1, 2,  -1, -2, -1, 1,  3,  1,  -3, -1, -3,
    -1, 3,  2, 2,  2, -2, -2, 2, -2, -2, 3,  1,  3,  -1, -3, 1,  -3, -1};
const int range_8[12][2] = {0, -3, 0, -2, 0, 2, 0, 3,  -2, 0, -3, 0,
                            2, 0,  3, 0,  1, 1, 1, -1, -1, 1, -1, -1};
const int range_9[16][2] = {0, -4, 0, -3, 0, -2, 0,  2, 0,  3, 0,
                            4, -2, 0, -3, 0, -4, 0,  2, 0,  3, 0,
                            4, 0,  1, 1,  1, -1, -1, 1, -1, -1};

void getRangeVectorFromRange(int range, vector<QPoint> &rangeVec, int x, int y,
                             int mapSizeX, int mapSizeY) {
  assert(rangeVec.empty());
  int *p = NULL;
  // int (*p)[2]=NULL;
  int pointCount = 0;
  switch (range) {
  case 1:
    p = (int *)range_1;
    pointCount = 4;
    break;
  case 2:
    p = (int *)range_2;
    pointCount = 8;
    break;
  case 3:
    p = (int *)range_3;
    pointCount = 8;
    break;
  case 4:
    p = (int *)range_4;
    pointCount = 12;
    break;
  case 5:
    p = (int *)range_5;
    pointCount = 24;
    break;
  case 6:
    p = (int *)range_6;
    pointCount = 28;
    break;
  case 7:
    p = (int *)range_7;
    pointCount = 36;
    break;
  case 8:
    p = (int *)range_8;
    pointCount = 12;
    break;
  case 9:
    p = (int *)range_9;
    pointCount = 16;
    break;
  default:
    qDebug() << range;
    assert(0);
  }
  //    for(int i=0;i<2*pointCount;++i)
  //    {
  //        qDebug()<<p[i];
  //    }
  while (pointCount--) {
    //        qDebug()<<pointCount<<":";
    //        qDebug()<<p[pointCount*2+0]<<" "<<p[pointCount*2+1];
    //        qDebug()<<x+p[pointCount*2+0]<<" "<<y+p[pointCount*2+1];
    if (x + p[pointCount * 2 + 0] < 0 || y + p[pointCount * 2 + 1] < 0 ||
        x + p[pointCount * 2 + 0] > mapSizeX - 1 ||
        y + p[pointCount * 2 + 1] > mapSizeY - 1)
      continue;
    rangeVec.push_back(
        QPoint(x + p[pointCount * 2 + 0], y + p[pointCount * 2 + 1]));
  }
}
bool checkIfInRange(QPoint const &attacker, QPoint const &defencer, int range) {

  // qDebug()<<attacker.x()<<" "<<attacker.y()<<" target:"<<defencer.x()<<"
  // "<<defencer.y();
  assert(attacker != defencer);
  //    if(attacker==defencer)
  //        return true;
  int *p = NULL;
  // int (*p)[2]=NULL;
  int pointCount = 0;
  switch (range) {
  case 1:
    p = (int *)range_1;
    pointCount = 4;
    break;
  case 2:
    p = (int *)range_2;
    pointCount = 8;
    break;
  case 3:
    p = (int *)range_3;
    pointCount = 8;
    break;
  case 4:
    p = (int *)range_4;
    pointCount = 12;
    break;
  case 5:
    p = (int *)range_5;
    pointCount = 24;
    break;
  case 6:
    p = (int *)range_6;
    pointCount = 28;
    break;
  case 7:
    p = (int *)range_7;
    pointCount = 36;
    break;
  case 8:
    p = (int *)range_8;
    pointCount = 12;
    break;
  case 9:
    p = (int *)range_9;
    pointCount = 16;
    break;
  default:
    qDebug() << range;
    assert(0);
  }
  while (pointCount--) {
    if (attacker + QPoint(p[pointCount * 2 + 0], p[pointCount * 2 + 1]) ==
        defencer) {
      return true; //防御方在攻击方射程内
    }
  }
  return false;
}

QRect getRectFromPose(ROLEPOSE pose) {
  int index = (int)pose;
  return QRect(index % 3 * BASEWIDTH, index / 3 * BASEHEIGHT, BASEWIDTH,
               BASEHEIGHT);
}
QRect getRectFromMapIndex(int xIndex, int yIndex) {
  return QRect(xIndex * BASEWIDTH, yIndex * BASEHEIGHT, WINDOWWIDTH,
               WINDOWHEIGHT);
}

QString getSubFromQString(QString const &str, int n, char ch) {
  // qDebug()<<str;
  assert(n > 0);
  int length = str.length();
  int previousPos = 0; //上一个分隔符的位置
  for (int i = 0; i < length; ++i) {
    if (str[i] != ch)
      continue;
    else {
      if (n == 1) {
        // QString *sub=new QString(str.mid(previousPos,i-previousPos));
        return str.mid(previousPos, i - previousPos);
      }
      previousPos = i + 1;
      --n;
    }
  }
  qDebug() << str;
  assert(0);
}
void swap(int &x, int &y) {
  // qDebug()<<"swap:"<<x<<" "<<y;
  if (x == y)
    return;
  x ^= y;
  y ^= x;
  x ^= y;
}
QString &cutHeadFromQString(QString &str, int n, char ch) {
  int length = str.length();
  for (int i = 0; i < length; ++i) {
    if (str[i] != ch)
      continue;
    else {
      if (n == 1) {
        str.remove(0, i + 1);
        return str;
      }
      --n;
    }
  }
  quitApp(ERRORLIBDATABROKEN);
}
void getPathFromQString(vector<QPoint> &vec, QString &str) {
  while (str.length() != 0) {
    vec.push_back(QPoint(getSubFromQString(str, 1, '#').toInt(),
                         getSubFromQString(str, 2, '#').toInt()));
    cutHeadFromQString(str, 2, '#');
  }
}

void getScriptFromQString(vector<Script> &vec, QString &str) {
  assert(vec.empty());
  Script sc;
  AnimationOperation operation;
  while (str.length() != 0) {
    //一般是param2,格式是这样的 200$205$1#对话内容#$
    // operation和operation之间,operation和param之间用$分割,#分割一个script中的各个参数
    operation =
        static_cast<AnimationOperation>(getSubFromQString(str, 1, '$').toInt());

    switch (operation)
    //从参数中第一个$前取得操作,如果是无参数,则op
    {
    //无参数的一些操作
    case static_cast<int>(AniStart):
    case static_cast<int>(AniEnd):
    case static_cast<int>(AniShowBattleStart):
    case static_cast<int>(AniShowBattleFail):
    case static_cast<int>(AniShowBattleWin):
    case static_cast<int>(AniShowSelfAction):
    case static_cast<int>(AniShowEnemyAction):
    case static_cast<int>(AniShowFriendAction):
    case static_cast<int>(AniFocusRoleBeInactive):
    case static_cast<int>(AniQuitToMainMenu):
    case static_cast<int>(AniMultiTalkFlag):
    case static_cast<int>(AniSetSideStoryOn):
    case static_cast<int>(AniSetSideStoryOK):
    case static_cast<int>(AniSetSideStoryOff):
      sc.op = operation;
      sc.param = QString();
      vec.push_back(sc);
      cutHeadFromQString(str, 1, '$');
      break;
    default:
      sc.op = operation;
      sc.param = getSubFromQString(str, 2, '$');
      vec.push_back(sc);
      cutHeadFromQString(str, 2, '$');
      break;
    }
  }
}

QString getRoleNameFromRoleID(int roleID) {
  // 1 maphero
  // 2~999storymapnpc(包括僵尸)
  // 1001~1020 battlehero
  // 2001~2999 battleenemy
  // 3001~3999 battlefriend
  switch (roleID) {
  case 1:
    return QString("夏小风");
  case 2:
  case 3002:
    return QString("冯小鱼");
  case 3:
    return QString("邵克克");
  case 4:
    return QString("陈小中");
  case 5:
    return QString("蔡大宝");
  case 6:
    return QString("李小莹");
  case 7:
    return QString("武豆浆");
  case 8:
    return QString("烧饼大叔");
  case 9:
    return QString("郭小新");
  case 10:
    return QString("唐小糖");
  case 11:
    return QString("李小宁");
  case 12:
    return QString("闫小龙");
  case 13:
    return QString("保安大叔");
  case 14:
    return QString("二大妈");
  case 15:
    return QString("于阿姨");
  case 16:
    return QString("人造人16号");
  case 17:
    return QString("人造人17号");
  case 18:
    return QString("人造人18号");
  case 19:
    return QString("人造人19号");
  case 20:
    return QString("人造人20号");
  case 101:
    return QString("机械馆大爷");
  case 102:
    return QString("神秘商人");
  case 103:
    return QString("詹小妮");
  case 151:
    return QString("喵星人");
  case 501:
  case 2001:
    return QString("僵尸");
  case 502:
  case 2002:
    return QString("邪恶士兵");
  case 503:
  case 2003:
    return QString("幽灵士兵");
  case 504:
  case 2004:
    return QString("僵尸士兵");
  case 505:
  case 2005:
    return QString("杨士豪");
  case 506:
  case 2006:
    return QString("铁面将军");
  case 507:
  case 2007:
    return QString("幽灵士兵");
  case 508:
  case 2008:
    return QString("僵尸");
  case 512:
  case 2012:
    return QString("死神士兵");
  case 523:
  case 2023:
    //小boss
    return QString("王二狗");
  default:
    qDebug() << roleID;
    assert(0);
  }
}
BattleInfo getBattleInfoFromBattleID(int id) {
  switch (id) {
  case 1:
    return BattleInfo(1, "五舍遭遇战");
  case 2:
    return BattleInfo(3, "五舍之战");
  case 3:
    return BattleInfo(4, "解救系霸之战");
  case 4:
    return BattleInfo(4, "校医院之战");
  case 5:
    return BattleInfo(5, "大黑楼之战");
  case 101:
    return BattleInfo(3, "西山遭遇战");
  case 103:
    return BattleInfo(5, "福佳遭遇战");
  case 105:
    return BattleInfo(6, "研教楼遭遇战");
  case 107:
    return BattleInfo(7, "综合楼遭遇战");
  case 109:
    return BattleInfo(8, "一食堂遭遇战");
  case 111:
    return BattleInfo(9, "主楼遭遇战");
  case 113:
    return BattleInfo(10, "一馆遭遇战");
  case 115:
    return BattleInfo(11, "音乐喷泉遭遇战");
  case 117:
    return BattleInfo(12, "南门酒店遭遇战");
  default:
    qDebug() << id;
    assert(0);
  }
}
void quitApp(int errorCode) {
  QMessageBox::warning(
      NULL, "发生错误",
      QString("程序异常退出，错误代码%1，请重新安装程序或联系开发者。")
          .arg(errorCode),
      QMessageBox::Yes);
  qApp->quit();
}
// QString getMenuStringFromIndex(int index)
//{
//    switch(index)
//    {
//    case 0:return QString("回合结束");
//    case 1:return QString("存档");
//    case 2:return QString("读档");
//    case 3:return QString("撤退");
//    case 4:return QString("战斗目的");
//    default:
//        qDebug()<<index;
//        assert(0);
//        return QString();
//    }
//}
bool getRandomOfPercent(int x) //对于输入的x,返回true或false,true的概率为百分之x
{
  qsrand(time(NULL));
  if (qrand() % 100 < x)
    return true;
  else
    return false;
}

int getRandomNumberIn(int x, int y) //返回[x,y]内的一个随机数
{
  assert(y > x);
  qsrand(time(NULL));
  return ((qrand() % (y - x + 1)) + x);
}

// int getSideStoryIDFromStoryID(int storyID)
//{//由于一个剧情同时只能包括一个支线剧情,因此主线剧情中包含的支线剧情可以枚举出来
//    switch(storyID)
//    {
//    case 5:
//    case 6:
//        return 1;
//    default:
//        return 0;
//    }
//}

// int QStringToInt(const QString &str)
//{
//    return str.toInt();
//}
