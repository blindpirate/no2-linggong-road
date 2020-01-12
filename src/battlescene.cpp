#include "battlescene.h"
#include "astar.cpp"

// battleID是奇数时是进攻局,否则为防御局

const int startPointOffset[12][2] = {0, 0, 1,  0, -1, 0,  0, 1, 0,  -1, 1, -1,
                                     1, 1, -1, 1, -1, -1, 2, 0, -2, 0,  0, -2};

const int rangeArray[4][2] = {1, 0, -1, 0, 0, 1, 0, -1};

#define FOCUSROLEBEINACTIVE                                                    \
  focusRole->changeActiveStatusTo(false);                                      \
  focusRole->setReactOnViewMove(true);                                         \
  swap(mapArray[focusRole->getMapY()][focusRole->getMapX()],                   \
       mapArray[focusRoleMapYCache][focusRoleMapXCache]);                      \
  currentActiveHeroNum--;                                                      \
  if (currentActiveHeroNum == 0) {                                             \
    currentRoundNum++;                                                         \
    processEnemyAction();                                                      \
  }

#define PRINTSCRIPTVEC                                                         \
  for (int i = 0; i < scriptVec.size(); ++i) {                                 \
    qDebug() << scriptVec[i].op;                                               \
    qDebug() << scriptVec[i].param;                                            \
  }

// mapArray中元素低八位指示了元素的类型,次八位为相应vector中的下标
// battlescene可以从两种方式构造，一种是battlescene数据文件，一种是存档文件中的battle数据表

BattleScene::BattleScene(QWidget *parent, int battleID, KeyboardControl *key,
                         vector<int> &selectedRoleID, Role **roleInfo,
                         std::map<int, ArticleInfo> &articleIDToInfoMap)
    : QWidget(parent), mapSizeX(0), mapSizeY(0), mapSizePointX(0),
      mapSizePointY(0), currentPointX(0), currentPointY(0), mapArray(NULL),
      battleID(battleID), viewAutoMoveEndX(0), viewAutoMoveEndY(0),
      cursorOn(false), cursorControlOn(false),
      displayState(BattleNoExtraDisplay), focusRole(NULL), roleInfo(roleInfo),
      currentScriptIndex(0), userControlOn(false), directionKeyPressed(false),
      key(key),
      //    key->upKey(0),key->downKey(0),key->leftKey(0),key->downKey(0),
      //    key->actionKey(0),key->cancelKey(0),key->menuKey(0),
      upKeyPressed(false), downKeyPressed(false), leftKeyPressed(false),
      rightKeyPressed(false), focusDir(NOTMOVING), focusDirCache(NOTMOVING),
      viewIsMoving(true), viewMoveTimerID(0), viewDelayTimerID(0),
      viewAnimationMoveTimerID(0), descriptionTimerID(0), labelSelectedIndex(0),
      weaponSelectedIndex(0), medicineSelectedIndex(0), winRequirement(0),
      loseRequirement(0), currentActiveHeroNum(0), currentActiveEnemyNum(0),
      currentActiveFriendNum(0), currentRoundNum(1),
      pArticleIDToInfoMap(&articleIDToInfoMap),
      weaponWidget(new WeaponWidget(this, &articleIDToInfoMap)),
      operationWidget(new OperationWidget(this)),
      cursorWidget(new CursorWidget(this)),
      descriptionWidget(new DescriptionWidget(this)) {
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->show();
  this->setFocus();
  //    DBIO dbio;
  //    if(!dbio.getKeySettings(key->upKey,key->downKey,key->leftKey,key->rightKey,
  //                        key->actionKey,key->cancelKey,key->menuKey))quitApp(ERRORSETTINGFILENOTFOUND);
  loadScene(battleID, selectedRoleID);
  connect(cursorWidget, SIGNAL(arrivedDestination()), this,
          SLOT(startAnimation()));
}
BattleScene::BattleScene(QWidget *parent, DBIO &dbio, KeyboardControl *key,
                         Role **roleInfo,
                         std::map<int, ArticleInfo> &articleIDToInfoMap)
    : QWidget(parent), mapSizeX(0), mapSizeY(0), mapSizePointX(0),
      mapSizePointY(0), currentPointX(0), currentPointY(0), mapArray(NULL),
      viewAutoMoveEndX(0), viewAutoMoveEndY(0), cursorOn(true),
      cursorControlOn(true), displayState(BattleNoExtraDisplay),
      focusRole(NULL), roleInfo(roleInfo), currentScriptIndex(0),
      userControlOn(false), directionKeyPressed(false), key(key),
      upKeyPressed(false), downKeyPressed(false), leftKeyPressed(false),
      rightKeyPressed(false), focusDir(NOTMOVING), focusDirCache(NOTMOVING),
      viewIsMoving(true), viewMoveTimerID(0), viewDelayTimerID(0),
      viewAnimationMoveTimerID(0), descriptionTimerID(0), labelSelectedIndex(0),
      weaponSelectedIndex(0), medicineSelectedIndex(0), winRequirement(0),
      loseRequirement(0), currentActiveHeroNum(0), currentActiveEnemyNum(0),
      currentActiveFriendNum(0), currentRoundNum(1),
      pArticleIDToInfoMap(&articleIDToInfoMap),
      weaponWidget(new WeaponWidget(this, &articleIDToInfoMap)),
      operationWidget(new OperationWidget(this)),
      cursorWidget(new CursorWidget(this)),
      descriptionWidget(new DescriptionWidget(this)) {
  connect(cursorWidget, SIGNAL(arrivedDestination()), this,
          SLOT(startAnimation()));

  this->setGeometry(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->show();
  this->setFocus();

  BattleSceneData battleSceneData(0, 0, 0, 0, 0, 0, 0);
  int *temp = NULL;
  if (!dbio.getBattleDataFromSaveFile(
          this, scriptVec, selfViewVec, enemyViewVec, friendViewVec,
          battleSceneData, &temp, winRequirement, loseRequirement))
    quitApp(ERRORSAVEFILEREADFAIL);

  this->battleID = battleSceneData.battleID;
  this->mapID = battleSceneData.mapID;
  this->currentPointX = battleSceneData.currentPointX;
  this->currentPointY = battleSceneData.currentPointY;
  this->currentRoundNum = battleSceneData.currentRoundNum;

  map.load(QApplication::applicationDirPath() +
           QString("/data/image/map/battle%1.jpg").arg(mapID));
  if (map.isNull())
    quitApp(ERRORIMAGEFILENOTFOUND);
  assert(map.width() % BASEWIDTH == 0);
  assert(map.height() % BASEHEIGHT == 0);
  mapSizeX = map.width() / BASEWIDTH;
  mapSizeY = map.height() / BASEHEIGHT;
  mapSizePointX = map.width();
  mapSizePointY = map.height();

  mapArray = new int *[mapSizeY];
  mapArray[0] = temp;
  for (int i = 1; i < mapSizeY; i++)
    mapArray[i] = mapArray[i - 1] + mapSizeX;
  // memcpy(mapArray[0],getMapPointerFromBattleID(battleID),sizeof(int)*mapSizeX*mapSizeY);

  //    int *p=reinterpret_cast<int *>(flagData);
  //    for(int i=0;i<length;++i)
  //    {
  //        int x=p[i]&0xff;
  //        int y=(p[i]&0xff00)>>8;
  //        int flag=p[i]&0xff0000;
  //        mapArray[y][x]|=flag;
  //    }
  //    //加载地图上的flag
  //    delete []flagData;
  //    flagData=NULL;
  currentActiveHeroNum = 0;
  currentActiveEnemyNum = 0;
  currentActiveFriendNum = 0;

  for (int i = 0; i < selfViewVec.size(); ++i) {
    int roleMapX = selfViewVec[i]->getMapX();
    int roleMapY = selfViewVec[i]->getMapY();
    selfViewVec[i]->setRolePointer(roleInfo);
    mapArray[roleMapY][roleMapX] &= 0xffff0000; //右一字节和右二字节清零
    mapArray[roleMapY][roleMapX] |= ((i << 8) | (BattleHero));
    selfViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                roleMapY * BASEHEIGHT - currentPointY,
                                BASEWIDTH, BASEHEIGHT);
    selfViewVec[i]->setMapArray(mapArray);
    if (selfViewVec[i]->getActiveStatus())
      currentActiveHeroNum++;
    // selfViewVec[i]->startAnimation(DOWN);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), selfViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(this, SIGNAL(changeActiveStatusTo(bool)), selfViewVec[i],
            SLOT(changeActiveStatusTo(bool)));
    connect(selfViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
  }
  for (int i = 0; i < enemyViewVec.size(); ++i) {
    int roleMapX = enemyViewVec[i]->getMapX();
    int roleMapY = enemyViewVec[i]->getMapY();
    mapArray[roleMapY][roleMapX] &= 0xffff0000; //右一字节和右二字节清零
    mapArray[roleMapY][roleMapX] |= ((i << 8) | (BattleEnemy));
    enemyViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                 roleMapY * BASEHEIGHT - currentPointY,
                                 BASEWIDTH, BASEHEIGHT);
    enemyViewVec[i]->setMapArray(mapArray);
    // enemyViewVec[i]->changeActiveStatusTo(false);
    // enemyViewVec[i]->constructRolePointer((minLevel+maxLevel)/2);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), enemyViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(enemyViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
    connect(enemyViewVec[i], SIGNAL(AIMoved(int, int)), this,
            SLOT(onAIMoved(int, int)));
  }
  for (int i = 0; i < friendViewVec.size(); ++i) {
    int roleMapX = friendViewVec[i]->getMapX();
    int roleMapY = friendViewVec[i]->getMapY();
    mapArray[roleMapY][roleMapX] &= 0xffff0000; //右一字节和右二字节清零
    mapArray[roleMapY][roleMapX] |= ((i << 8) | (BattleFriend));
    friendViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                  roleMapY * BASEHEIGHT - currentPointY,
                                  BASEWIDTH, BASEHEIGHT);
    friendViewVec[i]->setMapArray(mapArray);
    friendViewVec[i]->changeActiveStatusTo(false);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), friendViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(friendViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
    connect(friendViewVec[i], SIGNAL(AIMoved(int, int)), this,
            SLOT(onAIMoved(int, int)));
  }

  //    cursorWidget->moveToPointOfView(battleSceneData.cursorPointX,
  //                                    battleSceneData.currentPointY);
  cursorWidget->raise();

  emit viewHasMovedToPoint(currentPointX, currentPointY);
  this->show();
  this->setFocus();
  this->update();
  userControlOn = true;
}
BattleScene::~BattleScene() {
  delete[] mapArray[0];
  delete[] mapArray;
  mapArray = NULL;

  //    for(int i=0;i<selfViewVec.size();++i)
  //    {
  //        delete selfViewVec[i];
  //        selfViewVec[i]=NULL;
  //    }
  //    for(int i=0;i<enemyViewVec.size();++i)
  //    {
  //        delete enemyViewVec[i];
  //        enemyViewVec[i]=NULL;
  //    }
  //    for(int i=0;i<friendViewVec.size();++i)
  //    {
  //        delete friendViewVec[i];
  //        friendViewVec[i]=NULL;
  //    }

  //    weaponWidget->deleteLater();
  //    operationWidget->deleteLater();
  //    cursorWidget->deleteLater();
}
void BattleScene::loadScene(int battleID, vector<int> &selectedRoleID) {
  DBIO dbio;

  int startLocationX, startLocationY;
  if (!dbio.getBattleDataFromLib(battleID, enemyViewVec, friendViewVec,
                                 scriptVec, this, mapID, winRequirement,
                                 loseRequirement, startLocationX,
                                 startLocationY, roleInfo[0]->level))
    quitApp(ERRORGETBATTLEROLEDATAFROMLIBFAIL);
  cursorWidget->raise(); //将光标升到栈顶
  currentPointX = (startLocationX - WINDOWWIDTH / BASEWIDTH / 2) * BASEWIDTH;
  currentPointY = (startLocationY - WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT;

  RoleView *hero = NULL; //用于创建初始化的RoleView实例
  assert(selectedRoleID.size() <= 12);

  map.load(QApplication::applicationDirPath() +
           QString("/data/image/map/battle%1.jpg").arg(mapID));
  if (map.isNull())
    quitApp(ERRORIMAGEFILENOTFOUND);
  assert(map.width() % BASEWIDTH == 0);
  assert(map.height() % BASEHEIGHT == 0);
  mapSizeX = map.width() / BASEWIDTH;
  mapSizeY = map.height() / BASEHEIGHT;
  mapSizePointX = map.width();
  mapSizePointY = map.height();

  mapArray = new int *[mapSizeY];
  mapArray[0] = new int[mapSizeX * mapSizeY];
  for (int i = 1; i < mapSizeY; i++)
    mapArray[i] = mapArray[i - 1] + mapSizeX;
  memcpy(mapArray[0], getMapPointerFromBattleID(battleID),
         sizeof(int) * mapSizeX * mapSizeY);

  if (currentPointX < 0)
    currentPointX = 0;
  if (currentPointY < 0)
    currentPointY = 0;
  if (currentPointX > mapSizePointX - WINDOWWIDTH)
    currentPointX = mapSizePointX - WINDOWWIDTH;
  if (currentPointY > mapSizePointY - WINDOWHEIGHT)
    currentPointY = mapSizePointY - WINDOWHEIGHT;

  for (int i = 0; i < selectedRoleID.size(); ++i) {
    hero = new RoleView(this, startLocationX + startPointOffset[i][0],
                        startLocationY + startPointOffset[i][1],
                        selectedRoleID[i]);
    selfViewVec.push_back(hero);
  }

  for (int i = 0; i < selfViewVec.size(); ++i) {
    int roleMapX = selfViewVec[i]->getMapX();
    int roleMapY = selfViewVec[i]->getMapY();
    selfViewVec[i]->setRolePointer(roleInfo);
    mapArray[roleMapY][roleMapX] = (i << 8) | (BattleHero);
    selfViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                roleMapY * BASEHEIGHT - currentPointY,
                                BASEWIDTH, BASEHEIGHT);
    selfViewVec[i]->setMapArray(mapArray);
    selfViewVec[i]->startAnimation(DOWN);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), selfViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(this, SIGNAL(changeActiveStatusTo(bool)), selfViewVec[i],
            SLOT(changeActiveStatusTo(bool)));
    // connect(selfViewVec[i],SIGNAL(roleAnimationEnd()),this,SLOT(startAnimation()));
  }
  for (int i = 0; i < enemyViewVec.size(); ++i) {
    // qDebug()<<enemyViewVec[i]->getReactOnViewMove();
    int roleMapX = enemyViewVec[i]->getMapX();
    int roleMapY = enemyViewVec[i]->getMapY();
    mapArray[roleMapY][roleMapX] = (i << 8) | (BattleEnemy);
    enemyViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                 roleMapY * BASEHEIGHT - currentPointY,
                                 BASEWIDTH, BASEHEIGHT);
    enemyViewVec[i]->setMapArray(mapArray);
    enemyViewVec[i]->changeActiveStatusTo(false);
    // enemyViewVec[i]->constructRolePointer((minLevel+maxLevel)/2);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), enemyViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(enemyViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
    connect(enemyViewVec[i], SIGNAL(AIMoved(int, int)), this,
            SLOT(onAIMoved(int, int)));
    // connect(enemyViewVec[i],SIGNAL(AIMoved(DIRECTION,int,int)),cursorWidget,SLOT());
    //        connect(enemyViewVec[i],SIGNAL(hasStoppedAttack()),this,SLOT(startAnimation()));
    //        connect(enemyViewVec[i],SIGNAL(hasStoppedDodge()),this,SLOT(startAnimation()));
    //        connect(enemyViewVec[i],SIGNAL(hasStoppedHurt()),this,SLOT(startAnimation()));
    //        connect(enemyViewVec[i],SIGNAL(hasStoppedSweat()),this,SLOT(startAnimation()));
    //        connect(enemyViewVec[i],SIGNAL(hasStoppedDie()),this,SLOT(startAnimation()));
    //        connect(enemyViewVec[i],SIGNAL(hasStoppedMoving()),this,SLOT(startAnimation()));
  }
  for (int i = 0; i < friendViewVec.size(); ++i) {
    int roleMapX = friendViewVec[i]->getMapX();
    int roleMapY = friendViewVec[i]->getMapY();
    mapArray[roleMapY][roleMapX] = (i << 8) | (BattleFriend);
    friendViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                  roleMapY * BASEHEIGHT - currentPointY,
                                  BASEWIDTH, BASEHEIGHT);
    friendViewVec[i]->setMapArray(mapArray);
    friendViewVec[i]->changeActiveStatusTo(false);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), friendViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(friendViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
    connect(friendViewVec[i], SIGNAL(AIMoved(int, int)), this,
            SLOT(onAIMoved(int, int)));
  }

  currentActiveHeroNum = selfViewVec.size();
  currentActiveEnemyNum = enemyViewVec.size();
  currentActiveFriendNum = friendViewVec.size();
  //    mapArray=new int*[mapSizeY];
  //    mapArray[0]=new int[mapSizeX*mapSizeY];
  //    for(int i = 1; i <mapSizeY; i++)
  //        mapArray[i] = mapArray[i-1]+mapSizeX;
  //    memcpy(mapArray[0],getPointerFromMapID(mapID),sizeof(int)*mapSizeX*mapSizeY);
  // emit battleSceneBuilt(this);

  if (scriptVec.empty())
    scriptVec.push_back(
        Script(AniStart)); //如果数据文件没有向其中添加动画的话,此处添加动画头
  scriptVec.push_back(Script(AniShowBattleStart));
  scriptVec.push_back(Script(AniShowRoundX, QString("1")));

  assert(selfViewVec[0]);

  scriptVec.push_back(
      Script(AniViewMoveTo, QString("%1#%2#")
                                .arg(selfViewVec[0]->getMapX())
                                .arg(selfViewVec[0]->getMapY())));

  int x = selfViewVec[0]->getMapX() - WINDOWWIDTH / BASEWIDTH / 2;
  int y = selfViewVec[0]->getMapY() - WINDOWHEIGHT / BASEHEIGHT / 2;
  if (x < 0)
    x = 0;
  if (x > (mapSizePointX - WINDOWWIDTH) / BASEWIDTH)
    x = (mapSizePointX - WINDOWWIDTH) / BASEWIDTH;

  if (y < 0)
    y = 0;
  if (y > (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT)
    y = (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT;

  scriptVec.push_back(
      Script(AniCursorMoveToViewPoint,
             QString("%1#%2#")
                 .arg(selfViewVec[0]->getPointX() - x * BASEWIDTH)
                 .arg(selfViewVec[0]->getPointY() - y * BASEHEIGHT)));

  scriptVec.push_back(Script(AniEnd));

  //    for(int i=0;i<scriptVec.size();++i)
  //    {
  //        qDebug()<<scriptVec[i].op;
  //        qDebug()<<scriptVec[i].param;
  //    }

  // currentScriptIndex=scriptVec.size()-1;
  //    cursorWidget->moveToPointOfView(selfViewVec[0]->getPointX()-x*BASEWIDTH,
  //                                    selfViewVec[0]->getPointY()-y*BASEHEIGHT);
  cursorWidget->raise();
  this->startAnimation();
  emit viewHasMovedToPoint(currentPointX, currentPointY);
}
void BattleScene::onDirectionKeyPress(DIRECTION dir) {
  // qDebug()<<"onDirectionKeyPress";
  switch (displayState) {
  case BattleShowSelfRange:
  case BattleShowSelfMedicineRange:
    break;
  case BattleNoExtraDisplay: //光标移动

  case BattleShowEnemyWalkRange:
  case BattleShowEnemyRange:
  case BattleShowFriendRange:
  case BattleShowFriendWalkRange:
    if (viewMoveTimerID == 0) {
      focusDir = dir;
      focusDirCache = dir;
      viewMoveTimerID = startTimer(VIEWMOVEINTERVAL);
      move(); //按下之后立即移动，而不是等待下一次定时器到来时才移动
    } else
      focusDirCache = dir;
  case BattleShowSelfWalkRange: // focusRole=NULL移动,通过cursorControlOn来区分
    if (viewMoveTimerID == 0) {
      focusDir = dir;
      focusDirCache = dir;
      focusRole->startAnimation(focusDir);
      viewMoveTimerID = startTimer(VIEWMOVEINTERVAL);
      move();
    } else
      focusDirCache = dir;
    break;
  case BattleShowLabel:
  case BattleShowWeapon:
  case BattleShowMedicine:
    break;
  default:
    assert(0);
  }
}
void BattleScene::onDirectionKeyRelease(DIRECTION dir) {
  // qDebug()<<"onDirectionKeyRelease";
  switch (displayState) {
  case BattleShowSelfRange:
  case BattleShowSelfMedicineRange:
    this->selectTarget(dir);
    break;
  case BattleNoExtraDisplay: //光标移动
  case BattleShowEnemyWalkRange:
  case BattleShowEnemyRange:
  case BattleShowFriendRange:
  case BattleShowFriendWalkRange:
    if (upKeyPressed)
      focusDirCache = UP;
    else if (downKeyPressed)
      focusDirCache = DOWN;
    else if (leftKeyPressed)
      focusDirCache = LEFT;
    else if (rightKeyPressed)
      focusDirCache = RIGHT;
    break;
  case BattleShowLabel:
    // qDebug()<<labelSelectedIndex;
    assert(operationWidget);
    if (dir == UP || dir == LEFT) {
      if (labelSelectedIndex == 0)
        labelSelectedIndex = 3;
      else if ((focusRole->getMapX() != focusRoleMapXCache ||
                focusRole->getMapY() != focusRoleMapYCache) &&
               labelSelectedIndex == 2) //动了
      {
        labelSelectedIndex = 0; //跳过1
      } else {
        labelSelectedIndex--;
      }
    } else {
      if (labelSelectedIndex == 3)
        labelSelectedIndex = 0;
      else if ((focusRole->getMapX() != focusRoleMapXCache ||
                focusRole->getMapY() != focusRoleMapYCache) &&
               labelSelectedIndex == 0) {
        labelSelectedIndex = 2; //向右跳过1
      } else
        labelSelectedIndex++;
    }
    operationWidget->show(focusRole->getPointX() - currentPointX,
                          focusRole->getPointY() - currentPointY,
                          labelSelectedIndex);
    break;
  case BattleShowWeapon:
  case BattleShowMedicine:
    assert(weaponWidget);
    if (dir == UP || dir == LEFT) {
      medicineSelectedIndex = weaponWidget->indexGoUp();
      weaponSelectedIndex = medicineSelectedIndex;
    } else {
      medicineSelectedIndex = weaponWidget->indexGoDown();
      weaponSelectedIndex = medicineSelectedIndex;
    }
    break;
  case BattleShowSelfWalkRange: // focusRole=NULL移动,通过cursorOn来区分
    if (upKeyPressed)
      focusDirCache = UP;
    else if (downKeyPressed)
      focusDirCache = DOWN;
    else if (leftKeyPressed)
      focusDirCache = LEFT;
    else if (rightKeyPressed)
      focusDirCache = RIGHT;
    break;

  default:
    qDebug() << displayState;
    assert(0);
  }
  this->update();
}
void BattleScene::onActionKey() {
  // if(viewMoveTimerID!=0)return;
  assert(viewMoveTimerID == 0);
  assert(viewDelayTimerID == 0);
  assert(viewAnimationMoveTimerID == 0);
  int cursorX = (cursorWidget->getViewPointX() + currentPointX) / BASEWIDTH;
  int cursorY = (cursorWidget->getViewPointY() + currentPointY) / BASEHEIGHT;
  // qDebug()<<mapArray[cursorY][cursorX];
  switch (displayState) {
  case BattleNoExtraDisplay:
    if (static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
        static_cast<int>(BattleHero)) // cursor所在处是一个hero
    {
      if (((mapArray[cursorY][cursorX] & 0x0f0000) >> 16) !=
          0) { //如果处于停止行动状态则直接返回
        break;
      }

      RoleView *tempRole =
          selfViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];

      if (tempRole->getActiveStatus()) //只有活动时候才能显示出标签
      {
        focusRole = tempRole;
        focusRole->raise();
        focusRole->setReactOnViewMove(false);
        cursorControlOn = false;
        displayState = BattleShowLabel;
        operationWidget->show(focusRole->getPointX() - currentPointX,
                              focusRole->getPointY() - currentPointY, 0);
        operationWidget->raise();
        focusRoleMapXCache = cursorX;
        focusRoleMapYCache = cursorY;
      }
    } else if (static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
               static_cast<int>(BattleEnemy)) {
      focusRole = enemyViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      displayState = BattleShowEnemyWalkRange;
      addRangeIntoVector();
    } else if (static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
               static_cast<int>(BattleFriend)) {
      focusRole = friendViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      displayState = BattleShowFriendWalkRange;
    }
    break;
  case BattleShowLabel:
    operationWidget->hide();
    switch (labelSelectedIndex) {
    case 0:
      displayState = BattleShowWeapon;
      weaponSelectedIndex = 0;
      weaponWidget->show(focusRole->getPointX() - currentPointX,
                         focusRole->getPointY() - currentPointY,
                         focusRole->getRolePointer(), ShowWeapon);
      break;
    case 1:
      displayState = BattleShowSelfWalkRange;
      cursorControlOn = false;
      this->addRangeIntoVector();
      break;
    case 2:
      if (focusRole->getMapX() != focusRoleMapXCache ||
          focusRole->getMapY() != focusRoleMapYCache) //动了
      {
        assert(scriptVec.empty());
        scriptVec.push_back(Script(AniStart));
        scriptVec.push_back(Script(AniFocusRoleBeInactive));
      } else {
        assert(scriptVec.empty());
        scriptVec.push_back(Script(AniStart));
        this->roleRecover();
      }
      // PRINTSCRIPTVEC
      this->startAnimation();
      displayState = BattleNoExtraDisplay;
      break;
    case 3:
      medicineSelectedIndex = 0;
      displayState = BattleShowMedicine;
      weaponWidget->show(focusRole->getPointX() - currentPointX,
                         focusRole->getPointY() - currentPointY,
                         focusRole->getRolePointer(), ShowMedicine);
      break;
    default:
      qDebug() << labelSelectedIndex;
      assert(0);
    }
    break;
  case BattleShowWeapon:
    //这时焦点肯定在某一个武器上，按下就使用之
    weaponWidget->hide();
    focusRole->setReactOnViewMove(true);
    cursorControlOn = true;
    displayState = BattleShowSelfRange;
    addRangeIntoVector();
    assert(focusRole);
    // articleSpecial=(*pArticleIDToInfoMap)[focusRole->getRolePointer()->weapon[weaponSelectedIndex]].special;
    //将武器的特效记录下来备用
    break;
  case BattleShowMedicine: {
    assert(focusRole);
    weaponWidget->hide();
    focusRole->setReactOnViewMove(true);
    Role *role = focusRole->getRolePointer();
    if (role->medicine[0] + role->medicine[1] + role->medicine[2] +
            role->medicine[3] ==
        0)
      break; //没有携带任何药物
    cursorControlOn = true;
    displayState =
        BattleShowSelfMedicineRange; //可以对别人使用药物，因此显示的是射程
    addRangeIntoVector();
    // articleSpecial=(*pArticleIDToInfoMap)[focusRole->getRolePointer()->medicine[medicineSelectedIndex]].special;
    //将药物的特效记录下来备用

  } break;
  case BattleShowSelfWalkRange:
    //确定到达某点，显示武器准备攻击
    if (mapArray[focusRole->getMapY()][focusRole->getMapX()] != 0)
      break; //允许跨越己方角色但是不允许停留
    labelSelectedIndex = 0;
    operationWidget->show(focusRole->getPointX() - currentPointX,
                          focusRole->getPointY() - currentPointY,
                          labelSelectedIndex);
    displayState = BattleShowLabel;
    swap(mapArray[focusRole->getMapY()][focusRole->getMapX()],
         mapArray[focusRoleMapYCache][focusRoleMapXCache]);
    break;
  case BattleShowSelfRange:
    // if((mapArray[cursorY][cursorX]&0xff)!=static_cast<int>(BattleEnemy))break;
    assert(scriptVec.empty());

    scriptVec.push_back(Script(AniStart));
    if ((mapArray[cursorY][cursorX] & 0xff) ==
        static_cast<int>(
            BattleEnemy)) { //攻击性武器,只用于对敌方角色,由负责目标选择的代码保证攻击性武器选择的目标一定是敌人

      targetRole = enemyViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      this->roleAttack(focusRole->getMapX(), focusRole->getMapY());

    } else if ((mapArray[cursorY][cursorX] & 0xff) ==
               static_cast<int>(BattleHero)) { //回复和治疗性武器
      targetRole = selfViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      int special = (*pArticleIDToInfoMap)[focusRole->getRolePointer()
                                               ->weapon[weaponSelectedIndex]]
                        .special;
      //判断治疗还是回复
      if (special > WEAPONRECOVERLOWBOUND &&
          special < WEAPONRECOVERUPBOUND) { //是回复
        this->roleCure(special);
      } else if (0) { //是治疗
      }
    } else {
      targetRole = friendViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      int special = (*pArticleIDToInfoMap)[focusRole->getRolePointer()
                                               ->weapon[weaponSelectedIndex]]
                        .special;
      //判断治疗还是回复
      this->roleCure(special);
    }
    scriptVec.push_back(Script(AniEnd));
    // assert(static_cast<int>(mapArray[cursorY][cursorX]&0xff)==static_cast<int>(BattleEnemy));
    this->startAnimation();
    displayState = BattleNoExtraDisplay;

    break;
  case BattleShowSelfMedicineRange:
    // assert(static_cast<int>(mapArray[cursorY][cursorX]&0xff)==static_cast<int>(BattleHero));
    // targetRole=enemyViewVec[(mapArray[cursorY][cursorX]&0xff00)>>8];
    assert(scriptVec.empty());
    scriptVec.push_back(Script(AniStart));
    if ((mapArray[cursorY][cursorX] & 0xff) ==
        static_cast<int>(BattleEnemy)) { //攻击性药品,对敌人使用
    } else if ((mapArray[cursorY][cursorX] & 0xff) ==
               static_cast<int>(BattleHero)) { //恢复或者治疗性药品
      targetRole = selfViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      int special =
          (*pArticleIDToInfoMap)[focusRole->getRolePointer()
                                     ->medicine[medicineSelectedIndex]]
              .special;
      qDebug() << medicineSelectedIndex;
      if (special > MEDICINEORIGIN) { //是治疗
        int *p = focusRole->getRolePointer()->medicine;
        memmove(p + medicineSelectedIndex, p + (medicineSelectedIndex + 1),
                (MAXMEDICINECOUNT - 1 - medicineSelectedIndex) * sizeof(int));
        p[MAXMEDICINECOUNT - 1] = 0;
        medicineSelectedIndex = 0;
        this->roleCure(special);
      }
    } else {
      assert(0); //还未完成的代码的assert
    }
    scriptVec.push_back(Script(AniEnd));
    this->startAnimation();

    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowEnemyRange:
    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowEnemyWalkRange:
    //对敌人按一下显示移动范围，按两下显示射程

    if (((mapArray[cursorY][cursorX] & 0xff) ==
         static_cast<int>(BattleEnemy)) &&
        (focusRole ==
         enemyViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8])) {
      displayState = BattleShowEnemyRange;
      this->addRangeIntoVector();
    }

    else //如果还是在原来的位置上按下的action键,则显示射程,否则直接取消显示
    {
      //            qDebug()<<focusRole;
      //            qDebug()<<enemyViewVec[0];
      focusRole = NULL;
      displayState = BattleNoExtraDisplay;
    }
    break;
  case BattleShowFriendRange:
    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowFriendWalkRange:
    if ((mapArray[cursorY][cursorX] & 0xff) == static_cast<int>(BattleFriend) &&
        focusRole ==
            friendViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8]) {
      displayState = BattleShowFriendRange;
      this->addRangeIntoVector();
    } else //如果还是在原来的位置上按下的action键,则显示射程,否则直接取消显示
    {
      focusRole = NULL;
      displayState = BattleNoExtraDisplay;
    }
    break;

  default:
    break;
  }
  this->update();
}
void BattleScene::onCancelKey() {
  switch (displayState) {
  case BattleNoExtraDisplay:
    break;
  case BattleShowLabel:
    operationWidget->hide();
    if (focusRole->getMapX() != focusRoleMapXCache ||
        focusRole->getMapY() != focusRoleMapYCache) {
      int x = (focusRoleMapXCache - (WINDOWWIDTH / BASEWIDTH / 2)) * BASEWIDTH;
      int y =
          (focusRoleMapYCache - (WINDOWHEIGHT / BASEHEIGHT / 2)) * BASEHEIGHT;
      if (x < 0)
        currentPointX = 0;
      else if (x > mapSizePointX - WINDOWWIDTH)
        currentPointX = mapSizePointX - WINDOWWIDTH;
      else
        currentPointX = x;
      if (y < 0)
        currentPointY = 0;
      else if (y > mapSizePointY - WINDOWHEIGHT)
        currentPointY = mapSizePointY - WINDOWHEIGHT;
      else
        currentPointY = y;

      emit viewHasMovedToPoint(currentPointX, currentPointY);
      swap(mapArray[focusRoleMapYCache][focusRoleMapXCache],
           mapArray[focusRole->getMapY()][focusRole->getMapX()]);

      focusRole->setViewPointXY(currentPointX, currentPointY);
      focusRole->moveToPointOfMap(focusRoleMapXCache * BASEWIDTH,
                                  focusRoleMapYCache * BASEHEIGHT);

      cursorWidget->moveToPointOfView(
          focusRoleMapXCache * BASEWIDTH - currentPointX,
          focusRoleMapYCache * BASEHEIGHT - currentPointY);
    }
    focusRole->setOriginalState();
    focusRole = NULL;
    cursorControlOn = true;
    labelSelectedIndex = 0;
    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowWeapon:
    weaponWidget->hide();
    operationWidget->show(focusRole->getPointX() - currentPointX,
                          focusRole->getPointY() - currentPointY, 0);
    operationWidget->raise();
    displayState = BattleShowLabel;
    break;
  case BattleShowMedicine:
    weaponWidget->hide();
    operationWidget->show(focusRole->getPointX() - currentPointX,
                          focusRole->getPointY() - currentPointY, 3);
    operationWidget->raise();
    displayState = BattleShowLabel;
    break;
  case BattleShowSelfWalkRange:
    if (focusRole->getMapX() != focusRoleMapXCache ||
        focusRole->getMapY() != focusRoleMapYCache) //已经移动了
    {
      int x = (focusRoleMapXCache - (WINDOWWIDTH / BASEWIDTH / 2)) * BASEWIDTH;
      int y =
          (focusRoleMapYCache - (WINDOWHEIGHT / BASEHEIGHT / 2)) * BASEHEIGHT;
      if (x < 0)
        currentPointX = 0;
      else if (x > mapSizePointX - WINDOWWIDTH)
        currentPointX = mapSizePointX - WINDOWWIDTH;
      else
        currentPointX = x;
      if (y < 0)
        currentPointY = 0;
      else if (y > mapSizePointY - WINDOWHEIGHT)
        currentPointY = mapSizePointY - WINDOWHEIGHT;
      else
        currentPointY = y;
      emit viewHasMovedToPoint(currentPointX, currentPointY);
      focusRole->setViewPointXY(currentPointX, currentPointY);
      focusRole->moveToPointOfMap(focusRoleMapXCache * BASEWIDTH,
                                  focusRoleMapYCache * BASEHEIGHT);
      cursorWidget->moveToPointOfView(
          focusRoleMapXCache * BASEWIDTH - currentPointX,
          focusRoleMapYCache * BASEHEIGHT - currentPointY);
      swap(mapArray[focusRole->getMapY()][focusRole->getMapX()],
           mapArray[focusRoleMapYCache][focusRoleMapXCache]);
      displayState = BattleShowLabel;
    } else {
      displayState = BattleShowLabel;
    }
    operationWidget->show(focusRole->getPointX() - currentPointX,
                          focusRole->getPointY() - currentPointY, 0);
    operationWidget->raise();
    break;
  case BattleShowSelfRange:
    assert(focusRole);

    {
      int x = (focusRole->getMapX() - WINDOWWIDTH / BASEWIDTH / 2) * BASEWIDTH;
      int y =
          (focusRole->getMapY() - WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT;
      if (x < 0)
        currentPointX = 0;
      else if (x > mapSizePointX - WINDOWWIDTH)
        currentPointX = mapSizePointX - WINDOWWIDTH;
      else
        currentPointX = x;

      if (y < 0)
        currentPointY = 0;
      else if (y > mapSizePointY - WINDOWHEIGHT)
        currentPointY = mapSizePointY - WINDOWHEIGHT;
      else
        currentPointY = y;
      emit viewHasMovedToPoint(currentPointX, currentPointY);
    }
    focusRole->setReactOnViewMove(false);
    cursorWidget->moveToPointOfView(focusRole->getPointX() - currentPointX,
                                    focusRole->getPointY() - currentPointY);
    weaponWidget->show(focusRole->getPointX() - currentPointX,
                       focusRole->getPointY() - currentPointY,
                       focusRole->getRolePointer(), ShowWeapon);
    displayState = BattleShowWeapon;
    break;
  case BattleShowSelfMedicineRange:
    displayState = BattleShowLabel;
    operationWidget->show(focusRole->getPointX() - currentPointX,
                          focusRole->getPointY() - currentPointY, 3);
    break;
  case BattleShowEnemyRange:
    focusRole = NULL;
    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowEnemyWalkRange:
    focusRole = NULL;
    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowFriendRange:
    focusRole = NULL;
    displayState = BattleNoExtraDisplay;
    break;
  case BattleShowFriendWalkRange:
    focusRole = NULL;
    displayState = BattleNoExtraDisplay;
    break;

  default:
    break;
  }
  this->update();
}
void BattleScene::onMenuKey() {
  if (displayState != BattleNoExtraDisplay)
    return;
  MenuWidget *menu = new MenuWidget(this, key, BattleMenu);
  connect(menu, SIGNAL(selectIndex(int)), this, SLOT(onMenuSelect(int)));
  this->update();
}
void BattleScene::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  // qDebug()<<currentPointX<<" "<<currentPointY;
  painter.drawPixmap(
      QPoint(0, 0), map,
      QRect(currentPointX, currentPointY, WINDOWWIDTH, WINDOWHEIGHT));
  switch (displayState) {
  case BattleNoExtraDisplay:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    // cursorWidget->raise();
    //        if(cursorWidget->isHidden())
    //            cursorWidget->show();
    break;
  case BattleShowLabel:
    //        weaponWidget->hide();
    //        if(operationWidget->isHidden())
    //        {
    //            operationWidget->raise();
    //            operationWidget->show(focusRole->getPointX()-currentPointX,focusRole->getPointY()-currentPointY,labelSelectedIndex);
    //        }
    break;
  case BattleShowWeapon: //武器选择界面
                         //        operationWidget->hide();
                         //        weaponWidget->raise();
    //        weaponWidget->show(focusRole->getPointX()-currentPointX,focusRole->getPointY()-currentPointY,focusRole->getRolePointer(),ShowWeapon);
    break;
  case BattleShowMedicine:
    //        operationWidget->hide();
    //        weaponWidget->raise();
    //        weaponWidget->show(focusRole->getPointX()-currentPointX,focusRole->getPointY()-currentPointY,focusRole->getRolePointer(),ShowMedicine);
    break;
  case BattleShowSelfWalkRange:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(0, 0, 255, 128));
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  case BattleShowSelfRange:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(0, 255, 255, 128));
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  case BattleShowEnemyRange:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(255, 0, 0, 128)); //敌人的射程是红色
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  case BattleShowEnemyWalkRange:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(255, 0, 0, 128)); //敌人的行走范围是红色
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  case BattleShowFriendRange:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(0, 255, 0, 128)); //友军的射程是绿色
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  case BattleShowFriendWalkRange:
    //        operationWidget->hide();
    //        weaponWidget->hide();
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(0, 255, 0, 128)); //友军的行走范围是绿色
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  case BattleShowSelfMedicineRange:
    for (int i = 0; i < rangeVec.size(); ++i) {
      painter.setBrush(QColor(255, 255, 0, 128)); //友军的行走范围是绿色
      painter.drawRect(BASEWIDTH * rangeVec[i].x() - currentPointX,
                       BASEHEIGHT * rangeVec[i].y() - currentPointY, BASEWIDTH,
                       BASEHEIGHT);
    }
    break;
  default:
    qDebug() << displayState;
    assert(0);
  }
}
void BattleScene::keyPressEvent(QKeyEvent *event) {
  // qDebug()<<"onKeyPress";
#ifndef NDEBUG
  if (event->key() == Qt::Key_1)
    this->win();
  else if (event->key() == Qt::Key_2)
    this->fail();
#endif
  if (!userControlOn)
    return;
  descriptionWidget->hide();
  if (!descriptionTimerID) { //重启定时器
    descriptionTimerID = startTimer(DESCRIPTIONDELAY);
  } else {
    killTimer(descriptionTimerID);
    descriptionWidget->hide();
    descriptionTimerID = startTimer(DESCRIPTIONDELAY);
  }

  if (!event->isAutoRepeat()) {
    if (event->key() == key->leftKey) {
      leftKeyPressed = true;
      onDirectionKeyPress(LEFT);
    } else if (event->key() == key->upKey) {
      upKeyPressed = true;
      onDirectionKeyPress(UP);
    } else if (event->key() == key->downKey) {
      downKeyPressed = true;
      onDirectionKeyPress(DOWN);
    } else if (event->key() == key->rightKey) {
      rightKeyPressed = true;
      onDirectionKeyPress(RIGHT);
    }
    update();
  }
}
void BattleScene::keyReleaseEvent(QKeyEvent *event) {
  if (!userControlOn)
    return;

  descriptionWidget->hide();
  descriptionTimerID = startTimer(DESCRIPTIONDELAY);
  if (!descriptionTimerID) { //重启定时器
    descriptionTimerID = startTimer(DESCRIPTIONDELAY);
  } else {

    killTimer(descriptionTimerID);
    descriptionTimerID = startTimer(DESCRIPTIONDELAY);
  }

  if (!event->isAutoRepeat()) {

    if (event->key() == key->actionKey) {
      if (viewMoveTimerID != 0)
        return;
      onActionKey();
    } else if (event->key() == key->cancelKey) {
      if (viewMoveTimerID != 0)
        return;
      onCancelKey();
    } else if (event->key() == key->menuKey) {
      if (viewMoveTimerID != 0)
        return;
      onMenuKey();
    } else if (event->key() == key->upKey) {
      upKeyPressed = false;
      onDirectionKeyRelease(UP);
    } else if (event->key() == key->downKey) {
      downKeyPressed = false;
      onDirectionKeyRelease(DOWN);
    } else if (event->key() == key->rightKey) {
      rightKeyPressed = false;
      onDirectionKeyRelease(RIGHT);
    } else if (event->key() == key->leftKey) {
      leftKeyPressed = false;
      onDirectionKeyRelease(LEFT);
    }
    update();
  }
}
void BattleScene::timerEvent(QTimerEvent *event) {
  // qDebug()<<"timerEvent";
  if (event->timerId() ==
      viewMoveTimerID) { //使用userControlOn来区分view是否是自动移动
    move();
  } else if (event->timerId() == viewDelayTimerID) {
    killTimer(viewDelayTimerID);
    viewDelayTimerID = 0;
    this->startAnimation();
  } else if (event->timerId() == viewAnimationMoveTimerID) {
    viewMove(NOTMOVING, 0, 0);
  } else if (event->timerId() == descriptionTimerID) {
    if (!userControlOn || !this->hasFocus()) //如果当前没有焦点则不显示
      return;
    int cursorX = (cursorWidget->getViewPointX() + currentPointX) / BASEWIDTH;
    int cursorY = (cursorWidget->getViewPointY() + currentPointY) / BASEHEIGHT;
    RoleView *cursorRole = NULL; //光标所在处的cursorRole
    // qDebug()<<"description";
    killTimer(descriptionTimerID);
    descriptionTimerID = 0;
    switch (displayState) {
    case BattleNoExtraDisplay:
    case BattleShowSelfRange:
    case BattleShowSelfWalkRange:
    case BattleShowSelfMedicineRange:
    case BattleShowEnemyRange:
    case BattleShowEnemyWalkRange:
    case BattleShowFriendRange:
    case BattleShowFriendWalkRange:
      if (static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
          static_cast<int>(BattleHero)) // cursor所在处是一个hero
      {
        cursorRole = selfViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
        descriptionWidget->show(cursorRole->getPointX() - currentPointX,
                                cursorRole->getPointY() - currentPointY,
                                // AniRoleDisable);
                                cursorRole->getRolePointer());
        // show role decription
      } else if (static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
                 static_cast<int>(BattleEnemy)) {
        cursorRole = enemyViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
        descriptionWidget->show(cursorRole->getPointX() - currentPointX,
                                cursorRole->getPointY() - currentPointY,
                                cursorRole->getRolePointer());
      } else if (static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
                 static_cast<int>(BattleFriend)) {
        cursorRole = friendViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
        descriptionWidget->show(cursorRole->getPointX() - currentPointX,
                                cursorRole->getPointY() - currentPointY,
                                cursorRole->getRolePointer());
      }
      descriptionWidget->raise();
      break;
    case BattleShowLabel:
      assert(static_cast<int>(mapArray[cursorY][cursorX] & 0xff) ==
             static_cast<int>(BattleHero));
      cursorRole = selfViewVec[(mapArray[cursorY][cursorX] & 0xff00) >> 8];
      descriptionWidget->show(cursorRole->getPointX() - currentPointX,
                              cursorRole->getPointY() - currentPointY,
                              labelSelectedIndex);
      descriptionWidget->raise();
      // show operation label description
      break;
    default:
      break;
    }
  }
}
void BattleScene::startAnimation() {
  if (scriptVec.empty())
    return;
  Script *pScript = &scriptVec[currentScriptIndex];
#ifndef NDEBUG
  qDebug() << currentScriptIndex << ":" << pScript->op << " " << pScript->param;
  // qDebug()<<"x "<<currentPointX<<" y "<<currentPointY;
#endif;
  // DialogLabel *dialogLabel=NULL;
  // QPropertyAnimation *propertyAnimation=NULL;
  // BattleLabel *battleLabel=NULL;

  int x = 0;
  int y = 0;
  currentScriptIndex++;

  switch (pScript->op) {
  case AniStart:
    userControlOn = false;
    //            displayState=BattleShowAnimation;
    this->startAnimation();
    break;
  case AniViewMoveTo:
    //参数是中心点坐标,首先换算成左上角坐标
    if (viewAnimationMoveTimerID == 0) {
      viewAutoMoveEndX = getSubFromQString(pScript->param, 1, '#').toInt() -
                         WINDOWWIDTH / BASEWIDTH / 2;
      viewAutoMoveEndY = getSubFromQString(pScript->param, 2, '#').toInt() -
                         WINDOWHEIGHT / BASEHEIGHT / 2;
      if (viewAutoMoveEndX < 0)
        viewAutoMoveEndX = 0;
      if (viewAutoMoveEndX > (mapSizePointX - WINDOWWIDTH) / BASEWIDTH)
        viewAutoMoveEndX = (mapSizePointX - WINDOWWIDTH) / BASEWIDTH;
      if (viewAutoMoveEndY < 0)
        viewAutoMoveEndY = 0;
      if (viewAutoMoveEndY > (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT)
        viewAutoMoveEndY = (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT;
      viewAnimationMoveTimerID = startTimer(VIEWMOVEINTERVAL);
    }
    break;
  case AniRoleSetPath:
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    assert((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy) ||
           (mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend));
    if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy)) {
      enemyViewVec[(mapArray[y][x] & 0xff00) >> 8]->setPathVec(pScript->param);
    } else {
      friendViewVec[(mapArray[y][x] & 0xff00) >> 8]->setPathVec(pScript->param);
    }
    this->startAnimation();
    break;
  case AniRoleMove:
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    assert((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy) ||
           (mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend));
    if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy)) {
      enemyViewVec[(mapArray[y][x] & 0xff00) >> 8]->startMoving(false);
    } else {
      friendViewVec[(mapArray[y][x] & 0xff00) >> 8]->startMoving(false);
    }
    break;
  case AniRoleFastMove:

    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy)) {
      displayState = BattleShowEnemyWalkRange;
      addRangeIntoVector();
      enemyViewVec[(mapArray[y][x] & 0xff00) >> 8]->startMoving(true);
    } else {
      displayState = BattleShowFriendWalkRange;
      addRangeIntoVector();
      friendViewVec[(mapArray[y][x] & 0xff00) >> 8]->startMoving(true);
    }
    // qDebug()<<"x:"<<currentPointX<<" y:"<<currentPointY;
    break;
  case AniRoleAttack: {
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    if ((mapArray[y][x] & 0xff) ==
        static_cast<int>(BattleEnemy)) { //敌方角色攻击的时候显示射程
      displayState = BattleShowEnemyRange;
      addRangeIntoVector();
    } else if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend)) {
      displayState = BattleShowFriendRange;
      addRangeIntoVector();
    }
  }
    //这里没有break
  case AniRoleSweat:
  case AniRoleDie:
  case AniRoleLevelUp:
  case AniRoleChangeJob:
  case AniRoleAttention:
  case AniRoleCure:
  case AniRoleUseMagic: {
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    RoleAnimation *animation =
        new RoleAnimation(this, x * BASEWIDTH - currentPointX,
                          y * BASEHEIGHT - currentPointY, pScript->op);
    animation->start(true);
    while (cutHeadFromQString(pScript->param, 2, '#').length() != 0) {
      x = getSubFromQString(pScript->param, 1, '#').toInt();
      y = getSubFromQString(pScript->param, 2, '#').toInt();
      RoleAnimation *animation =
          new RoleAnimation(this, x * BASEWIDTH - currentPointX,
                            y * BASEHEIGHT - currentPointY, pScript->op);
      animation->start(false);
    }
  } break;
  case AniRoleRecover:
  case AniRoleHurt: {
    displayState = BattleNoExtraDisplay;
    //敌人攻击时要显示敌人的射程,将displayState置为了showenemyrange,这里恢复原来的值
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    int originalHP = getSubFromQString(pScript->param, 3, '#').toInt();
    int deltaHP = getSubFromQString(pScript->param, 4, '#').toInt();
    int fullHP = getSubFromQString(pScript->param, 5, '#').toInt();
    HPSlotWidget *w = new HPSlotWidget(
        this, x * BASEWIDTH - currentPointX, y * BASEHEIGHT - currentPointY,
        originalHP, deltaHP, fullHP, pScript->op);
    w->start(true);
    while (cutHeadFromQString(pScript->param, 5, '#').length() != 0) {
      x = getSubFromQString(pScript->param, 1, '#').toInt();
      y = getSubFromQString(pScript->param, 2, '#').toInt();
      int originalHP = getSubFromQString(pScript->param, 3, '#').toInt();
      int deltaHP = getSubFromQString(pScript->param, 4, '#').toInt();
      int fullHP = getSubFromQString(pScript->param, 5, '#').toInt();
      HPSlotWidget *w = new HPSlotWidget(
          this, x * BASEWIDTH - currentPointX, y * BASEHEIGHT - currentPointY,
          originalHP, deltaHP, fullHP, pScript->op);
      w->start(false);
    }
  } break;
  case AniRoleDisable:
  case AniRoleAttackDown:
  case AniRoleAccurcyDown:
  case AniRoleCritDown:
  case AniRoleDefenceDown:
  case AniRoleDodgeDown:
  case AniRoleSpeedDown:
  case AniRolePoison:

  {
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    descriptionWidget->show(x * BASEWIDTH - currentPointX,
                            y * BASEHEIGHT - currentPointY, pScript->op);
  } break;
  case AniRoleDodge:
    displayState = BattleNoExtraDisplay;
    //敌人攻击时要显示敌人的射程,将displayState置为了showenemyrange,这里恢复原来的值
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleHero)) {
      selfViewVec[(mapArray[y][x] & 0xff00) >> 8]->dodge(true);
    } else if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy)) {
      enemyViewVec[(mapArray[y][x] & 0xff00) >> 8]->dodge(true);
    } else {
      assert((mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend));
      friendViewVec[(mapArray[y][x] & 0xff00) >> 8]->dodge(true);
    }
    break;
  case AniRoleRotate:
    displayState = BattleNoExtraDisplay;
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    assert((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy) ||
           (mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend));
    if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleEnemy)) {
      enemyViewVec[(mapArray[y][x] & 0xff00) >> 8]->rotate();
    } else {
      friendViewVec[(mapArray[y][x] & 0xff00) >> 8]->rotate();
    }
    break;
  case AniRoleDisappear:
    //意味着死亡
    //        qDebug()<<pScript->param;
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    //        qDebug()<<x<<" "<<y;
    this->roleDie(x, y);
    break;
  case AniViewStartDialog: {
    DialogLabel *dialogLabel = new DialogLabel(this, pScript->param);
    connect(dialogLabel, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  }

  break;
  case AniViewStartDelay:
    assert(viewDelayTimerID == 0);
    viewDelayTimerID = startTimer(pScript->param.toInt());
    break;
  case AniShowBattleStart: {
    BattleLabel *battleLabel = new BattleLabel(this, QString("战斗开始"));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  } break;
  case AniShowRoundX: {
    BattleLabel *battleLabel =
        new BattleLabel(this, QString("第%1回合").arg(currentRoundNum));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  } break;
  case AniShowBattleFail: {
    BattleLabel *battleLabel = new BattleLabel(this, QString("战斗失败"));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(fail()));
  }

  break;
  case AniShowBattleWin: {
    BattleLabel *battleLabel = new BattleLabel(this, QString("胜利"));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(win()));
  } break;
  case AniShowSelfAction: {
    // cursorWidget->show();
    BattleLabel *battleLabel = new BattleLabel(this, QString("我军行动"));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(startAnimation()));
    emit changeActiveStatusTo(true);
  } break;
  case AniShowEnemyAction: {
    // cursorWidget->hide();
    BattleLabel *battleLabel = new BattleLabel(this, QString("敌军行动"));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(startAnimation()));
    for (int i = 0; i < enemyViewVec.size(); ++i) {
      enemyViewVec[i]->changeActiveStatusTo(true);
    }
  } break;
  case AniShowFriendAction: {
    BattleLabel *battleLabel = new BattleLabel(this, QString("友军行动"));
    connect(battleLabel, SIGNAL(destroyed()), this, SLOT(startAnimation()));
    for (int i = 0; i < friendViewVec.size(); ++i) {
      friendViewVec[i]->changeActiveStatusTo(true);
    }
  } break;
  case AniCursorMoveToViewPoint:
    assert(cursorWidget);
    //        qDebug()<<getSubFromQString(pScript->param,1,'#').toInt()<<" "
    //               <<getSubFromQString(pScript->param,2,'#').toInt();
    cursorWidget->moveToPointOfViewWithAnimation(
        getSubFromQString(pScript->param, 1, '#').toInt(),
        getSubFromQString(pScript->param, 2, '#').toInt());

    break;
  case AniFocusRoleBeInactive:
    assert(focusRole);
    displayState = BattleNoExtraDisplay;
    focusRole->changeActiveStatusTo(false);
    focusRole->setReactOnViewMove(true);
    scriptVec.clear();
    currentScriptIndex = 0;
    scriptVec.push_back(Script(AniStart));
    switch ((mapArray[focusRole->getMapY()][focusRole->getMapX()] & 0xff)) {
    case static_cast<int>(BattleHero):
      focusRole = NULL;
      currentActiveHeroNum--;
      // qDebug()<<"left:"<<currentActiveHeroNum;
      if (currentActiveHeroNum == 0) {
        currentRoundNum++;
        currentActiveEnemyNum = enemyViewVec.size();
        processEnemyAction();
      } else {
        displayState = BattleNoExtraDisplay;
        // scriptVec.swap(vector<Script>());
        scriptVec.clear();
        currentScriptIndex = 0;
        labelSelectedIndex = 0;
        weaponSelectedIndex = 0;
        userControlOn = true;
        cursorControlOn = true;
      }

      break;
    case static_cast<int>(BattleEnemy):
      focusRole = NULL;
      currentActiveEnemyNum--;
      if (currentActiveEnemyNum == 0) {
        currentActiveFriendNum = friendViewVec.size();
        if (currentActiveFriendNum == 0) {
          this->processSelfAction();
        } else
          this->processFriendAction();
      } else {
        processEnemyAction();
      }
      break;
    case static_cast<int>(BattleFriend):
      focusRole = NULL;
      currentActiveFriendNum--;
      if (currentActiveFriendNum == 0) {
        this->processSelfAction();
      }
      break;
    default:
      assert(0);
    }
    scriptVec.push_back(Script(AniEnd));
    this->startAnimation();
    break;

  case AniEnd:
    // this->checkIfWin();
    currentScriptIndex = 0;
    labelSelectedIndex = 0;
    userControlOn = true;
    displayState = BattleNoExtraDisplay;
    // scriptVec.swap(vector<Script>());
    scriptVec.clear();
    cursorWidget->raise();
    //            assert(selfViewVec[0]);
    //            cursorWidget->moveToPointOfView(selfViewVec[0]->getPointX()-currentPointX,selfViewVec[0]->getPointY()-currentPointY);
    cursorControlOn = true;
    this->setFocus();
    // currentActiveHeroNum=selfViewVec.size();
    // qDebug()<<(selfViewVec[0]->isVisible());
    // qDebug()<<(selfViewVec[0]->getReactOnViewMove());
    // qDebug()<<enemyViewVec[0]->getPointX()<<"
    // "<<enemyViewVec[0]->getPointY();
    break;

  default:
    qDebug() << pScript->op;
    assert(0);
  }
  this->update();
}
void BattleScene::onAIMoved(int pointX, int pointY) {
  currentPointX = (pointX - (WINDOWWIDTH / BASEWIDTH / 2) * BASEWIDTH);
  currentPointY = (pointY - (WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT);
  if (currentPointX < 0)
    currentPointX = 0;
  if (currentPointX > mapSizePointX - WINDOWWIDTH)
    currentPointX = mapSizePointX - WINDOWWIDTH;
  if (currentPointY < 0)
    currentPointY = 0;
  if (currentPointY > mapSizePointY - WINDOWHEIGHT)
    currentPointY = mapSizePointY - WINDOWHEIGHT;
  cursorWidget->moveToPointOfView(pointX - currentPointX,
                                  pointY - currentPointY);
  emit viewHasMovedToPoint(currentPointX, currentPointY);
  this->update();
}

void BattleScene::
    move() { //用cursorControlOn来判定是光标的移动事件还是focusRole的移动事件
  // qDebug()<<"move";
  if (cursorControlOn) {
    int cursorPointX = cursorWidget->getViewPointX();
    int cursorPointY = cursorWidget->getViewPointY();

    if ((currentPointX % BASEWIDTH == 0 && currentPointY % BASEHEIGHT == 0 &&
         viewIsMoving) ||
        (cursorPointX % BASEWIDTH == 0 && cursorPointY % BASEHEIGHT == 0 &&
         !viewIsMoving)) {
      if (focusDir != focusDirCache) {
        focusDir = focusDirCache;
      }
      if (!upKeyPressed && !downKeyPressed && !leftKeyPressed &&
          !rightKeyPressed) {
        assert(viewMoveTimerID);
        killTimer(viewMoveTimerID);
        viewMoveTimerID = 0;
        return;
      }
      if (displayState ==
          BattleShowSelfRange) //如果是射程检测是否在rangeVector内
      {
        if (!nextStepIsInRange((cursorPointX + currentPointX) / BASEWIDTH,
                               (cursorPointY + currentPointY) / BASEHEIGHT,
                               focusDir))
          return;
      }
      if (cursorPointX ==
          (static_cast<int>(WINDOWWIDTH / BASEWIDTH / 2) * BASEWIDTH)) {
        if (cursorPointY ==
            (static_cast<int>(WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT)) {
          if ((currentPointX <= 0 && focusDir == LEFT) ||
              (currentPointX >= mapSizePointX - WINDOWWIDTH &&
               focusDir == RIGHT) ||
              (currentPointY <= 0 && focusDir == UP) ||
              (currentPointY >= mapSizePointY - WINDOWHEIGHT &&
               focusDir == DOWN)) {
            viewIsMoving = false;
          } else if (!viewIsMoving)
            viewIsMoving = true;
        } else {
          if ((focusDir == LEFT && currentPointX > 0) ||
              (focusDir == RIGHT &&
               currentPointX < mapSizePointX - WINDOWWIDTH))
            viewIsMoving = true;
          else
            viewIsMoving = false;
        }
      } else if (cursorPointY ==
                 (static_cast<int>(WINDOWHEIGHT / BASEHEIGHT / 2) *
                  BASEHEIGHT)) {
        if ((focusDir == UP && currentPointY > 0) ||
            (focusDir == DOWN && currentPointY < mapSizePointY - WINDOWHEIGHT))
          viewIsMoving = true;
        else
          viewIsMoving = false;
      } else
        viewIsMoving = false;

      if (viewIsMoving)
        viewMove(focusDir, 0, 0);
      else
        cursorMove(focusDir);
    } else if (viewIsMoving)
      viewMove(focusDir, 0, 0);
    else
      cursorMove(focusDir);
  } else {
    int focusRolePointX = focusRole->getPointX();
    int focusRolePointY = focusRole->getPointY();

    // qDebug()<<focusRolePointX<<" "<<focusRolePointY;

    if ((currentPointX % BASEWIDTH == 0 && currentPointY % BASEHEIGHT == 0 &&
         viewIsMoving) ||
        (focusRolePointX % BASEWIDTH == 0 &&
         focusRolePointY % BASEHEIGHT == 0 && !viewIsMoving)) {
      if (focusDir != focusDirCache) {
        // qDebug()<<focusRolePointX<<"
        // "<<focusRolePointY<<"dircache:"<<focusDirCache<<"dir"<<focusDir;
        // swap(focusDir,focusDirCache);
        focusDir = focusDirCache;
        focusRole->changeDir(focusDir);
      }
      //交换dic和dircache
      // viewmoving状态和rolemoving状态一定是在完整的block中进行的

      if (!upKeyPressed && !downKeyPressed && !leftKeyPressed &&
          !rightKeyPressed) {
        // qDebug()<<focusRolePointX<<" "<<focusRolePointY;
        assert(viewMoveTimerID);
        killTimer(viewMoveTimerID);
        viewMoveTimerID = 0;
        focusRole->stopAnimation();
        return;
      }
      if (!nextStepIsInRange(focusRolePointX / BASEWIDTH,
                             focusRolePointY / BASEHEIGHT, focusDir))
        return;
      if (focusRolePointX - currentPointX ==
          (static_cast<int>(WINDOWWIDTH / BASEWIDTH / 2) * BASEWIDTH)) {
        if (focusRolePointY - currentPointY ==
            (static_cast<int>(WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT)) {
          if ((currentPointX <= 0 && focusDir == LEFT) ||
              (currentPointX >= mapSizePointX - WINDOWWIDTH &&
               focusDir == RIGHT) ||
              (currentPointY <= 0 && focusDir == UP) ||
              (currentPointY >= mapSizePointY - WINDOWHEIGHT &&
               focusDir == DOWN)) {
            viewIsMoving = false;
          } else if (!viewIsMoving)
            viewIsMoving = true;
        } else {
          if ((focusDir == LEFT && currentPointX > 0) ||
              (focusDir == RIGHT &&
               currentPointX < mapSizePointX - WINDOWWIDTH))
            viewIsMoving = true;
          else
            viewIsMoving = false;
        }
      } else if (focusRolePointY - currentPointY ==
                 (static_cast<int>(WINDOWHEIGHT / BASEHEIGHT / 2) *
                  BASEHEIGHT)) {
        if ((focusDir == UP && currentPointY > 0) ||
            (focusDir == DOWN && currentPointY < mapSizePointY - WINDOWHEIGHT))
          viewIsMoving = true;
        else
          viewIsMoving = false;
      } else
        viewIsMoving = false;
      // qDebug()<<viewIsMoving;
      //*************判定接下来是主角移动还是视图移动
      // qDebug()<<"viewIsmoving:"<<viewIsMoving;
      if (viewIsMoving)
        viewMove(focusDir, focusRolePointX, focusRolePointY);
      else
        focusRoleMove(focusDir, focusRolePointX, focusRolePointY);
    } else if (viewIsMoving)
      viewMove(focusDir, focusRolePointX, focusRolePointY);
    else
      focusRoleMove(focusDir, focusRolePointX, focusRolePointY);
  }
}
bool BattleScene::nextStepIsInRange(int x, int y, DIRECTION dir) {
  // qDebug()<<x<<" "<<y;
  //检查一个当前点xy的按照dir移动的下一个点在不在rangevector里,并且如果是己方角色允许通行但是不允许停止,若在返回true
  //遍历太麻烦,有没有其他好办法,例如使得rangevector中元素严格有序
  switch (dir) {
  case UP:
    if (y > 0)
      y -= 1;
    else
      return false;
    break;
  case DOWN:
    if (y < mapSizeY - 1)
      y += 1;
    else
      return false;
    break;
  case LEFT:
    if (x > 0)
      x -= 1;
    else
      return false;
    break;
  case RIGHT:
    if (x < mapSizeX - 1)
      x += 1;
    else
      return false;
    break;
  default:
    assert(0);
  }
  bool find = false;
  for (vector<QPoint>::iterator iter = rangeVec.begin(); iter != rangeVec.end();
       ++iter) {
    if (*iter == QPoint(x, y)) {
      find = true;
      break;
    }
  }
  if (!find)
    return false;
  //检查完在不在walkRangeVector内

  if (mapArray[y][x] == 0 ||
      (mapArray[y][x] & 0xff) == static_cast<int>(BattleHero) ||
      (mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend))
    return true;

  return false;
}
void BattleScene::viewMove(DIRECTION dir, int focusRolePointX,
                           int focusRolePointY) {
  //    qDebug()<<(focusRole->getReactOnViewMove());
  switch (dir) { // default语句指向的是动画中的view自动移动
  case UP:
    if (currentPointY > 0)
      currentPointY -= MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()) &&
        userControlOn) //非userControlOn的时候证明是AI移动引起的,所以不需要设置
      focusRole->setPointXY(focusRolePointX, focusRolePointY - MAPMOVEPIXEL);
    break;
  case DOWN:
    if (currentPointY < mapSizePointY - WINDOWHEIGHT)
      currentPointY += MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()) && userControlOn)
      focusRole->setPointXY(focusRolePointX, focusRolePointY + MAPMOVEPIXEL);
    break;
  case LEFT:
    if (currentPointX > 0)
      currentPointX -= MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()) && userControlOn)
      focusRole->setPointXY(focusRolePointX - MAPMOVEPIXEL, focusRolePointY);
    break;
  case RIGHT:
    if (currentPointX < mapSizePointX - WINDOWWIDTH)
      currentPointX += MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()) && userControlOn)
      focusRole->setPointXY(focusRolePointX + MAPMOVEPIXEL, focusRolePointY);
    break;
  default:
    assert(viewAutoMoveEndX >= 0 && viewAutoMoveEndY >= 0);
    // assert(!cursorControlOn);
    int dis = sqrt(static_cast<double>(
        (currentPointX - viewAutoMoveEndX * BASEWIDTH) *
            (currentPointX - viewAutoMoveEndX * BASEWIDTH) +
        (currentPointY - viewAutoMoveEndY * BASEHEIGHT) *
            (currentPointY - viewAutoMoveEndY * BASEHEIGHT)));
    if (dis < ANIMATIONMOVEPIXEL) {
      assert(viewAnimationMoveTimerID);
      killTimer(viewAnimationMoveTimerID);
      viewAnimationMoveTimerID = 0;
      currentPointX = viewAutoMoveEndX * BASEWIDTH;
      currentPointY = viewAutoMoveEndY * BASEHEIGHT;
      this->startAnimation();
      //            if(focusRole)
      //            {
      //                focusRole->onViewMovedTo(currentPointX,currentPointY);
      //                focusRole->setReactOnViewMove(true);
      //            }
    } else {
      // qDebug()<<currentPointX<<" "<<currentPointY;
      currentPointX += (currentPointX > viewAutoMoveEndX * BASEWIDTH ? -1 : 1) *
                       abs(currentPointX - viewAutoMoveEndX * BASEWIDTH) *
                       ANIMATIONMOVEPIXEL / dis;
      currentPointY +=
          (currentPointY > viewAutoMoveEndY * BASEHEIGHT ? -1 : 1) *
          abs(currentPointY - viewAutoMoveEndY * BASEHEIGHT) *
          ANIMATIONMOVEPIXEL / dis;
      // qDebug()<<currentPointX<<" "<<currentPointY;
    }
    break;
  }
  this->update();
  emit viewHasMovedToPoint(currentPointX, currentPointY);
}
void BattleScene::focusRoleMove(DIRECTION dir, int focusRolePointX,
                                int focusRolePointY) {
  //    focusRole->raise();
  //    cursorWidget->raise();
  switch (dir) {
  case UP:
    // if(focusRolePointY!=0)
    cursorWidget->moveToPointOfView(focusRolePointX - currentPointX,
                                    focusRolePointY - MAPMOVEPIXEL -
                                        currentPointY);
    focusRole->moveToPointOfMap(focusRolePointX,
                                focusRolePointY - MAPMOVEPIXEL);
    break;
  case DOWN:
    // if(focusRolePointY!=WINDOWHEIGHT-BASEHEIGHT)
    cursorWidget->moveToPointOfView(focusRolePointX - currentPointX,
                                    focusRolePointY + MAPMOVEPIXEL -
                                        currentPointY);
    focusRole->moveToPointOfMap(focusRolePointX,
                                focusRolePointY + MAPMOVEPIXEL);
    break;
  case LEFT:
    // if(focusRolePointX!=0)
    cursorWidget->moveToPointOfView(focusRolePointX - MAPMOVEPIXEL -
                                        currentPointX,
                                    focusRolePointY - currentPointY);
    focusRole->moveToPointOfMap(focusRolePointX - MAPMOVEPIXEL,
                                focusRolePointY);
    break;
  case RIGHT:
    // if(focusRolePointX!=WINDOWWIDTH-BASEWIDTH)
    // qDebug()<<focusRolePointX<<" "<<focusRolePointY;
    cursorWidget->moveToPointOfView(focusRolePointX + MAPMOVEPIXEL -
                                        currentPointX,
                                    focusRolePointY - currentPointY);
    focusRole->moveToPointOfMap(focusRolePointX + MAPMOVEPIXEL,
                                focusRolePointY);
    break;
  default:
    break;
  }
  this->update();
}
void BattleScene::cursorMove(DIRECTION dir) {
  int x = cursorWidget->getViewPointX();
  int y = cursorWidget->getViewPointY();
  switch (dir) {
  case UP:
    if (y > 0)
      cursorWidget->moveToPointOfView(x, y - MAPMOVEPIXEL);
    break;
  case DOWN:
    if (y < mapSizePointY - BASEHEIGHT)
      cursorWidget->moveToPointOfView(x, y + MAPMOVEPIXEL);
    break;
  case LEFT:
    if (x > 0)
      cursorWidget->moveToPointOfView(x - MAPMOVEPIXEL, y);
    break;
  case RIGHT:
    if (y < mapSizePointX - BASEWIDTH)
      cursorWidget->moveToPointOfView(x + MAPMOVEPIXEL, y);
    break;
  default:
    break;
  }
}
void BattleScene::addRangeIntoVector() {
  assert(focusRole);
  assert(displayState == BattleShowSelfRange ||
         displayState == BattleShowEnemyRange ||
         displayState == BattleShowFriendRange ||
         displayState == BattleShowSelfWalkRange ||
         displayState == BattleShowEnemyWalkRange ||
         displayState == BattleShowFriendWalkRange ||
         displayState == BattleShowSelfMedicineRange);
  // rangeVec.swap(vector<QPoint>());
  rangeVec.clear();
  int x = focusRole->getMapX();
  int y = focusRole->getMapY();

  if (displayState == BattleShowSelfRange ||
      displayState == BattleShowEnemyRange ||
      displayState == BattleShowFriendRange) {
    int range = focusRole->getRolePointer()->range;
    assert(range > 0);
    getRangeVectorFromRange(range, rangeVec, x, y, mapSizeX, mapSizeY);
    rangeVec.push_back(QPoint(x, y));
    //        int offsetX=0;
    //        int offsetY=0;

    //            for(int currentRange=range;currentRange>-1;--currentRange)
    //            {
    //                for(int i=-currentRange;i<currentRange+1;++i)
    //                {
    //                    offsetX=i;
    //                    offsetY=currentRange-abs(i);

    //                    if(x+offsetX<0||
    //                            x+offsetX>mapSizeX-1||
    //                            y+offsetY<0||
    //                            y+offsetY>mapSizeY-1)continue;
    //                        rangeVec.push_back(QPoint(x+offsetX,y+offsetY));
    //                    if(offsetY==0)continue;
    //                    offsetY=-offsetY;
    //                    if(x+offsetX<0||
    //                            x+offsetX>mapSizeX-1||
    //                            y+offsetY<0||
    //                            y+offsetY>mapSizeY-1)continue;
    //                        rangeVec.push_back(QPoint(x+offsetX,y+offsetY));
    //                }
    //            }
    // int **rangeArray=getRangePointerFromRangeType(range&0xff);
    //        int rangeVecSize=(range&0xff00)>>8;
    //        for(int i=0;i<rangeVecSize;++i)
    //        {
    //            if(x+rangeArray[i][0]<0||
    //                    x+rangeArray[i][0]>mapSizeX-1||
    //                    y+rangeArray[i][1]<0||
    //                    y+rangeArray[i][1]>mapSizeY-1)continue;
    //            rangeVec.push_back(QPoint(x+rangeArray[i][0],y+rangeArray[i][1]));
    //        }

  } else if (displayState == BattleShowSelfWalkRange ||
             displayState == BattleShowEnemyWalkRange ||
             displayState == BattleShowFriendWalkRange) {
    int walkRange = focusRole->getRolePointer()->speed;
    assert(walkRange > 0);
    int offsetX = 0;
    int offsetY = 0;

    for (int currentRange = walkRange; currentRange > -1; --currentRange) {
      for (int i = -currentRange; i < currentRange + 1; ++i) {
        offsetX = i;
        offsetY = currentRange - abs(i);

        if (!(x + offsetX < 0 || x + offsetX > mapSizeX - 1 ||
              y + offsetY < 0 || y + offsetY > mapSizeY - 1)) {
          rangeVec.push_back(QPoint(x + offsetX, y + offsetY));
        }
        if (offsetY == 0)
          continue;
        offsetY = -offsetY;
        if (!(x + offsetX < 0 || x + offsetX > mapSizeX - 1 ||
              y + offsetY < 0 || y + offsetY > mapSizeY - 1)) {
          rangeVec.push_back(QPoint(x + offsetX, y + offsetY));
        }
      }
    }
  } else if (displayState == BattleShowSelfMedicineRange) {
    getRangeVectorFromRange(1, rangeVec, x, y, mapSizeX, mapSizeY);
    rangeVec.push_back(QPoint(x, y));
  }

  //    for(int i=0;i<rangeVec.size();++i)
  //    {
  //        qDebug()<<rangeVec[i].x()<<" "<<rangeVec[i].y();
  //    }
}
IfBattleEnd BattleScene::checkBattleEnd() {
  // qDebug()<<winRequirement<<" "<<loseRequirement;
  if (selfViewVec.empty())
    return BattleFail;
  if (enemyViewVec.empty())
    return BattleWin;
  switch (winRequirement & 0xff) {
  case 0: //敌方全灭
    if (enemyViewVec.size() == 0) {
      return BattleWin;
    }
    break;
  case 0x01: //某敌人死亡
  {
    int deadID = (winRequirement & 0xff00) >> 8;

    for (int i = 0; i < enemyViewVec.size(); ++i) {
      if (enemyViewVec[i]->getID() == deadID)
        break;                          //没死
      if (i == enemyViewVec.size() - 1) //没找到,证明死了
      {
        return BattleWin;
      }
    }

  } break;
  case 0x02: //我方角色赶到某点处
  {
    uint id = (winRequirement & 0xff000000) >> 24;
    uint x = (winRequirement & 0xff0000) >> 16;
    uint y = (winRequirement & 0xff00) >> 8;
    if ((mapArray[y][x] & 0xff) == static_cast<int>(BattleHero)) {
      if (selfViewVec[(mapArray[y][x] & 0xff00) >> 8]->getID() - 1000 == id) {
        return BattleWin;
      }
    }
  } break;
  default:
    assert(0);
  }

  int count = 0;
  //**********VS编译器不支持跨容器遍历,只好先建立一个临时容器
  //    vector<RoleView*>temp(selfViewVec);
  //    for(int i=0;i<friendViewVec.size();++i)
  //    {
  //        temp.push_back(friendViewVec[i]);
  //    }
  //*************照顾VS编译器

  int id = 0;
  for (int i = 0; i < 4; i++) {
    id = (loseRequirement >> (8 * i)) & 0xff;
    if (id == 0)
      continue;
    count++; // count统计应该存活的id个数,然后寻找

    RoleView *currentRoleView;
    for (int i = 0; i < selfViewVec.size() + friendViewVec.size();
         ++i) //进行跨容器遍历
    {
      if (i < selfViewVec.size()) {
        currentRoleView = selfViewVec[i];
      } else {
        currentRoleView = friendViewVec[i - selfViewVec.size()];
      }

      if (currentRoleView->getID() == id + 1000 ||
          currentRoleView->getID() == id + 3000) {
        count--;
        break;
      }
    }
  }

  if (count)
    return BattleFail; //证明有人应该存在的却不在,所以是死掉了,失败条件达成

  return BattleNotEnd;
}
void BattleScene::win() {
  this->close();
  emit battleWin();
}
void BattleScene::fail() {
  emit battleFail();
  //    for(int i=0;i<selfViewVec.size();++i)
  //    {
  //        selfViewVec[i]->close();
  //        selfViewVec[i]=NULL;
  //    }
  //    for(int i=0;i<enemyViewVec.size();++i)
  //    {
  //        enemyViewVec[i]->close();
  //        enemyViewVec[i]=NULL;
  //    }
  //    for(int i=0;i<friendViewVec.size();++i)
  //    {
  //        friendViewVec[i]->close();
  //        friendViewVec[i]=NULL;
  //    }
  //    weaponWidget->close();
  //    operationWidget->close();
  //    cursorWidget->close();
  //    descriptionWidget->close();

  this->close();
  // this->deleteLater();
}
void BattleScene::roleRecover() {
  assert(focusRole);
  Role *pRole = focusRole->getRolePointer();
  int recoverHP = (pRole->HPMax) * 0.1 + getRandomNumberIn(-2, 2);

  scriptVec.push_back(
      Script(AniRoleRecover, QString("%1#%2#%3#%4#%5#")
                                 .arg(focusRole->getMapX())
                                 .arg(focusRole->getMapY())
                                 .arg(focusRole->getRolePointer()->HP)
                                 .arg(recoverHP)
                                 .arg(focusRole->getRolePointer()->HPMax)));

  if (pRole->HP + recoverHP > pRole->HPMax)
    pRole->HP = pRole->HPMax;
  else
    pRole->HP += recoverHP;

  scriptVec.push_back(Script(AniFocusRoleBeInactive));
}
void BattleScene::roleAttack(
    int attackerX,
    int attackerY) //此函数只负责向scriptVec中添加攻击和闪避/伤害以及失去活动状态的script,而不负责添加anistart和aniend
{ //当AI攻击时,这个函数调用发生在攻击者移动到指定位置之前,所以它无法预知攻击者发起攻击时的位置,需要当做参数传递进来
  assert(focusRole);
  assert(targetRole);
  Role *attacker = focusRole->getRolePointer();
  Role *defencer = targetRole->getRolePointer();
  //    scriptVec.push_back(Script(AniStart));

  scriptVec.push_back(
      Script(AniRoleAttack, QString("%1#%2#").arg(attackerX).arg(attackerY)));
  //首先根据攻方命中和防御方闪避计算是否命中
  double coeff = 1.0;
  if ((attacker->level) >= (defencer->level))
    coeff = log(static_cast<double>(NATURALLOGE + (attacker->level) -
                                    (defencer->level)));
  else
    coeff = 1.0 / (log(static_cast<double>(NATURALLOGE - (attacker->level) +
                                           (defencer->level))));
  int probability = ((attacker->accurcy) - (defencer->dodge)) * coeff;
  if (getRandomOfPercent(probability)) //命中了
  {
    double critCoeff = 1;   //暴击系数
    if (attacker->exp >= 0) //只有攻击方是我方角色才计算暴击系数
    {
      if (getRandomOfPercent(attacker->crit)) { //暴击2倍加成伤害
        critCoeff = 2.0;
      }
    }
    int damage = ((attacker->physicalAttack) - (defencer->physicalDefence)) *
                     coeff *
                     jobCoefficient[(static_cast<int>(defencer->job)) / 3]
                                   [(static_cast<int>(attacker->job)) / 3] +
                 getRandomNumberIn(-5, 5);
    //+100;//+100是为了调试方便
    damage *= critCoeff;
    if (damage < 1)
      damage = 1;
    //        qDebug()<<"coeff"<<coeff;
    //        qDebug()<<"procoeff"<<(jobCoefficient[(static_cast<int>(defencer->pro))/3][(static_cast<int>(attacker->pro))/3]);
    //        qDebug()<<"random"<<getRandomNumberIn(-5,5);
    //        qDebug()<<"damage"<<damage;
    // damage=100;
    if ((defencer->HP) - damage <= 0) //死了
    {
      scriptVec.push_back(Script(AniRoleHurt, QString("%1#%2#%3#%4#%5#")
                                                  .arg(targetRole->getMapX())
                                                  .arg(targetRole->getMapY())
                                                  .arg(defencer->HP)
                                                  .arg(damage)
                                                  .arg(defencer->HPMax)));
      scriptVec.push_back(Script(AniRoleDie, QString("%1#%2#")
                                                 .arg(targetRole->getMapX())
                                                 .arg(targetRole->getMapY())));
      scriptVec.push_back(
          Script(AniRoleDisappear, QString("%1#%2#")
                                       .arg(targetRole->getMapX())
                                       .arg(targetRole->getMapY())));
      defencer->HP = 1;
    } else {

      scriptVec.push_back(Script(AniRoleHurt, QString("%1#%2#%3#%4#%5#")
                                                  .arg(targetRole->getMapX())
                                                  .arg(targetRole->getMapY())
                                                  .arg(defencer->HP)
                                                  .arg(damage)
                                                  .arg(defencer->HPMax)));
      (defencer->HP) -= damage;
    }
    if (attacker->exp >= 0) { //为我方角色加经验
      attacker->exp += damage;
      if (attacker->exp >= attacker->expMax) { //达到了升级条件
        if (attacker->level == CHANGEJOBLEVEL1 ||
            attacker->level == CHANGEJOBLEVEL2) {
          scriptVec.push_back(
              Script(AniRoleChangeJob, QString("%1#%2#")
                                           .arg(focusRole->getMapX())
                                           .arg(focusRole->getMapY())));
        } else {
          scriptVec.push_back(
              Script(AniRoleLevelUp, QString("%1#%2#")
                                         .arg(focusRole->getMapX())
                                         .arg(focusRole->getMapY())));
        }
        attacker->level += 1;
        attacker->exp -= (attacker->expMax);
        attacker->expMax +=
            (2 + getRandomNumberIn(0, 3)); //每升一级经验上限增一点
        assert(attacker->exp < attacker->expMax);
      }
    }

    bool weaponEffectOn =
        getRandomOfPercent(((attacker->IQ) > (defencer->IQ)) ? 80 : 50);
    //根据双方iq计算武器特效是否命中,公式有待讨论,当前的公式是,若攻方智力高则命中率80%否则50%

    int articleSpecial = 0;
    if (focusRole->getType() == static_cast<int>(BattleHero))
      articleSpecial = (*pArticleIDToInfoMap)[focusRole->getRolePointer()
                                                  ->weapon[weaponSelectedIndex]]
                           .special;
    else {
      articleSpecial =
          (*pArticleIDToInfoMap)[focusRole->getRolePointer()->weapon[0]]
              .special;
    }
    if (weaponEffectOn && (scriptVec.end() - 1)->op != AniRoleDisappear &&
        (scriptVec.end() - 2)->op !=
            AniRoleDisappear) { //如果敌人死了就不需要加特殊效果动画了
      switch (articleSpecial) {
      case WEAPONDISABLE_1:
      case WEAPONDISABLE_2:
      case WEAPONDISABLE_3:
      case WEAPONDISABLE_4:
      case WEAPONDISABLE_5:
      case WEAPONDISABLE_6:
      case WEAPONDISABLE_7:
      case WEAPONDISABLE_8:
      case WEAPONDISABLE_9:
      case WEAPONDISABLE_10: {
        int x = targetRole->getMapX();
        int y = targetRole->getMapY();
        mapArray[y][x] = mapArray[y][x] | (articleSpecial << 16);
        scriptVec.push_back(
            Script(AniRoleDisable, QString("%1#%2#").arg(x).arg(y)));
        //右数第三个字节的低4位现在用于存储当前角色的状态,0为正常,1~10为休息1~10个回合
        //其他效果比如中毒,防御下降
      } break;
      case WEAPONPOISON_5:
      case WEAPONPOISON_10:
      case WEAPONPOISON_15:
      case WEAPONPOISON_20:
      case WEAPONPOISON_25:
      case WEAPONPOISON_30:
      case WEAPONPOISON_35:
      case WEAPONPOISON_40:
      case WEAPONPOISON_45:
      case WEAPONPOISON_50: {
        int x = targetRole->getMapX();
        int y = targetRole->getMapY();
        mapArray[y][x] =
            mapArray[y][x] | ((articleSpecial - WEAPONPOISON_5 - 1) << 20);
        scriptVec.push_back(
            Script(AniRolePoison, QString("%1#%2#").arg(x).arg(y)));
      } break;
      case WEAPONATTACKDOWN_1:
      case WEAPONATTACKDOWN_2:
      case WEAPONATTACKDOWN_3:
      case WEAPONATTACKDOWN_4:
      case WEAPONATTACKDOWN_5:
      case WEAPONATTACKDOWN_6:
      case WEAPONATTACKDOWN_7:
      case WEAPONATTACKDOWN_8:
      case WEAPONATTACKDOWN_9:
      case WEAPONATTACKDOWN_10:
        targetRole->getRolePointer()->physicalAttack *=
            (1.0 - (articleSpecial - WEAPONATTACKDOWN_1 - 1) / 10);
        scriptVec.push_back(
            Script(AniRoleAttackDown, QString("%1#%2#")
                                          .arg(targetRole->getMapX())
                                          .arg(targetRole->getMapY())));
        break;
      case WEAPONCRITDOWN_1:
      case WEAPONCRITDOWN_2:
      case WEAPONCRITDOWN_3:
      case WEAPONCRITDOWN_4:
      case WEAPONCRITDOWN_5:
      case WEAPONCRITDOWN_6:
      case WEAPONCRITDOWN_7:
      case WEAPONCRITDOWN_8:
      case WEAPONCRITDOWN_9:
      case WEAPONCRITDOWN_10:
        targetRole->getRolePointer()->crit *=
            (1.0 - (articleSpecial - WEAPONCRITDOWN_1 - 1) / 10);
        scriptVec.push_back(
            Script(AniRoleCritDown, QString("%1#%2#")
                                        .arg(targetRole->getMapX())
                                        .arg(targetRole->getMapY())));
        break;
      case WEAPONDEFENCEDOWN_1:
      case WEAPONDEFENCEDOWN_2:
      case WEAPONDEFENCEDOWN_3:
      case WEAPONDEFENCEDOWN_4:
      case WEAPONDEFENCEDOWN_5:
      case WEAPONDEFENCEDOWN_6:
      case WEAPONDEFENCEDOWN_7:
      case WEAPONDEFENCEDOWN_8:
      case WEAPONDEFENCEDOWN_9:
      case WEAPONDEFENCEDOWN_10:
        targetRole->getRolePointer()->physicalDefence *=
            (1.0 - (articleSpecial - WEAPONDEFENCEDOWN_1 - 1) / 10);
        scriptVec.push_back(
            Script(AniRoleDefenceDown, QString("%1#%2#")
                                           .arg(targetRole->getMapX())
                                           .arg(targetRole->getMapY())));
        break;
      case WEAPONDODGEDOWN_1:
      case WEAPONDODGEDOWN_2:
      case WEAPONDODGEDOWN_3:
      case WEAPONDODGEDOWN_4:
      case WEAPONDODGEDOWN_5:
      case WEAPONDODGEDOWN_6:
      case WEAPONDODGEDOWN_7:
      case WEAPONDODGEDOWN_8:
      case WEAPONDODGEDOWN_9:
      case WEAPONDODGEDOWN_10:
        targetRole->getRolePointer()->dodge *=
            (1.0 - (articleSpecial - WEAPONDODGEDOWN_1 - 1) / 10);
        scriptVec.push_back(
            Script(AniRoleDodgeDown, QString("%1#%2#")
                                         .arg(targetRole->getMapX())
                                         .arg(targetRole->getMapY())));
        break;
      case WEAPONACCURCYDOWN_1:
      case WEAPONACCURCYDOWN_2:
      case WEAPONACCURCYDOWN_3:
      case WEAPONACCURCYDOWN_4:
      case WEAPONACCURCYDOWN_5:
      case WEAPONACCURCYDOWN_6:
      case WEAPONACCURCYDOWN_7:
      case WEAPONACCURCYDOWN_8:
      case WEAPONACCURCYDOWN_9:
      case WEAPONACCURCYDOWN_10:
        targetRole->getRolePointer()->accurcy *=
            (1.0 - (articleSpecial - WEAPONACCURCYDOWN_1 - 1) / 10);
        scriptVec.push_back(
            Script(AniRoleAccurcyDown, QString("%1#%2#")
                                           .arg(targetRole->getMapX())
                                           .arg(targetRole->getMapY())));
        break;
      case WEAPONSPEEDDOWN_1:
      case WEAPONSPEEDDOWN_2:
      case WEAPONSPEEDDOWN_3:
      case WEAPONSPEEDDOWN_4:
      case WEAPONSPEEDDOWN_5:
      case WEAPONSPEEDDOWN_6:
      case WEAPONSPEEDDOWN_7:
      case WEAPONSPEEDDOWN_8:
      case WEAPONSPEEDDOWN_9:
      case WEAPONSPEEDDOWN_10:
        targetRole->getRolePointer()->speed -=
            (articleSpecial - WEAPONACCURCYDOWN_1 - 1);
        scriptVec.push_back(
            Script(AniRoleAccurcyDown, QString("%1#%2#")
                                           .arg(targetRole->getMapX())
                                           .arg(targetRole->getMapY())));
        break;
      case WEAPONAOE_1:
      case WEAPONAOE_2:
      case WEAPONAOE_3:
      case WEAPONAOE_4:
      case WEAPONAOE_5:
      case WEAPONAOE_6:
      case WEAPONAOE_7:
      case WEAPONAOE_8:
      case WEAPONAOE_9:
      case WEAPONAOE_10:
        break;

      default:
        break;
      }
    }

  } else // miss
  {
    scriptVec.push_back(Script(AniRoleDodge, QString("%1#%2#")
                                                 .arg(targetRole->getMapX())
                                                 .arg(targetRole->getMapY())));
  }
  //    scriptVec.swap(vector<Script>());
  //    processRoleAttack(scriptVec,focusRole,targetRole);
  scriptVec.push_back(Script(AniFocusRoleBeInactive));
  // scriptVec.push_back(Script(AniEnd,QString()));
  //    for(int i=0;i<scriptVec.size();++i)
  //    {
  //        qDebug()<<scriptVec[i].op;
  //        qDebug()<<scriptVec[i].param;
  //    }
  //    this->startAnimation();
}
void BattleScene::processEnemyAction() {
  assert(currentActiveEnemyNum);
  //每次处理一个enemy的行动
  // qDebug()<<currentActiveEnemyNum;
  if (currentActiveEnemyNum == enemyViewVec.size()) {
    scriptVec.push_back(Script(AniShowEnemyAction));
  }
  // assert(scriptVec.empty());
  //    scriptVec.push_back(Script(AniStart));
  focusRole = enemyViewVec[enemyViewVec.size() - currentActiveEnemyNum];
  int focusRoleX = focusRole->getMapX();
  int focusRoleY = focusRole->getMapY();
  assert(focusRole);

  //判断当前敌人是否中毒或者失去行动能力
  int disableFlag = (mapArray[focusRoleY][focusRoleX] & 0x0f0000) >> 16;
  int poisonFlag = (mapArray[focusRoleY][focusRoleX] & 0xf00000) >> 20;

  if (poisonFlag != 0) {
    this->cursorMoveToFocusRole();
    assert(poisonFlag < WEAPONPOISON_50 + 1 && poisonFlag > WEAPONPOISON_5 - 1);
    Role *role = focusRole->getRolePointer();
    int damage = 0.05 * (role->HPMax) * (poisonFlag - WEAPONPOISON_5 + 1);
    if (role->HP <= damage) {
      damage = role->HP - 1;
    }
    scriptVec.push_back(Script(
        AniRolePoison, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
    scriptVec.push_back(Script(AniRoleHurt, QString("%1#%2#%3#%4#%5#")
                                                .arg(focusRoleX)
                                                .arg(focusRoleY)
                                                .arg(role->HP)
                                                .arg(damage)
                                                .arg(role->HPMax)));
    role->HP -= damage;
  }
  if (disableFlag != 0) {
    assert(disableFlag > WEAPONDISABLE_1 - 1 &&
           disableFlag < WEAPONDISABLE_10 + 1);
    this->cursorMoveToFocusRole();

    disableFlag--;

    mapArray[focusRoleY][focusRoleX] &= 0xfff0ffff; //右三字节低4位清零
    mapArray[focusRoleY][focusRoleX] |= (disableFlag << 16);
    //过去了一个回合,因此flag-1后重新放回标志位
    scriptVec.push_back(Script(
        AniRoleDisable, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
    scriptVec.push_back(Script(AniFocusRoleBeInactive));
    // scriptVec.push_back(AniEnd);
    // this->startAnimation();
    return;
  }

  //判断敌人是攻击系还是回复系
  int recoverHP = 25 * (focusRole->getRolePointer()->weapon[0] -
                        (ENEMYWEAPONRECOVER_25 - 1));
  if (recoverHP > 0 &&
      recoverHP <
          251) { //证明是回复系,回复系按照从前到后的顺序检查有没有受伤的enemy角色
    //优先顺序:如果靠前的敌人能够进行回复则直接回复
    //如果没有能回复的角色就正常攻击
    for (int i = 0; i < enemyViewVec.size(); ++i) {
      targetRole = enemyViewVec[i];
      Role *currentRole = enemyViewVec[i]->getRolePointer();
      if ((currentRole->HP) < (currentRole->HPMax)) {
        if (focusRole == targetRole) { //如果自己补自己的话
          this->cursorMoveToFocusRole();
          scriptVec.push_back(
              Script(AniRoleRotate,
                     QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
          this->roleCure(focusRoleX, focusRoleY, recoverHP);
          // this->startAnimation();
          return;
        }
        vector<QPoint> path;
        AStarPathFinder(mapArray[0], mapSizeX, mapSizeY, targetRole->getMapX(),
                        targetRole->getMapY(), focusRole->getMapX(),
                        focusRole->getMapY(), path);
        //寻路算法,得到是从enemy指向hero的路径
        if (path.empty()) {
          this->cursorMoveToFocusRole();
          this->focusRoleDonotMove();
          // scriptVec.push_back(Script(AniRoleRotate));
        }

        assert(path.size() > 1);

        int step = (focusRole->getRolePointer()->speed);
        for (vector<QPoint>::iterator iter = path.begin();
             iter != path.end() - 1; ++iter, --step) {
          if (step < 0) { //到了自己的行动极限还是打不到
            break;
          }
          // path的第一个元素是攻击方坐标最后一个元素是防御方坐标
          if (checkIfInRange(*iter, path[path.size() - 1],
                             focusRole->getRolePointer()
                                 ->range)) { //在某一步已经可以进行回复
            this->cursorMoveToFocusRole();
            scriptVec.push_back(
                Script(AniRoleRotate,
                       QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
            if (path.size() != 2) //=2的时候相邻,证明不需要移动
            {
              scriptVec.push_back(
                  Script(AniRoleFastMove,
                         QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
              iter++;
              iter->setX(-1);
              iter->setY(
                  -1); //攻击位置的下一个元素置为(-1,-1),这样,role移动到攻击位置就会停止
              focusRole->setPathVec(new vector<QPoint>(path));
              iter--;
            }

            this->roleCure(iter->x(), iter->y(), recoverHP);
            // this->startAnimation();
            return;
          }
        }
      }
    }
  }
  //是攻击系或者无法在范围内回复自己角色,则进行攻击
  targetRole = selfViewVec[0];
  int nearestDistance =
      MANHATTAN(selfViewVec[0]->getMapX(), selfViewVec[0]->getMapY(),
                focusRoleX, focusRoleY);
  int currentDistance = nearestDistance;
  for (int i = 1; i < selfViewVec.size(); ++i) {
    currentDistance =
        MANHATTAN(selfViewVec[i]->getMapX(), selfViewVec[i]->getMapY(),
                  focusRoleX, focusRoleY);
    if (nearestDistance > currentDistance) {
      nearestDistance = currentDistance;
      targetRole = selfViewVec[i];
    }
  }
  for (int i = 0; i < friendViewVec.size(); ++i) {
    currentDistance =
        MANHATTAN(friendViewVec[i]->getMapX(), friendViewVec[i]->getMapY(),
                  focusRoleX, focusRoleY) +
        focusRole->getRolePointer()->speed;
    if (nearestDistance > currentDistance) {
      nearestDistance = currentDistance;
      targetRole = friendViewVec[i];
    }
  } //优先选择selfVec中的人物,但是若friendVec中的人物距离太近(加上敌人的移动距离仍然是距离当前敌人最近的),就选择它

  //寻找hero中manhattan距离最近的角色的index
  vector<QPoint> path;
  AStarPathFinder(mapArray[0], mapSizeX, mapSizeY, targetRole->getMapX(),
                  targetRole->getMapY(), focusRoleX, focusRoleY, path);
  // qDebug()<<(targetRole->getMapX())<<(targetRole->getMapY())<<(focusRole->getMapX())<<(focusRole->getMapY());

  //寻路算法,得到是从enemy指向hero的路径

  //    qDebug()<<"path:";
  //    for(int i=0;i<path.size();++i)
  //    {
  //        qDebug()<<path[i].x()<<" "<<path[i].y();
  //    }
  //    qDebug()<<"pathend";

  this->cursorMoveToFocusRole();
  //首先将光标固定到当前角色

  if (path.empty()) { //证明它被己方角色包围了没法打到任何一个人
    this->focusRoleDonotMove();
    // this->startAnimation();
    return;
  }
  assert(path.size() > 1);
  // path的第一个元素是攻击方(AI)的坐标,最后一个元素是防御方的坐标,所以至少有两个元素
  if (path.size() == 2 && (focusRole->getRolePointer()->range == 8 ||
                           focusRole->getRolePointer()->range ==
                               9)) { //远程系在近身处无法攻击,需要特殊处理
    scriptVec.push_back(Script(
        AniRoleRotate, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));

    if (focusRoleX != 0 && mapArray[focusRoleY][focusRoleX - 1] == 0) {
      vector<QPoint> *v = new vector<QPoint>();
      v->push_back(QPoint(focusRoleX, focusRoleY));
      v->push_back(QPoint(focusRoleX - 1, focusRoleY));
      v->push_back(QPoint(-1, -1));
      focusRole->setPathVec(v);
      this->roleAttack(focusRoleX - 1, focusRoleY);
      return;
    } else if (focusRoleX != mapSizeX - 1 &&
               mapArray[focusRoleY][focusRoleX + 1] == 0) {
      vector<QPoint> *v = new vector<QPoint>();
      v->push_back(QPoint(focusRoleX, focusRoleY));
      v->push_back(QPoint(focusRoleX + 1, focusRoleY));
      v->push_back(QPoint(-1, -1));
      focusRole->setPathVec(v);
      this->roleAttack(focusRoleX + 1, focusRoleY);
      return;
    } else if (focusRoleY != mapSizeY - 1 &&
               mapArray[focusRoleY + 1][focusRoleX] == 0) {
      vector<QPoint> *v = new vector<QPoint>();
      v->push_back(QPoint(focusRoleX, focusRoleY));
      v->push_back(QPoint(focusRoleX, focusRoleY + 1));
      v->push_back(QPoint(-1, -1));
      focusRole->setPathVec(v);
      this->roleAttack(focusRoleX, focusRoleY + 1);
      return;
    } else if (focusRoleY != 0 && mapArray[focusRoleY - 1][focusRoleX] == 0) {
      vector<QPoint> *v = new vector<QPoint>();
      v->push_back(QPoint(focusRoleX, focusRoleY));
      v->push_back(QPoint(focusRoleX, focusRoleY - 1));
      v->push_back(QPoint(-1, -1));
      focusRole->setPathVec(v);
      this->roleAttack(focusRoleX, focusRoleY - 1);
      return;
    } else {
      scriptVec.push_back(AniFocusRoleBeInactive);
      return;
    }
  }

  int i = (focusRole->getRolePointer()->speed);
  for (vector<QPoint>::iterator iter = path.begin(); iter != path.end() - 1;
       ++iter, --i) {

    if (i < 0) { //到了自己的行动极限还是打不到,那就向最远的距离移动
      scriptVec.push_back(Script(
          AniRoleRotate, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
      if (battleID % 2 == 1) { //是进攻局
        vector<QPoint> *p = new vector<QPoint>(
            path.begin(),
            path.begin() + (focusRole->getRolePointer()->speed) + 1);
        //由当前的focusRole负责释放
        p->push_back(QPoint(-1, -1));
        focusRole->setPathVec(p);
        scriptVec.push_back(
            Script(AniRoleFastMove, QString("%1#%2#")
                                        .arg(focusRole->getMapX())
                                        .arg(focusRole->getMapY())));
      }
      scriptVec.push_back(Script(AniFocusRoleBeInactive));
      break;
    }
    if (checkIfInRange(*iter, path[path.size() - 1],
                       focusRole->getRolePointer()
                           ->range)) { //在某一步时候攻击方已经可以打到防御方
      scriptVec.push_back(Script(
          AniRoleRotate, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
      if (path.size() != 2) //=2的时候证明不需要移动
      {
        scriptVec.push_back(
            Script(AniRoleFastMove,
                   QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
        iter++;
        iter->setX(-1);
        iter->setY(
            -1); //攻击位置的下一个元素置为(-1,-1),这样,role移动到攻击位置就会停止
        focusRole->setPathVec(new vector<QPoint>(path));
        iter--;
      }

      this->roleAttack(iter->x(), iter->y());

      break;
    }
  }

  //    qDebug()<<"currentIndex:"<<currentScriptIndex;
  //    for(int i=0;i<scriptVec.size();++i)
  //    {
  //        qDebug()<<scriptVec[i].op<<" "<<scriptVec[i].param;
  //    }
  // this->startAnimation();

  // targetRole=NULL;

  //    scriptVec.push_back(Script(AniRoleRecover,
  //                               QString("%1#%2#")
  //                               .arg(enemyViewVec[i]->getMapX())
  //                               .arg(enemyViewVec[i]->getMapY())));

  // qDebug()<<focusRole->getID();

  //    if(!friendViewVec.empty())processFriendAction();
  //    scriptVec.push_back(Script(AniShowRoundX,QString("%1").arg(currentRoundNum)));
  //    scriptVec.push_back(Script(AniShowSelfAction));

  //    assert(selfViewVec[0]);
  //    int x=(selfViewVec[0]->getMapX()-(WINDOWWIDTH/BASEWIDTH/2));
  //    int y=(selfViewVec[0]->getMapY()-(WINDOWHEIGHT/BASEHEIGHT/2));

  //    if(x<0)x=0;
  //    else
  //    if(x>(mapSizePointX-WINDOWWIDTH)/BASEWIDTH)x=(mapSizePointX-WINDOWWIDTH)/BASEWIDTH;

  //    if(y<0)y=0;
  //    else
  //    if(y>(mapSizePointY-WINDOWHEIGHT)/BASEHEIGHT)y=(mapSizePointY-WINDOWHEIGHT)/BASEHEIGHT;

  //    scriptVec.push_back(Script(AniViewMoveTo,QString("%1#%2#")
  //                               .arg(x)
  //                               .arg(y)));
  //    scriptVec.push_back(Script(AniCursorMoveToViewPoint,QString("%1#%2#")
  //                               .arg(selfViewVec[0]->getPointX()-x*BASEWIDTH)
  //                               .arg(selfViewVec[0]->getPointY()-y*BASEHEIGHT)));
  //    //动画结束后视角回到主角里

  //    currentActiveEnemyNum--;
  //    scriptVec.push_back(Script(AniEnd));
  //    this->startAnimation();
}
void BattleScene::processFriendAction() {
  if (currentActiveFriendNum == friendViewVec.size()) {
    scriptVec.push_back(Script(AniShowFriendAction));
  }
  focusRole = friendViewVec[friendViewVec.size() - currentActiveFriendNum];
  assert(focusRole);
  int focusRoleX = focusRole->getMapX(), focusRoleY = focusRole->getMapY();

  //判断当前是否中毒或者失去行动能力
  int disableFlag = (mapArray[focusRoleY][focusRoleX] & 0x0f0000) >> 16;
  int poisonFlag = (mapArray[focusRoleY][focusRoleX] & 0xf00000) >> 20;

  if (poisonFlag != 0) {
    assert(poisonFlag < WEAPONPOISON_50 + 1 && poisonFlag > WEAPONPOISON_5 - 1);
    Role *role = focusRole->getRolePointer();
    int damage = 0.05 * (role->HPMax) * (poisonFlag - WEAPONPOISON_5 + 1);
    if (role->HP <= damage) {
      damage = role->HP - 1;
    }
    scriptVec.push_back(Script(
        AniRolePoison, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
    scriptVec.push_back(Script(AniRoleHurt, QString("%1#%2#%3#%4#%5#")
                                                .arg(focusRoleX)
                                                .arg(focusRoleY)
                                                .arg(role->HP)
                                                .arg(damage)
                                                .arg(role->HPMax)));
    role->HP -= damage;
  }
  if (disableFlag != 0) {
    assert(disableFlag > WEAPONDISABLE_1 - 1 &&
           disableFlag < WEAPONDISABLE_10 + 1);
    disableFlag--;
    mapArray[focusRoleY][focusRoleX] &= 0x0fffff; //右三字节高4位清零
    mapArray[focusRoleY][focusRoleX] |= (disableFlag << 20);
    //过去了一个回合,因此flag-1后重新放回标志位
    scriptVec.push_back(Script(
        AniRoleDisable, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
    scriptVec.push_back(Script(AniFocusRoleBeInactive));
    // this->startAnimation();
    return;
  }

  //判断敌人是攻击系还是回复系
  int recoverHP = 25 * (focusRole->getRolePointer()->weapon[0] -
                        (ENEMYWEAPONRECOVER_25 - 1));
  if (recoverHP > 0 &&
      recoverHP <
          251) { //证明是回复系,回复系按照从前到后的顺序检查有没有受伤的friend角色
    //优先顺序:如果靠前的敌人能够进行回复则直接回复
    //如果没有能回复的角色就正常攻击
    targetRole = friendViewVec[0];

    RoleView *currentRoleView = NULL;
    for (int i = 0; i < friendViewVec.size() + selfViewVec.size();
         ++i) //进行跨容器遍历
    {
      if (i < friendViewVec.size()) {
        currentRoleView = friendViewVec[i];
      } else {
        assert(i - friendViewVec.size() < selfViewVec.size());
        currentRoleView = selfViewVec[i - friendViewVec.size()];
      }

      Role *currentRole = currentRoleView->getRolePointer();
      if ((currentRole->HP) < (currentRole->HPMax)) {
        targetRole = currentRoleView;
        if (focusRole == targetRole) { //如果自己补自己的话
          this->cursorMoveToFocusRole();
          scriptVec.push_back(
              Script(AniRoleRotate,
                     QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
          this->roleCure(focusRoleX, focusRoleY, recoverHP);
          // this->startAnimation();
          return;
        }
        vector<QPoint> path;
        AStarPathFinder(mapArray[0], mapSizeX, mapSizeY, targetRole->getMapX(),
                        targetRole->getMapY(), focusRoleX, focusRoleY, path);
        //寻路算法,得到是从friend指向hero的路径

        int step = (focusRole->getRolePointer()->speed);
        for (vector<QPoint>::iterator iter = path.begin();
             iter != path.end() - 1; ++iter, --step) {
          if (step < 0) { //到了自己的行动极限还是打不到
            break;
          }
          if (checkIfInRange(
                  *iter, path[path.size() - 1],
                  focusRole->getRolePointer()->range)) { //在某一步已经可以打到
            this->cursorMoveToFocusRole();
            scriptVec.push_back(
                Script(AniRoleRotate, QString("%1#%2#")
                                          .arg(focusRole->getMapX())
                                          .arg(focusRole->getMapY())));
            if (path.size() != 2) //=2的时候证明不需要移动
            {
              scriptVec.push_back(
                  Script(AniRoleFastMove, QString("%1#%2#")
                                              .arg(focusRole->getMapX())
                                              .arg(focusRole->getMapY())));
              iter++;
              iter->setX(-1);
              iter->setY(
                  -1); //攻击位置的下一个元素置为(-1,-1),这样,role移动到攻击位置就会停止
              focusRole->setPathVec(new vector<QPoint>(path));
              iter--;
            }

            this->roleCure(iter->x(), iter->y(), recoverHP);
            // this->startAnimation();
            return;
          }
        }
      }
    }
  }
  //是攻击系或者无法在范围内回复自己角色,则进行攻击
  targetRole = enemyViewVec[0];
  int nearestDistance =
      MANHATTAN(enemyViewVec[0]->getMapX(), enemyViewVec[0]->getMapY(),
                focusRoleX, focusRoleY);
  int currentDistance = nearestDistance;
  for (int i = 1; i < enemyViewVec.size(); ++i) {
    currentDistance =
        MANHATTAN(enemyViewVec[i]->getMapX(), enemyViewVec[i]->getMapY(),
                  focusRoleX, focusRoleY);
    if (nearestDistance > currentDistance) {
      nearestDistance = currentDistance;
      targetRole = enemyViewVec[i];
    }
  }

  //寻找hero中manhattan距离最近的角色的index
  vector<QPoint> path;
  AStarPathFinder(mapArray[0], mapSizeX, mapSizeY, targetRole->getMapX(),
                  targetRole->getMapY(), focusRoleX, focusRoleY, path);
  //寻路算法,得到是从friend指向hero的路径

  //    qDebug()<<(targetRole->getMapX())<<" "<<(targetRole->getMapY())<<"
  //    "<<focusRoleX<<" "<<focusRoleY; for(int i=0;i<path.size();++i)
  //    {
  //        qDebug()<<path[i].x()<<" "<<path[i].y();
  //    }

  this->cursorMoveToFocusRole();
  //首先将光标固定到当前角色
  if (path.empty()) {
    this->focusRoleDonotMove();
    // this->startAnimation();
    return;
  }
  assert(path.size() > 1);
  // path的第一个元素是攻击方(AI)的坐标,最后一个元素是防御方的坐标
  int i = (focusRole->getRolePointer()->speed);
  for (vector<QPoint>::iterator iter = path.begin(); iter != path.end() - 1;
       ++iter, --i) {
    if (i < 0) { //到了自己的行动极限还是打不到,尽可能远的移动
      scriptVec.push_back(Script(
          AniRoleRotate, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
      vector<QPoint> *p = new vector<QPoint>(
          path.begin(),
          path.begin() + (focusRole->getRolePointer()->speed) + 1);
      //由当前的focusRole负责释放
      p->push_back(QPoint(-1, -1));
      focusRole->setPathVec(p);
      scriptVec.push_back(Script(
          AniRoleFastMove, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
      scriptVec.push_back(Script(AniFocusRoleBeInactive));
      break;
    }
    if (checkIfInRange(*iter, path[path.size() - 1],
                       focusRole->getRolePointer()
                           ->range)) { //在某一步时候攻击方已经可以打到防御方
      // path.pop_back();//踢掉目标终点坐标
      scriptVec.push_back(Script(
          AniRoleRotate, QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));
      if (path.size() != 2) //=2的时候证明不需要移动
      {
        scriptVec.push_back(
            Script(AniRoleFastMove,
                   QString("%1#%2#").arg(focusRoleX).arg(focusRoleY)));

        iter++;
        iter->setX(-1);
        iter->setY(
            -1); //攻击位置的下一个元素置为(-1,-1),这样,role移动到攻击位置就会停止
        focusRole->setPathVec(new vector<QPoint>(path));
        iter--;
      }
      this->roleAttack(iter->x(), iter->y());

      break;
    }
  }

  // this->startAnimation();
}
void BattleScene::processSelfAction() {
  currentActiveHeroNum = selfViewVec.size();
  scriptVec.push_back(Script(AniShowSelfAction));
  scriptVec.push_back(
      Script(AniShowRoundX, QString("%1").arg(currentRoundNum)));

  for (int i = 0; i < selfViewVec.size(); ++i) { //逐个检查是否有中毒情况
    RoleView *tempRoleView = selfViewVec[i];
    int poisonFlag =
        (mapArray[tempRoleView->getMapY()][tempRoleView->getMapX()] &
         0xf00000) >>
        20;
    int disableFlag =
        (mapArray[tempRoleView->getMapY()][tempRoleView->getMapX()] &
         0x0f0000) >>
        16;
    if (poisonFlag != 0) { //如果处于中毒状态
      assert(poisonFlag > WEAPONPOISON_5 - 1 &&
             poisonFlag < WEAPONPOISON_50 + 1);
      Role *role = tempRoleView->getRolePointer();
      int damage = 0.05 * (role->HPMax) * (poisonFlag - WEAPONPOISON_5 + 1);
      if (role->HP <= damage) {
        damage = role->HP - 1;
      }
      scriptVec.push_back(
          Script(AniRolePoison, QString("%1#%2#")
                                    .arg(tempRoleView->getMapX())
                                    .arg(tempRoleView->getMapY())));
      scriptVec.push_back(Script(AniRoleHurt, QString("%1#%2#%3#%4#%5#")
                                                  .arg(tempRoleView->getMapX())
                                                  .arg(tempRoleView->getMapY())
                                                  .arg(role->HP)
                                                  .arg(damage)
                                                  .arg(role->HPMax)));
      role->HP -= damage;
    }
    if (disableFlag != 0) {
      assert(disableFlag > WEAPONDISABLE_1 - 1 &&
             disableFlag < WEAPONDISABLE_10 + 1);
      disableFlag--;
      mapArray[tempRoleView->getMapY()][tempRoleView->getMapX()] &= 0xf0ffff;
      mapArray[tempRoleView->getMapY()][tempRoleView->getMapX()] |=
          (disableFlag << 16);
      scriptVec.push_back(
          Script(AniRoleDisable, QString("%1#%2#")
                                     .arg(tempRoleView->getMapX())
                                     .arg(tempRoleView->getMapY())));
    }
  }

  assert(selfViewVec[0]);
  int x = (selfViewVec[0]->getMapX() - (WINDOWWIDTH / BASEWIDTH / 2));
  int y = (selfViewVec[0]->getMapY() - (WINDOWHEIGHT / BASEHEIGHT / 2));

  if (x < 0)
    x = 0;
  else if (x > (mapSizePointX - WINDOWWIDTH) / BASEWIDTH)
    x = (mapSizePointX - WINDOWWIDTH) / BASEWIDTH;

  if (y < 0)
    y = 0;
  else if (y > (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT)
    y = (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT;

  scriptVec.push_back(
      Script(AniViewMoveTo, QString("%1#%2#")
                                .arg(selfViewVec[0]->getMapX())
                                .arg(selfViewVec[0]->getMapY())));

  scriptVec.push_back(
      Script(AniCursorMoveToViewPoint,
             QString("%1#%2#")
                 .arg(selfViewVec[0]->getPointX() - x * BASEWIDTH)
                 .arg(selfViewVec[0]->getPointY() - y * BASEHEIGHT)));

  // scriptVec.push_back(Script(AniEnd));
  // this->startAnimation();
}
void BattleScene::roleCure(int special) {
  assert(focusRole);
  assert(targetRole);
  int recoverHP = 0;
  Role *r = targetRole->getRolePointer();
  switch (special) {
  case WEAPONRECOVER_25:
  case MEDICINERECOVER_25:
    recoverHP = 25;
    break;
  case WEAPONRECOVER_50:
  case MEDICINERECOVER_50:
    recoverHP = 50;
    break;
  case WEAPONRECOVER_75:
  case MEDICINERECOVER_75:
    recoverHP = 75;
    break;
  case MEDICINERECOVER_100:
  case WEAPONRECOVER_100:
  case WEAPONMULTIRECOVER_100:
    recoverHP = 100;
    break;
  case WEAPONRECOVER_150:
    recoverHP = 150;
    break;
  case WEAPONRECOVER_200:
  case MEDICINERECOVER_200:
    recoverHP = 200;
    break;
  case MEDICINERECOVER_999:
    recoverHP = 999;
    break;
  default:
    qDebug() << special;
    assert(0);
  }

  scriptVec.push_back(
      Script(AniRoleRecover, QString("%1#%2#%3#%4#%5#")
                                 .arg(targetRole->getMapX())
                                 .arg(targetRole->getMapY())
                                 .arg(r->HP)
                                 .arg(recoverHP)
                                 .arg(targetRole->getRolePointer()->HPMax)));

  r->HP += recoverHP;
  if (r->HP > r->HPMax)
    r->HP = r->HPMax;

  scriptVec.push_back(Script(AniFocusRoleBeInactive));
}
void BattleScene::onMenuSelect(int index) {
  if (index == 0) //回合结束
  {
    emit changeActiveStatusTo(false);
    focusRole = NULL;
    currentRoundNum++;
    currentActiveEnemyNum = enemyViewVec.size();
    this->processEnemyAction();
    this->startAnimation();
    // cursorControlOn=false;
  } else if (index == 1) //存档
  {
    SaveLoadWidget *widget = new SaveLoadWidget(this, key, OperationSave);
    // widget->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT*2);
    connect(widget, SIGNAL(saveToFile()), this->parentWidget(),
            SLOT(onSaveData()));
  } else if (index == 2) //读档
  {
    SaveLoadWidget *widget = new SaveLoadWidget(this, key, OperationLoad);
    // widget->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT*2);
    connect(widget, SIGNAL(loadFrom(QString)), this->parentWidget(),
            SLOT(loadScene(QString)));
    // this->close();
  } else if (index == 3) //战斗目的
  {
    QString winRequirementText, failRequirementText;
    DBIO dbio;
    if (!dbio.getBattleWinFailTextFromLib(battleID, winRequirementText,
                                          failRequirementText))
      quitApp(ERRORGETBATTLEWINFAILDATAFAIL);
    BattleAimWidget *w =
        new BattleAimWidget(this, winRequirementText, failRequirementText);
  } else //撤退
  {
    scriptVec.clear();
    currentScriptIndex = 0;
    scriptVec.push_back(Script(AniStart));
    scriptVec.push_back(Script(AniShowBattleFail));
    this->startAnimation();
  }
}
void BattleScene::roleDie(int x, int y) {
  int index = (mapArray[y][x] & 0xff00) >> 8;
  switch (mapArray[y][x] & 0xff) {
  case static_cast<int>(BattleHero):
    selfViewVec[index]->close();
    // delete selfViewVec[index];
    for (int i = index; i < selfViewVec.size() - 1; ++i) {
      selfViewVec[i] = selfViewVec[i + 1];
      mapArray[selfViewVec[i]->getMapY()][selfViewVec[i]->getMapX()] -= 0x0100;
      // mapArray中的索引值前移1
    }
    selfViewVec.pop_back();
    break;
  case static_cast<int>(BattleEnemy):
    enemyViewVec[index]->close();
    // delete enemyViewVec[index];
    for (int i = index; i < enemyViewVec.size() - 1; ++i) {
      enemyViewVec[i] = enemyViewVec[i + 1];
      mapArray[enemyViewVec[i]->getMapY()][enemyViewVec[i]->getMapX()] -=
          0x0100;
      // mapArray中的索引值前移1
    }
    enemyViewVec.pop_back();
    break;
  case static_cast<int>(BattleFriend):
    friendViewVec[index]->close();
    // delete friendViewVec[index];
    for (int i = index; i < friendViewVec.size() - 1; ++i) {
      friendViewVec[i] = friendViewVec[i + 1];
      mapArray[friendViewVec[i]->getMapY()][friendViewVec[i]->getMapX()] -=
          0x0100;
      // mapArray中的索引值前移1
    }
    friendViewVec.pop_back();
    break;
  default:
    assert(0);
  }
  mapArray[y][x] = 0;
  switch (this->checkBattleEnd()) {
  case BattleWin:
    // scriptVec.swap(vector<Script>());
    scriptVec.clear();
    scriptVec.push_back(Script(AniShowBattleWin));
    currentScriptIndex = 0;
    break;
  case BattleFail:
    // scriptVec.swap(vector<Script>());
    scriptVec.clear();
    scriptVec.push_back(Script(AniShowBattleFail));
    currentScriptIndex = 0;
  default:
    break;
  }
  this->startAnimation();
}
bool BattleScene::saveDataTo(DBIO &dbio) {
  BattleSceneData sceneData(currentPointX, currentPointY, battleID, mapID,
                            cursorWidget->getViewPointX(),
                            cursorWidget->getViewPointY(), currentRoundNum);
  /*
  uint scriptDataLength=0;//计算script需要的字节数
  for(int i=0;i<scriptVec.size();++i)
  {
      scriptDataLength+=(sizeof(AnimationOperation)+scriptVec[i].param.toUtf8().size()+1);
  }

  uchar *scriptData=new uchar[scriptDataLength];
  uchar *currentPointer=scriptData;

  *(reinterpret_cast<uint *>(currentPointer))=scriptVec.size();
  currentPointer+=sizeof(uint);

  for(int i=0;i<scriptVec.size();++i)
  {
      *(reinterpret_cast<AnimationOperation*>(currentPointer))=scriptVec[i].op;//复制op数据

      QByteArray scriptParamData=scriptVec[i].param.toUtf8();
      memcpy(currentPointer+sizeof(AnimationOperation),
             scriptParamData.data(),
             scriptParamData.size()+1);//复制param数据
      currentPointer+=(sizeof(AnimationOperation)+scriptParamData.size()+1);
  }
  assert(scriptData+scriptDataLength==currentPointer);
  */
  int vecSizeSum =
      selfViewVec.size() + enemyViewVec.size() + friendViewVec.size();
  int *flagData = new int[vecSizeSum];
  memset(flagData, 0, sizeof(int) * (vecSizeSum));
  int length = 0;
  for (int i = 0; i < selfViewVec.size(); ++i) {
    int flag = (mapArray[selfViewVec[i]->getMapY()][selfViewVec[i]->getMapX()] &
                0xff0000);
    if (flag != 0) {
      flag |= selfViewVec[i]->getMapX();
      flag |= (selfViewVec[i]->getMapX()) << 16;
      flagData[length++] |= flag;
    }
  }
  for (int i = 0; i < enemyViewVec.size(); ++i) {
    int flag =
        (mapArray[enemyViewVec[i]->getMapY()][enemyViewVec[i]->getMapX()] &
         0xff0000);
    if (flag != 0) {
      flag |= enemyViewVec[i]->getMapX();
      flag |= (enemyViewVec[i]->getMapX()) << 16;
      flagData[length++] |= flag;
    }
  }
  for (int i = 0; i < friendViewVec.size(); ++i) {
    int flag =
        (mapArray[friendViewVec[i]->getMapY()][friendViewVec[i]->getMapX()] &
         0xff0000);
    if (flag != 0) {
      flag |= friendViewVec[i]->getMapX();
      flag |= (friendViewVec[i]->getMapX()) << 16;
      flagData[length++] |= flag;
    }
  }
  if (!dbio.saveBattleSceneToFile(sceneData, selfViewVec, enemyViewVec,
                                  friendViewVec, mapArray[0],
                                  mapSizeX * mapSizeY))
    return false;

  delete[] flagData;

  return true;
  //归档完成
}
void BattleScene::selectTarget(DIRECTION dir) {
  int targetType = static_cast<int>(BattleHero), articleSpecial = 0;
  if (displayState == BattleShowSelfRange) {
    articleSpecial = (*pArticleIDToInfoMap)[focusRole->getRolePointer()
                                                ->weapon[weaponSelectedIndex]]
                         .special;
  } else {
    assert(displayState == BattleShowSelfMedicineRange);
    articleSpecial =
        (*pArticleIDToInfoMap)[focusRole->getRolePointer()
                                   ->medicine[medicineSelectedIndex]]
            .special;
  }
  //本次允许选择的敌人类型
  switch (articleSpecial) {
  case WEAPONRECOVER_25:
  case WEAPONRECOVER_50:
  case WEAPONRECOVER_100:
  case WEAPONMULTIRECOVER_100:
  case WEAPONANTIPOISON:
  case WEAPONRECOVER:
  case MEDICINERECOVER_25:
  case MEDICINERECOVER_50:
  case MEDICINERECOVER_75:
  case MEDICINERECOVER_200:
  case MEDICINERECOVER_100:
  case MEDICINERECOVER_999:
    targetType =
        static_cast<int>(BattleHero); //如果允许选择hero也允许选择friend
    break;
  default:
    targetType = static_cast<int>(BattleEnemy);
    break;
  }
  assert(focusRole && focusRole->getType() == static_cast<int>(BattleHero));
  assert(rangeVec.size() != 0);
  int x = (cursorWidget->getViewPointX() + currentPointX) / BASEWIDTH;
  int y = (cursorWidget->getViewPointY() + currentPointY) / BASEHEIGHT;
  /*
      //以下if分支的意义在于,首先在临近的四个格子内寻找目标,如果找到就直接使用,否则再去射程内寻找
      if(dir==UP)
      {
          if(y!=0)
          {
              if((mapArray[y-1][x]&0xff)==targetType)
              {
                  cursorWidget->moveToPointOfView(x*BASEWIDTH-currentPointX,
                                                  (y-1)*BASEHEIGHT-currentPointY);
                  break;
              }
          }
      }
      else if(dir==DOWN)
      {
          if(y!=mapSizeY-1)
          {
              if((mapArray[y+1][x]&0xff)==targetType)
              {
                  cursorWidget->moveToPointOfView(x*BASEWIDTH-currentPointX,
                                                  (y+1)*BASEHEIGHT-currentPointY);
                  break;
              }
          }
      }
      else if(dir==LEFT)
      {
          if(x!=0)
          {
              if((mapArray[y][x-1]&0xff)==targetType)
              {
                  cursorWidget->moveToPointOfView((x-1)*BASEWIDTH-currentPointX,
                                                  y*BASEHEIGHT-currentPointY);
                  break;
              }
          }
      }
      else
      {
          if(x!=mapSizeX-1)
          {
              if((mapArray[y][x+1]&0xff)==targetType)
              {
                  cursorWidget->moveToPointOfView((x+1)*BASEWIDTH-currentPointX,
                                                  y*BASEHEIGHT-currentPointY);
                  break;
              }
          }
      }
      */

  vector<QPoint>::iterator iter = rangeVec.begin();
  //        for(int i=0;i<rangeVec.size();++i)
  //        {
  //            qDebug()<<rangeVec[i].x()<<" "<<rangeVec[i].y();
  //        }
  while (*iter != QPoint(x, y))
    iter++;
  //首先定位到光标当前所在的位置,肯定可以找到,因为(focusRole.x,y)也被加入了rangeVec
  int count = 0; //计数器,如果搜索了整个vector都找不到光标可以停留的位置则不动
  if (dir == UP || dir == LEFT) { //向前检索下一个可以停留的位置
    while (1) {
      iter++;
      if (iter == rangeVec.end())
        iter = rangeVec.begin();
      x = iter->x();
      y = iter->y();

      // qDebug()<<count<<":"<<x<<" "<<y<<" "<<(mapArray[y][x]&0xff)<<"
      // "<<targetType; qDebug()<<(mapArray[y][x]&0xff)<<" "<<targetType<<"
      // "<<((mapArray[y][x]&0xff)==targetType);
      if ((mapArray[y][x] & 0xff) == targetType ||
          (targetType == static_cast<int>(BattleHero) &&
           (mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend))) {
        cursorWidget->moveToPointOfView(x * BASEWIDTH - currentPointX,
                                        y * BASEHEIGHT - currentPointY);
        break;
      }
      count++;
      if (count == rangeVec.size()) { //已经完成了对rangeVector的遍历
        break;
      }
    }
  } else { //向后检索下一个可以停留的位置
    while (1) {
      if (iter == rangeVec.begin())
        iter = rangeVec.end();
      iter--;
      x = iter->x();
      y = iter->y();

      if ((mapArray[y][x] & 0xff) == targetType ||
          (targetType == static_cast<int>(BattleHero) &&
           (mapArray[y][x] & 0xff) == static_cast<int>(BattleFriend))) {
        cursorWidget->moveToPointOfView(x * BASEWIDTH - currentPointX,
                                        y * BASEHEIGHT - currentPointY);
        break;
      }
      count++;
      if (count == rangeVec.size()) { //已经完成了对rangeVector的遍历
        break;
      }
    }
  }
}
void BattleScene::cursorMoveToFocusRole() {
  int x = focusRole->getMapX() - WINDOWWIDTH / BASEWIDTH / 2,
      y = focusRole->getMapY() - WINDOWHEIGHT / BASEHEIGHT / 2;
  scriptVec.push_back(Script(
      AniViewMoveTo,
      QString("%1#%2#").arg(focusRole->getMapX()).arg(focusRole->getMapY())));

  if (x < 0)
    x = 0;
  if (x > (mapSizePointX - WINDOWWIDTH) / BASEWIDTH)
    x = (mapSizePointX - WINDOWWIDTH) / BASEWIDTH;

  if (y < 0)
    y = 0;
  if (y > (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT)
    y = (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT;

  scriptVec.push_back(
      Script(AniCursorMoveToViewPoint,
             QString("%1#%2#")
                 .arg(focusRole->getPointX() - x * BASEWIDTH)
                 .arg(focusRole->getPointY() - y * BASEHEIGHT)));
}
void BattleScene::focusOutEvent(QFocusEvent *) {
  upKeyPressed = false;
  downKeyPressed = false;
  leftKeyPressed = false;
  rightKeyPressed = false;
}
void BattleScene::roleCure(int x, int y, int recoverHP) {
  scriptVec.push_back(Script(AniRoleCure, QString("%1#%2#").arg(x).arg(y)));
  Role *target = targetRole->getRolePointer();

  scriptVec.push_back(Script(AniRoleRecover, QString("%1#%2#%3#%4#%5#")
                                                 .arg(targetRole->getMapX())
                                                 .arg(targetRole->getMapY())
                                                 .arg(target->HP)
                                                 .arg(recoverHP)
                                                 .arg(target->HPMax)));
  target->HP += recoverHP;
  if (target->HP > target->HPMax) {
    target->HP = target->HPMax;
  }

  scriptVec.push_back(Script(AniFocusRoleBeInactive));
}
