//#include "storyscene.h"
#include "storyscene.h"
#include "dialoglabel.h"
#include "getgoodslabel.h"
#include "menuwidget.h"
#include "roleanimation.h"
#include "roleinfowidget.h"
#include "statuswidget.h"
#include "tradewidget.h"

StoryScene::StoryScene(QWidget *parent, KeyboardControl *key, bool gameInit)
    : QWidget(parent),
      // dbio(DBIO()),
      storyID(0), sideStoryID(0), mapID(0), mapArray(NULL), battleScene(NULL),
      viewMoveTimerID(0), autoRecoverTimerID(0), viewAnimationMoveTimerID(0),
      carriedCash(1000),
      //    fadeTimerID(0),
      //    currentAlpha(255),
      mapSizeX(0), mapSizeY(0), mapSizePointX(0), mapSizePointY(0),
      currentPointX(0), currentPointY(0), // currentBlockX(0),currentBlockY(0),
      focusRole(NULL), targetRole(NULL), key(key),
      //    key->upKey(Qt::Key_Up),key->downKey(Qt::Key_Down),key->lefKey(Qt::Key_Left),key->rightKey(Qt::Key_Right),
      //    key->actionKey(Qt::Key_A),key->cancelKey(Qt::Key_S),key->menuKey(Qt::Key_Space),
      upKeyPressed(false), downKeyPressed(false), leftKeyPressed(false),
      rightKeyPressed(false), viewIsMoving(false), focusDir(NOTMOVING),
      focusDirCache(NOTMOVING), animationEndX(-1), animationEndY(-1),
      viewDelayTimerID(0), currentScriptIndex(0),
      roleInfoArray(new Role *[ROLENUMBER]), currentBattleID(0),
      userControlOn(false) {
  this->show();
  // this->setFocus();
  this->setGeometry(0, 23, WINDOWWIDTH, WINDOWHEIGHT);
  this->setAttribute(Qt::WA_DeleteOnClose);
  memset(roleInfoArray, 0, sizeof(Role *) * ROLENUMBER);
  if (gameInit) {
    DBIO dbio;
    if (!dbio.getRoleInfoFromLibOfID(roleInfoArray, 1))
      quitApp(ERRORGETROLEINFOFAIL);
    // if(!dbio.getGlobalGoodsIDFromLib(gotArticleSet))quitApp(ERRORGETARTICLESETFAIL);
    loadScene(1, 6, 0, 4);
    // loadScene(2,5,8,24);
    // loadScene(3,6,3,11);
    // loadScene(4,5,8,24);
    // loadScene(6,3,3,12);
    // loadScene(11,1,15,36);
    // loadScene(0,8,32,30);
  }
  /***********以下是调试代码************/
  //    carriedArticleIDSet.insert(9999);
  //    carriedArticleIDSet.insert(9998);
  //    carriedArticleIDSet.insert(9997);
  //    carriedArticleIDSet.insert(9996);
  //    carriedArticleIDSet.insert(9995);
  //    carriedArticleIDSet.insert(9995);
  //    carriedArticleIDSet.insert(20000);
  //    carriedArticleIDSet.insert(7000);
  //    carriedArticleIDSet.insert(7000);
  //    carriedArticlaeIDSet.insert(7000);
  //    carriedArticleIDSet.insert(1);
  //    carriedArticleIDSet.insert(2);
  /****************调试代码****************/
}
void StoryScene::loadScene(const QString &fileName) {
  userControlOn = false;

  if (!scriptVec.empty())
    scriptVec.clear();
  // scriptVec.swap(vector<Script>());
  if (!roleViewVec.empty()) {
    for (int i = 0; i < roleViewVec.size(); ++i) {
      delete roleViewVec[i];
    }
    // roleViewVec.swap(vector<RoleView*>());
    roleViewVec.clear();
  }
  //    if(!objectVec.empty())
  //    {
  //        for(int i=0;i<objedctVec.size();++i)
  //        {
  //            delete objectVec[i];
  //        }
  //        objectVec.clear();
  //        //objectVec.swap(vector<InvisibleObject *>());
  //    }
  if (!gotArticleSet.empty())
    gotArticleSet.clear();
  if (!carriedArticleIDSet.empty())
    carriedArticleIDSet.clear();
  if (mapArray) {
    delete[] mapArray[0];
    delete mapArray;
    mapArray = NULL;
  }

  focusRole = NULL;
  if (viewMoveTimerID != 0) {
    killTimer(viewMoveTimerID);
    viewMoveTimerID = 0;
  }
  animationEndX = 0;
  animationEndY = 0;
  upKeyPressed = false;
  downKeyPressed = false;
  leftKeyPressed = false;
  rightKeyPressed = false;
  focusDir = NOTMOVING;
  focusDirCache = NOTMOVING;
  for (int i = 0; i < ROLENUMBER; ++i) {
    delete roleInfoArray[i];
    roleInfoArray[i] = NULL;
  }

  //清理工作

  int *temp = NULL; //先暂存读取到的mapArray指针
  DBIO dbio;
  StorySceneData data(0, 0, 0, 0, 0, 0, 0);
  if (!dbio.getStorySceneDataFromSaveFile(this, fileName, data, roleViewVec,
                                          roleInfoArray, gotArticleSet,
                                          carriedArticleIDSet, &temp))
    quitApp(ERRORSAVEFILEREADFAIL);
  currentPointX = data.currentPointX;
  currentPointY = data.currentPointY;
  currentBattleID = data.currentBattleID;
  mapID = data.mapID;
  storyID = data.storyID;
  carriedCash = data.carriedCash;
  sideStoryID = data.sideStoryID;

  map.load(QApplication::applicationDirPath() +
           QString("/data/image/map/%1.jpg").arg(mapID));
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

  // memcpy(mapArray[0],getMapPointerFromStoryMapID(mapID),sizeof(int)*mapSizeX*mapSizeY);

  //开始标记roleView和object

  for (uint i = 0; i < roleViewVec.size(); ++i) {
    int roleMapX = roleViewVec[i]->getMapX();
    int roleMapY = roleViewVec[i]->getMapY();
    roleViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                roleMapY * BASEHEIGHT - currentPointY,
                                BASEWIDTH, BASEHEIGHT);
    roleViewVec[i]->setMapArray(mapArray);
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), roleViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(roleViewVec[i], SIGNAL(roleDisappear(int, int)), this,
            SLOT(onRoleDisappear(int, int)));
    connect(roleViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
    if (roleViewVec[i]->getID() == 1) {
      // mapArray[roleMapY][roleMapX]<<=16;//原先地图元素移至右三字节
      mapArray[roleMapY][roleMapX] &= 0xffff0000;
      mapArray[roleMapY][roleMapX] |=
          ((i << 8) | (static_cast<int>(StoryMapHero)));
      focusRole = roleViewVec[i];
      roleViewVec[i]->setReactOnViewMove(false);
    } else {
      // mapArray[roleMapY][roleMapX]<<=16;
      mapArray[roleMapY][roleMapX] &= 0xffff0000;
      mapArray[roleMapY][roleMapX] |=
          ((i << 8) | (static_cast<int>(StoryMapNPC)));
    }
  }
  focusDir = focusRole->getDirection();

  //    for(uint i=0;i<objectVec.size();++i)
  //    {
  //        int x=objectVec[i]->getX();
  //        int y=objectVec[i]->getY();
  //        if(objectVec[i]->getID()==0)
  //            mapArray[y][x]=(mapArray[y][x]<<16)|(i<<8)|static_cast<int>(StoryMapObject);
  //        else
  //            mapArray[y][x]=(mapArray[y][x]<<16)|(i<<8)|static_cast<int>(StoryMapArticle);
  //    }

  emit viewHasMovedToPoint(currentPointX, currentPointY);

  if (battleScene) {
    battleScene->close();
    battleScene = NULL; //关闭当前的战斗
  }

  if (currentBattleID) { //若存档中存在战斗
    battleScene =
        new BattleScene(this, dbio, key, roleInfoArray, articleIDToInfoMap);
    if (autoRecoverTimerID)
      killTimer(autoRecoverTimerID);
    autoRecoverTimerID = 0;
    // battleScene->setFocus();
    for (int i = 0; i < roleViewVec.size(); ++i)
      roleViewVec[i]->hide();
    connect(battleScene, SIGNAL(battleFail()), this, SLOT(onBattleFail()));
    connect(battleScene, SIGNAL(battleWin()), this, SLOT(onBattleWin()));
  } else {
    this->setFocus();
    if (autoRecoverTimerID == 0) {
      autoRecoverTimerID = startTimer(AUTORECOVERINTERVAL);
    }
  }

  userControlOn = true;

  FadeScene *fade = new FadeScene(this);
  fade->raise();
}
void StoryScene::loadScene(int storyID, int mapID, int heroX, int heroY) {
  //    fadeTimerID=startTimer(FADEINTERVAL);
  if (autoRecoverTimerID == 0) {
    autoRecoverTimerID = startTimer(AUTORECOVERINTERVAL);
  }

  upKeyPressed = false;
  downKeyPressed = false;
  leftKeyPressed = false;
  rightKeyPressed = false;
  userControlOn = false;
  DBIO dbio;
  /**
    首先清除现有的roleViewVector和objectVector中的所有内容,释放maparray,
    然后从相应位置读取地图文件名和地图坐标,在正确坐标上装载地图,建立新maparray
    然后读取文件中role和obj的信息,分别实例化后加入vector中,在此过程中将相应信息加入maparray
    **/

  if (mapArray) {
    delete[] mapArray[0];
    delete[] mapArray;
    mapArray = NULL;
  }

  focusRole = NULL;
  focusDir = DOWN;

  if (roleViewVec.empty()) { //初始化的时候把hero建立起来
    roleViewVec.push_back(new RoleView(this, heroX, heroY, 1));
  } else {
    for (int i = roleViewVec.size() - 1; i > 0; --i) {
      delete roleViewVec[i]; //销毁除了hero之外的角色
      roleViewVec.pop_back();
    }
    // roleViewVec.swap(vector<RoleView *>());
  }
  // roleViewVec[0]->hide();

  //    if(!objectVec.empty())
  //    {
  //        objectVec.clear();
  //        //objectVec.swap(vector<InvisibleObject *>());
  //        for(int i=0;i<objectVec.size();++i)
  //        {
  //            delete objectVec[i];
  //            objectVec[i]=NULL;
  //        }
  //    }
  if (!scriptVec.empty())
    scriptVec.clear();
  // scriptVec.swap(vector<Script>());
  if (!carriedArticleIDSet.empty())
    carriedArticleIDSet.clear();
  if (viewMoveTimerID != 0) {
    killTimer(viewMoveTimerID);
    viewMoveTimerID = 0;
  }
  currentScriptIndex = 0;
  //清空roleViewVec和objectVec
  if (mapID != this->mapID) {
    map.load(QApplication::applicationDirPath() +
             QString("/data/image/map/%1.jpg").arg(mapID));
    this->mapID = mapID;
    assert(map.width() % BASEWIDTH == 0);
    assert(map.height() % BASEHEIGHT == 0);
    mapSizeX = map.width() / BASEWIDTH;
    mapSizeY = map.height() / BASEHEIGHT;
    mapSizePointX = map.width();
    mapSizePointY = map.height();
  }

  if (heroX != -1 &&
      heroY != -1) { //如果heroX,heroY=(-1,-1)则是原图重载，并且不改变视角位置
    currentPointX = (heroX - WINDOWWIDTH / BASEWIDTH / 2) * BASEWIDTH;
    currentPointY = (heroY - WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT;

    if (currentPointX < 0)
      currentPointX = 0;
    if (currentPointX > mapSizePointX - WINDOWWIDTH)
      currentPointX = mapSizePointX - WINDOWWIDTH;
    if (currentPointY < 0)
      currentPointY = 0;
    if (currentPointY > mapSizePointY - WINDOWHEIGHT)
      currentPointY = mapSizePointY - WINDOWHEIGHT;

    if (mapSizePointX < WINDOWWIDTH)
      currentPointX = (mapSizePointX - WINDOWWIDTH) / 2;
    if (mapSizePointY < WINDOWHEIGHT)
      currentPointY = (mapSizePointY - WINDOWHEIGHT) / 2;
    currentPointX -= (currentPointX % BASEWIDTH);
    currentPointY -= (currentPointY % BASEHEIGHT);
    roleViewVec[0]->setViewPointXY(currentPointX, currentPointY);
    roleViewVec[0]->moveToPointOfMap(heroX * BASEWIDTH, heroY * BASEHEIGHT);
  }
  roleViewVec[0]->stopAnimation();

  mapArray = new int *[mapSizeY];
  mapArray[0] = new int[mapSizeX * mapSizeY];
  for (int i = 1; i < mapSizeY; i++)
    mapArray[i] = mapArray[i - 1] + mapSizeX;
  memcpy(mapArray[0], getMapPointerFromStoryMapID(mapID),
         sizeof(int) * mapSizeX * mapSizeY);
  //    for(int i=0;i<scriptVec.size();++i)
  //    {
  //        qDebug()<<scriptVec[i].op;
  //        qDebug()<<scriptVec[i].param;
  //    }

  if (!dbio.getStorySceneDataFromLib(roleViewVec, scriptVec, gotArticleSet,
                                     this, storyID, mapID, mapArray,
                                     sideStoryID))
    quitApp(ERRORGETSTORYSCENEDATAFAIL);

  FadeScene *fade = new FadeScene(this);
  //#ifdef NDEBUG
  connect(fade, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  //#endif
  fade->raise();

  // qDebug()<<"roleViewVec"<<roleViewVec.size()<<"objectVec"<<objectVec.size();
  for (int i = 0; i < roleViewVec.size(); ++i) {
    int roleMapX = roleViewVec[i]->getMapX();
    int roleMapY = roleViewVec[i]->getMapY();
    roleViewVec[i]->setGeometry(roleMapX * BASEWIDTH - currentPointX,
                                roleMapY * BASEHEIGHT - currentPointY,
                                BASEWIDTH, BASEHEIGHT);
    roleViewVec[i]->setMapArray(mapArray);
    // roleViewVec[i]->setViewPoint(currentPointX);
    connect(roleViewVec[i], SIGNAL(roleAnimationEnd()), this,
            SLOT(startAnimation()));
    connect(this, SIGNAL(viewHasMovedToPoint(int, int)), roleViewVec[i],
            SLOT(onViewMovedTo(int, int)));
    connect(roleViewVec[i], SIGNAL(roleDisappear(int, int)), this,
            SLOT(onRoleDisappear(int, int)));
    // connect(roleViewVec[i],SIGNAL(roleAnimationEnd()),this,SLOT(startAnimation()));
    // connect(fade,SIGNAL(destroyed()),roleViewVec[i],SLOT(show()));
    // connect(roleViewVec[i],SIGNAL(startConversation(QString const
    // &)),this,SLOT(startConversation(QString)));
    // qDebug()<<bin<<"i:"<<i<<"i<<8:"<<(i<<8)<<"r[i]:"<<roleViewVec[i]->getType()<<"resault:"<<((i<<8)|(roleViewVec[i]->getType()));
    if (roleViewVec[i]->getID() == 1) {
      mapArray[roleMapY][roleMapX] <<= 16; //原先地图元素移至右三字节
      mapArray[roleMapY][roleMapX] |=
          ((i << 8) | (static_cast<int>(StoryMapHero)));
      focusRole = roleViewVec[i];
      roleViewVec[i]->setReactOnViewMove(false);
    } else {
      mapArray[roleMapY][roleMapX] <<= 16; //原先地图元素移至右三字节
      mapArray[roleMapY][roleMapX] |=
          ((i << 8) | (static_cast<int>(StoryMapNPC)));
    }
    roleViewVec[i]->show();
    //地图元素矩阵中的对应元素低八位为NPC的标志,次八位为在vector中的下标方便查找
  }

  //这几行代码用于载入新场景时根据出口位置重置focusRole的姿势的
  int focusRoleX = focusRole->getMapX(), focusRoleY = focusRole->getMapY();
  if (focusRoleX != 0 && ((mapArray[focusRoleY][focusRoleX - 1] & 0xff) ==
                          static_cast<int>(StoryMapExit))) { //左侧是出口
    focusRole->setPose(STAYRIGHT);
  } else if (focusRoleY != 0 &&
             ((mapArray[focusRoleY - 1][focusRoleX] & 0xff) ==
              static_cast<int>(StoryMapExit))) { //上方是出口
    focusRole->setPose(STAYFRONT);
  } else if (focusRoleX != mapSizeX - 1 &&
             ((mapArray[focusRoleY][focusRoleX + 1] & 0xff) ==
              static_cast<int>(StoryMapExit))) { //右侧是出口
    focusRole->setPose(STAYLEFT);
  } else if (focusRoleY != mapSizeY - 1 &&
             ((mapArray[focusRoleY + 1][focusRoleX] & 0xff) ==
              static_cast<int>(StoryMapExit))) { //下方是出口
    focusRole->setPose(STAYBACK);
  }
  //    for(int i=0;i<objectVec.size();++i)
  //    {
  //        int objectID=objectVec[i]->getID();
  //        //if(gotArticleSet.find(objectID)!=gotArticleSet.end())continue;
  //        //已经存在则不在地图上标注,当然也就不会触发物品响应
  //        if(objectID!=0)
  //        {

  //            mapArray[objectVec[i]->getY()][objectVec[i]->getX()]=
  //                    (mapArray[objectVec[i]->getY()][objectVec[i]->getX()]<<16)|(i<<8)|static_cast<int>(StoryMapArticle);
  //            //qDebug()<<hex<<mapArray[objectVec[i]->getY()][objectVec[i]->getX()];
  //        }
  //        else
  //            mapArray[objectVec[i]->getY()][objectVec[i]->getX()]=(i<<8)|static_cast<int>(StoryMapObject);
  //    }

  //    for(int i=0;i<scriptVec.size();++i)
  //    {
  //        qDebug()<<scriptVec[i].op;
  //        qDebug()<<scriptVec[i].param;
  //    }

  // char *data;
  // roleViewVec[1]->saveToData(&data);
  // RoleView *testView=new RoleView(this,data);
  emit viewHasMovedToPoint(currentPointX, currentPointY); //完成移动后发射信号

  // userControlOn=true;
  /*
  int focusRolePointX=focusRole->getPointX();
  int focusRolePointY=focusRole->getPointY();
  if(focusRolePointX-currentPointX==(static_cast<int>(WINDOWWIDTH/BASEWIDTH/2)*BASEWIDTH))
  {
      if(focusRolePointY-currentPointY==(static_cast<int>(WINDOWHEIGHT/BASEHEIGHT/2)*BASEHEIGHT))
      {
          if((currentPointX<=0&&focusDir==LEFT)||
                                  (currentPointX>=mapSizePointX-WINDOWWIDTH&&focusDir==RIGHT)||
                                  (currentPointY<=0&&focusDir==UP)||
                                  (currentPointY>=mapSizePointY-WINDOWHEIGHT&&focusDir==DOWN))
          {
              viewIsMoving=false;
          }
          else
              viewIsMoving=true;
      }
      else
      {
          if((focusDir==LEFT&&currentPointX>0)
                  ||(focusDir==RIGHT&&currentPointX<mapSizePointX-WINDOWWIDTH))viewIsMoving=true;
          else viewIsMoving=false;
      }
  }
  else
  if(focusRolePointY-currentPointY==(static_cast<int>(WINDOWHEIGHT/BASEHEIGHT/2)*BASEHEIGHT))
  {
      if((focusDir==UP&&currentPointY>0)||(focusDir==DOWN&&currentPointY<mapSizePointY-WINDOWHEIGHT))viewIsMoving=true;
      else viewIsMoving=false;
  }
  else viewIsMoving=false;
  */
  // qDebug()<<focusRolePointX<<" "<<focusRolePointY;
  // qDebug()<<viewIsMoving;
  // qDebug()<<mapArray[25][8];
  //    if(!scriptVec.empty())startAnimation();
}
StoryScene::~StoryScene() {
  if (mapArray) {
    delete[] mapArray[0];
    delete[] mapArray;
  }
  delete battleScene;
  for (int i = 0; i < ROLENUMBER; i++) {
    delete roleInfoArray[i];
    roleInfoArray[i] = NULL;
  }
  //    for(int i=0;i<roleViewVec.size();++i)
  //    {
  //        delete roleViewVec[i];
  //    }
  // qDebug()<<objectVec.size();
  //    for(int i=0;i<objectVec.size();++i)
  //    {
  //        delete objectVec[i];
  //    }
  delete[] roleInfoArray;
}
void StoryScene::keyPressEvent(QKeyEvent *event) {
  // qDebug()<<" 1222";
  if (!userControlOn)
    return;
  if (event->key() != key->upKey && event->key() != key->downKey &&
      event->key() != key->leftKey && event->key() != key->rightKey)
    return;
  if (!event->isAutoRepeat()) {
    if (event->key() == key->upKey) {
      onDirectionKeyPress(UP);
    } else if (event->key() == key->downKey) {
      onDirectionKeyPress(DOWN);
    } else if (event->key() == key->leftKey) {
      onDirectionKeyPress(LEFT);
    } else if (event->key() == key->rightKey) {
      onDirectionKeyPress(RIGHT);
    }
  }
}
void StoryScene::keyReleaseEvent(QKeyEvent *event) {
  //#ifndef NDEBUG
  //    if(event->key()==Qt::Key_Q&&currentScriptIndex==0)
  //    {
  //        this->startAnimation();
  //    }
  //#endif
  if (!userControlOn)
    return;
  if (!event->isAutoRepeat()) {
    if (event->key() == key->actionKey) {
      onActionKey();
    } else if (event->key() == key->cancelKey) {
      onCancelKey();
    } else if (event->key() == key->menuKey) {
      if (!this->hasFocus())
        return;
      onMenuKey();
    } else if (event->key() == key->upKey) {
      onDirectionKeyRelease(UP);
      // upKeyPressed=false;
    } else if (event->key() == key->downKey) {
      onDirectionKeyRelease(DOWN);
      // downKeyPressed=false;
    } else if (event->key() == key->leftKey) {
      onDirectionKeyRelease(LEFT);
      // leftKeyPressed=false;
    } else if (event->key() == key->rightKey) {
      onDirectionKeyRelease(RIGHT);
      // rightKeyPressed=false;
    }

    //        if(upKeyPressed)focusDirCache=UP;
    //        else if(downKeyPressed)focusDirCache=DOWN;
    //        else if(leftKeyPressed)focusDirCache=LEFT;
    //        else if(rightKeyPressed)focusDirCache=RIGHT;
    //        else
    //        {
    //            directionKeyPressed=false;
    //        }
  }
}
void StoryScene::timerEvent(QTimerEvent *event) {
  if (event->timerId() == viewMoveTimerID) {
    move();
  } else if (event->timerId() == viewDelayTimerID) {
    // assert(storyAnimationOn);
    killTimer(viewDelayTimerID);
    viewDelayTimerID = 0;
    startAnimation();
  } else if (event->timerId() == viewAnimationMoveTimerID) {
    viewMove(NOTMOVING, 0, 0);
  } else if (event->timerId() == autoRecoverTimerID) {
    if (roleInfoArray)
      for (int i = 0; i < ROLENUMBER; ++i) {
        if (roleInfoArray[i] &&
            (roleInfoArray[i]->HP) < (roleInfoArray[i]->HPMax)) {
          roleInfoArray[i]->HP += (roleInfoArray[i]->HPMax * AUTOHPRECOVER);
        }
      }
  }
  //    else if(event->timerId()==fadeTimerID)
  //    {
  //        if(currentAlpha<10)
  //        {
  //            currentAlpha=0;
  //            killTimer(FADEINTERVAL);
  //            fadeTimerID=0;
  //        }
  //        else currentAlpha-=10;
  //    }
  this->update();
}
void StoryScene::move() {
  assert(focusRole);
  int focusRolePointX = focusRole->getPointX();
  int focusRolePointY = focusRole->getPointY();
  // qDebug()<<focusRolePointX<<" "<<focusRolePointY;

  if ((currentPointX % BASEWIDTH == 0 && currentPointY % BASEHEIGHT == 0 &&
       viewIsMoving) ||
      (focusRolePointX % BASEWIDTH == 0 && focusRolePointY % BASEHEIGHT == 0 &&
       !viewIsMoving)) {
    if (focusDir != focusDirCache) {
      // qDebug()<<focusRolePointX<<"
      // "<<focusRolePointY<<"dircache:"<<focusDirCache<<"dir"<<focusDir;
      // swap(focusDir,focusDirCache);
      focusDir = focusDirCache;
      focusRole->changeDir(focusDir);
    }
    //交换dic和dircache
    // viewmoving状态和rolemoving状态一定是在完整的block中进行的

    if ((!upKeyPressed && !downKeyPressed && !leftKeyPressed &&
         !rightKeyPressed) ||
        !userControlOn) {
      // qDebug()<<focusRolePointX<<" "<<focusRolePointY;
      // if(viewMoveTimerID)
      assert(viewMoveTimerID);
      killTimer(viewMoveTimerID);
      viewMoveTimerID = 0;
      focusRole->stopAnimation();
      return;
    }

    if (!userControlOn) {
      assert(viewMoveTimerID);
      killTimer(viewMoveTimerID);
      viewMoveTimerID = 0;
      focusRole->stopAnimation();
      return;
    }

    //***********************检查矩阵状态，决定下一步的行为********************************
    // GlobalXY是hero在整个地图中的矩阵坐标
    // qDebug()<<keyControlOn<<" "<<focusRoleGlobalX<<" "<<focusRoleGlobalY<<"
    // "<<mapArray[focusRoleGlobalX][focusRoleGlobalY];
    // assert(mapArray[focusRoleGlobalY][focusRoleGlobalX]==0);
    // if(processNextArrayElement(focusRolePointX,focusRolePointY,focusDir))return;
    //检查下一个矩阵元素状态,如果可以直接响应(例如僵尸的战斗)就直接触发,否则意味着下一个方格不可走,函数直接返回
    int nextX = 0, nextY = 0;
    int nextElement = processNextArrayElement(focusRolePointX, focusRolePointY,
                                              focusDir, true, nextX, nextY);

    // qDebug()<<focusRolePointX<<" "<<focusRolePointY<<" "<<nextElement;
    // qDebug()<<(static_cast<int>(nextElement&0xff)==static_cast<int>(StoryMapNPC));
    switch (nextElement & 0xff) {
    case static_cast<int>(StoryMapNPC):
      qDebug() << ((nextElement & 0xff00) >> 8);
      if (roleViewVec[(nextElement & 0xff00) >> 8]->canBeContactedDirectly()) {
        targetRole = roleViewVec[(nextElement & 0xff00) >> 8];
        targetRole->onContact(scriptVec, focusDir);
        this->startAnimation();
      }
      return;
      //        case static_cast<int>(StoryMapObject):
      //        case static_cast<int>(StoryMapArticle):
      //// if(objectVec[(nextElement&0xff00)>>8]->canBeContactedDirectly()) / {
      //// objectVec[(nextElement&0xff00)>>8]->onContact(scriptVec); /
      ///this->startAnimation(); /            }
      //            return;
    case static_cast<int>(StoryMapExit): {
      int nextMapID = 0, nextHeroX = 0, nextHeroY = 0;
      DBIO dbio;
      if (!dbio.getExitInfoFromLib(mapID, nextX, nextY, nextMapID, nextHeroX,
                                   nextHeroY))
        quitApp(ERRORGETEXITINFOFAIL);
      this->loadScene(storyID, nextMapID, nextHeroX, nextHeroY);
    }
      return;
    case 0:
      break;
    default:
      return;
    }
    //***********************检查矩阵状态，决定下一步的行为********************************

    //**************判定接下来是主角移动还是视图移动
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
        } else
          viewIsMoving = true;
      } else {
        if ((focusDir == LEFT && currentPointX > 0) ||
            (focusDir == RIGHT && currentPointX < mapSizePointX - WINDOWWIDTH))
          viewIsMoving = true;
        else
          viewIsMoving = false;
      }
    } else if (focusRolePointY - currentPointY ==
               (static_cast<int>(WINDOWHEIGHT / BASEHEIGHT / 2) * BASEHEIGHT)) {
      if ((focusDir == UP && currentPointY > 0) ||
          (focusDir == DOWN && currentPointY < mapSizePointY - WINDOWHEIGHT))
        viewIsMoving = true;
      else
        viewIsMoving = false;
    } else
      viewIsMoving = false;
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
  // qDebug()<<focusRolePointX<<" "<<focusRolePointY;
}
void StoryScene::viewMove(
    DIRECTION dir, int focusRolePointX,
    int focusRolePointY) { // dir为NOTMOVING的时候是过场动画下的场景移动
  switch (dir) {
  case UP:
    currentPointY -= MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()))
      focusRole->setPointXY(focusRolePointX, focusRolePointY - MAPMOVEPIXEL);
    break;
  case DOWN:
    currentPointY += MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()))
      focusRole->setPointXY(focusRolePointX, focusRolePointY + MAPMOVEPIXEL);
    break;
  case LEFT:
    currentPointX -= MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()))
      focusRole->setPointXY(focusRolePointX - MAPMOVEPIXEL, focusRolePointY);
    break;
  case RIGHT:
    currentPointX += MAPMOVEPIXEL;
    if (focusRole && !(focusRole->getReactOnViewMove()))
      focusRole->setPointXY(focusRolePointX + MAPMOVEPIXEL, focusRolePointY);
    break;
  default:
    // qDebug()<<animationEndX<<" "<<animationEndY;
    assert(animationEndX >= 0 && animationEndY >= 0);
    int dis = sqrt(
        static_cast<double>((currentPointX - animationEndX * BASEWIDTH) *
                                (currentPointX - animationEndX * BASEWIDTH) +
                            (currentPointY - animationEndY * BASEHEIGHT) *
                                (currentPointY - animationEndY * BASEHEIGHT)));
    if (dis < ANIMATIONMOVEPIXEL) {
      assert(viewAnimationMoveTimerID);
      killTimer(viewAnimationMoveTimerID);
      viewAnimationMoveTimerID = 0;
      currentPointX = animationEndX * BASEWIDTH;
      currentPointY = animationEndY * BASEHEIGHT;
      if (focusRole) {
        focusRole->onViewMovedTo(currentPointX, currentPointY);
        focusRole->setReactOnViewMove(false);
      }
      startAnimation();
    } else {
      currentPointX += (currentPointX > (animationEndX * BASEWIDTH) ? -1 : 1) *
                       abs(currentPointX - (animationEndX * BASEWIDTH)) *
                       ANIMATIONMOVEPIXEL / dis;
      currentPointY += (currentPointY > (animationEndY * BASEHEIGHT) ? -1 : 1) *
                       abs(currentPointY - (animationEndY * BASEHEIGHT)) *
                       ANIMATIONMOVEPIXEL / dis;
    }
    break;
  }

  // qDebug()<<currentPointX<<" "<<currentPointY;
  emit viewHasMovedToPoint(currentPointX, currentPointY);
}
void StoryScene::focusRoleMove(DIRECTION dir, int focusRolePointX,
                               int focusRolePointY) {
  // qDebug()<<focusRolePointX<<" "<<focusRolePointY;
  switch (dir) {
  case UP:
    // if(focusRolePointY!=0)
    focusRole->moveToPointOfMap(focusRolePointX,
                                focusRolePointY - MAPMOVEPIXEL);
    break;
  case DOWN:
    // if(focusRolePointY!=WINDOWHEIGHT-BASEHEIGHT)
    focusRole->moveToPointOfMap(focusRolePointX,
                                focusRolePointY + MAPMOVEPIXEL);
    break;
  case LEFT:
    // if(focusRolePointX!=0)
    focusRole->moveToPointOfMap(focusRolePointX - MAPMOVEPIXEL,
                                focusRolePointY);
    break;
  case RIGHT:
    // if(focusRolePointX!=WINDOWWIDTH-BASEWIDTH)
    focusRole->moveToPointOfMap(focusRolePointX + MAPMOVEPIXEL,
                                focusRolePointY);
    break;
  default:
    break;
  }
}
void StoryScene::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  //    if(fadeTimerID!=0)
  //    {
  //        painter.setBrush(QBrush(QColor(0,0,0,currentAlpha)));
  //        painter.drawRect(this->rect());
  //    }
  //    else
  if (!battleScene)
    painter.drawPixmap(
        QPoint(0, 0), map,
        QRect(currentPointX, currentPointY, WINDOWWIDTH, WINDOWHEIGHT));
  //    if(menuOn)
  //    {
  //        painter.setBrush(QBrush(QColor(23,124,176,192)));
  //        painter.setPen(QPen(QBrush(QColor(73,65,102)),2));
  //        painter.setRenderHints(QPainter::Antialiasing
  //                                  | QPainter::HighQualityAntialiasing,
  //                                  true);//反锯齿
  //        painter.drawRoundedRect((WINDOWWIDTH-MENUWIDTH)/2,(WINDOWHEIGHT-MENUHEIGHT)/2,MENUWIDTH,MENUHEIGHT,5,5);
  //    }
}
void StoryScene::onActionKey() {
  assert(focusRole);
  int nextX = 0, nextY = 0;
  uint nextArrayElement = static_cast<uint>(
      processNextArrayElement(focusRole->getPointX(), focusRole->getPointY(),
                              focusDir, false, nextX, nextY));

  switch (nextArrayElement & 0xff) {
  case static_cast<int>(StoryMapNPC):
    targetRole = roleViewVec[(nextArrayElement & 0xff00) >> 8];
    targetRole->onContact(scriptVec, focusDir);
    this->startAnimation();
    break;
  case static_cast<int>(StoryMapArticle): {
    mapArray[nextY][nextX] = (nextArrayElement & 0xff000000) >> 24;

    if (gotArticleSet.find(storyID << 24 | mapID << 16 | nextX << 8 | nextY) !=
        gotArticleSet.end())
      break;
    //找到了,说明之前已经被获取过了
    //否则获取该物品
    int articleID = (nextArrayElement & 0xffff00) >> 8;
    carriedArticleIDSet.insert(articleID);

    gotArticleSet.insert(storyID << 24 | mapID << 16 | nextX << 8 | nextY);
    if (articleIDToInfoMap.find(articleID) == articleIDToInfoMap.end()) {
      DBIO dbio;
      if (!dbio.getArticleInfoFromLib(articleID, &articleIDToInfoMap))
        quitApp(ERRORGETARTICLEINFOFAIL);
    }
    scriptVec.clear();
    scriptVec.push_back(Script(AniStart));
    scriptVec.push_back(
        Script(AniViewStartGetGoods,
               QString("获得") + articleIDToInfoMap[articleID].name));
    scriptVec.push_back(Script(AniEnd));
    this->startAnimation();
  } break;
  case static_cast<int>(StoryMapCash): {
    mapArray[nextY][nextX] = (nextArrayElement & 0xff000000) >> 24;
    if (gotArticleSet.find(storyID << 24 | mapID << 16 | nextX << 8 | nextY) !=
        gotArticleSet.end())
      break;
    //找到了,说明之前已经被获取过了
    //否则获取该物品

    int cash = (nextArrayElement & 0xffff00) >> 8;
    carriedCash += cash;

    gotArticleSet.insert(storyID << 24 | mapID << 16 | nextX << 8 | nextY);
    scriptVec.clear();
    scriptVec.push_back(Script(AniStart));
    scriptVec.push_back(
        Script(AniViewStartGetGoods, QString("获得%1金钱").arg(cash)));
    scriptVec.push_back(Script(AniEnd));
    this->startAnimation();
  } break;
  case 1:
  case 0:
  case 6:
    break;
  default:
    qDebug() << (nextArrayElement & 0xff);
    assert(0);
  }
  //    else if((nextArrayElement&0xff)==static_cast<uint>(StoryMapArticle))
  //    {
  //        objectVec[(nextArrayElement&0xff00)>>8]->onContact(scriptVec);
  //    }
  //    else if((nextArrayElement&0xff)==static_cast<uint>(StoryMapArticle))
  //    {

  //        gotArticleSet.insert(articleID);
  //        if(articleID>10000)
  //            articleID-=(articleID%10000);
  //        carriedArticleIDSet.insert(articleID);
  //        mapArray[nextY][nextX]>>=24;
  //    }

  // this->startAnimation();
  this->update();
}
void StoryScene::onCancelKey() { this->update(); }
void StoryScene::onMenuKey() {
  MenuWidget *w = new MenuWidget(this, key, StoryMenu);
  w->setFocus();
  connect(w, SIGNAL(selectIndex(int)), this, SLOT(onMenuSelect(int)));
  connect(this, SIGNAL(animationStart()), w, SLOT(close()));
  this->update();
}
void StoryScene::onDirectionKeyPress(DIRECTION dir) {
  // qDebug()<<dir;
  switch (dir) {
  case UP:
    upKeyPressed = true;
    break;
  case DOWN:
    downKeyPressed = true;
    break;
  case LEFT:
    leftKeyPressed = true;
    break;
  case RIGHT:
    rightKeyPressed = true;
    break;
  default:
    break;
  }
  if (viewMoveTimerID == 0) {
    focusDir = dir;
    focusDirCache = dir;
    focusRole->startAnimation(focusDir);
    viewMoveTimerID = startTimer(VIEWMOVEINTERVAL);
    move();
  } else
    focusDirCache = dir;
}
void StoryScene::onDirectionKeyRelease(DIRECTION dir) {

  switch (dir) {
  case UP:
    upKeyPressed = false;
    break;
  case DOWN:
    downKeyPressed = false;
    break;
  case LEFT:
    leftKeyPressed = false;
    break;
  case RIGHT:
    rightKeyPressed = false;
    break;
  default:
    break;
  }
  if (upKeyPressed)
    focusDirCache = UP;
  else if (downKeyPressed)
    focusDirCache = DOWN;
  else if (leftKeyPressed)
    focusDirCache = LEFT;
  else if (rightKeyPressed)
    focusDirCache = RIGHT;
}
// void StoryScene::startGetGoods(const QString &name)
//{
//    //qDebug()<<name;
//    GetGoodsLabel *label=new GetGoodsLabel(this,name);
//    label->show();
//    label->setFocus();
//}
void StoryScene::startAnimation() {
  if (scriptVec.empty())
    return;
  Script *pScript = &scriptVec[currentScriptIndex];
  int x = 0;
  int y = 0;
#ifndef NDEBUG
  qDebug() << "currentIndex:" << currentScriptIndex << " "
           << scriptVec[currentScriptIndex].op << " "
           << scriptVec[currentScriptIndex].param;
#endif
  currentScriptIndex++;
  switch (pScript->op) {
  case AniStart:
    userControlOn = false;
    emit animationStart();
    this->startAnimation();
    break;
  case AniViewMoveTo:
#ifdef IGNOREANIVIEWMOVE
    qDebug() << "viewmove ignored.";
    this->startAnimation();
    break;
#endif
    //参数是中心点坐标,首先换算成左上角坐标
    if (focusRole)
      focusRole->setReactOnViewMove(true);
    animationEndX = getSubFromQString(pScript->param, 1, '#')
                        .toInt(); //-WINDOWWIDTH/BASEWIDTH/2;
    animationEndY = getSubFromQString(pScript->param, 2, '#')
                        .toInt(); //-WINDOWHEIGHT/BASEHEIGHT/2;
    if (animationEndX == -1) { //如果是-1的话移动目标点是focusRole横纵坐标各-7
      animationEndX = (focusRole->getMapX());
      animationEndY = (focusRole->getMapY());
    }
    animationEndX -= WINDOWWIDTH / BASEWIDTH / 2;
    animationEndY -= WINDOWHEIGHT / BASEHEIGHT / 2;
    if (animationEndX < 0)
      animationEndX = 0;
    if (animationEndX > (mapSizePointX - WINDOWWIDTH) / BASEWIDTH)
      animationEndX = (mapSizePointX - WINDOWWIDTH) / BASEWIDTH;
    if (animationEndY < 0)
      animationEndY = 0;
    if (animationEndY > (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT)
      animationEndY = (mapSizePointY - WINDOWHEIGHT) / BASEHEIGHT;
    // qDebug()<<animationEndX<<" "<<animationEndY;
    viewAnimationMoveTimerID = startTimer(VIEWMOVEINTERVAL);
    break;
  case AniRoleSetPath:
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    roleViewVec[(static_cast<uint>(mapArray[y][x] & 0xff00)) >> 8]->setPathVec(
        pScript->param);
    this->startAnimation();
    break;
  case AniRoleMove:
    //这个后面要跟一个延时
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    if (x == -1) {
      targetRole->rewalk();
    } else {
      roleViewVec[(static_cast<uint>(mapArray[y][x] & 0xff00)) >> 8]
          ->startMoving(false);
      while (cutHeadFromQString(pScript->param, 2, '#').length() != 0) {
        x = getSubFromQString(pScript->param, 1, '#').toInt();
        y = getSubFromQString(pScript->param, 2, '#').toInt();
        roleViewVec[(static_cast<uint>(mapArray[y][x] & 0xff00)) >> 8]
            ->startMoving(false);
      }
    }
    this->startAnimation();
    break;
  case AniRoleFastMove:
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    roleViewVec[(static_cast<uint>(mapArray[y][x] & 0xff00)) >> 8]->startMoving(
        true);
    while (cutHeadFromQString(pScript->param, 2, '#').length() != 0) {
      x = getSubFromQString(pScript->param, 1, '#').toInt();
      y = getSubFromQString(pScript->param, 2, '#').toInt();
      roleViewVec[(static_cast<uint>(mapArray[y][x] & 0xff00)) >> 8]
          ->startMoving(true);
    }
    this->startAnimation();
    break;
  case AniRoleSweat:
#ifdef IGNORESWEAT
    qDebug() << "sweat ignored.";
    this->startAnimation();
    break;
#endif
  case AniRoleAttention:
#ifdef IGNOREATTENTION
    qDebug() << "attention ignored.";
    this->startAnimation();
    break;
#endif
  case AniRoleAttack:
  case AniRoleDie:
  case AniRoleLevelUp:
  case AniRoleChangeJob:

  {
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    RoleAnimation *animation =
        new RoleAnimation(this, x * BASEWIDTH - currentPointX,
                          y * BASEHEIGHT - currentPointY, pScript->op);
    animation->start(true);
    //        qDebug()<<"next index:"<<currentScriptIndex;
    //        qDebug()<<scriptVec[currentScriptIndex].op;
    //        qDebug()<<scriptVec[currentScriptIndex].param;
    //        connect(roleViewVec[(static_cast<uint>(mapArray[y][x]&0xff00))>>8],
    //                SIGNAL(roleAnimationEnd()),this,SLOT(startAnimation()));
    // roleViewVec[(static_cast<uint>(mapArray[y][x]&0xff00))>>8]->sweat(true);
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
  case AniRoleFlash:
    // do
    {
      x = getSubFromQString(pScript->param, 1, '#').toInt();
      y = getSubFromQString(pScript->param, 2, '#').toInt();
      assert((mapArray[y][x] & 0xff) == static_cast<int>(StoryMapNPC) ||
             (mapArray[y][x] & 0xff) == static_cast<int>(StoryMapHero));
      roleViewVec[(mapArray[y][x] & 0xff00) >> 8]->flash();
    }
    break;
    // while(cutHeadFromQString(pScript->param,2,'#').length()!=0);
  case AniRoleDodge:
    x = getSubFromQString(pScript->param, 1, '#').toInt();
    y = getSubFromQString(pScript->param, 2, '#').toInt();
    roleViewVec[(static_cast<uint>(mapArray[y][x] & 0xff00)) >> 8]->dodge(true);
    this->startAnimation();
    break;
  case AniViewStartDialog:
#ifdef IGNOREANIDIALOG
    qDebug() << "dialog ignored.";
    this->startAnimation();
    break;
#endif
    {
      DialogLabel *label = new DialogLabel(this, pScript->param);
      connect(label, SIGNAL(destroyed()), this, SLOT(startAnimation()));
    }
    break;
  case AniViewStartDelay:
#ifdef IGNOREANIDELAY
    this->startAnimation();
    break;
#endif
    viewDelayTimerID = startTimer(pScript->param.toInt());
    break;
  case AniStartBattle: {
    currentBattleID = pScript->param.toInt();
    BattleRoleSelectScene *roleSelectScene =
        new BattleRoleSelectScene(this, key, roleInfoArray, currentBattleID);

    connect(roleSelectScene, SIGNAL(startBattle(vector<int>)), this,
            SLOT(startBattle(vector<int>)));
  }

  // this->startAnimation();
  break;
    //    case AniSwitchScene:
    //        loadScene(getSubFromQString(pScript->param,1,'#').toInt(),
    //                  getSubFromQString(pScript->param,2,'#').toInt(),
    //                  getSubFromQString(pScript->param,3,'#').toInt(),
    //                  getSubFromQString(pScript->param,4,'#').toInt());
    //        //装载新场景后所有的动画就由新场景负责,因此这个应该是scriptVec的最后一条,它之后的script将不会被执行
    //        //this->startAnimation();
    //            break;
  case AniViewStartGetGoods: {
    GetGoodsLabel *label = new GetGoodsLabel(this, pScript->param);
    connect(label, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  } break;
  case AniRoleDisappear:
    do {
      x = getSubFromQString(pScript->param, 1, '#').toInt();
      y = getSubFromQString(pScript->param, 2, '#').toInt();
      this->onRoleDisappear(x, y);
      /*
      int index=(static_cast<int>(mapArray[y][x]&0xff00))>>8;
      roleViewVec[index]->close();
      for(int i=index;i<roleViewVec.size()-1;++i)
      {
          roleViewVec[i]=roleViewVec[i+1];
          mapArray[roleViewVec[i]->getMapY()][roleViewVec[i]->getMapX()]-=0x0100;
      }
      roleViewVec.pop_back();
      mapArray[y][x]>>=16;//原先的地图元素恢复到地图上
      */
    } while (cutHeadFromQString(pScript->param, 2, '#').length() != 0);
    this->startAnimation();
    break;
  case AniStartTrade: {
    TradeWidget *w =
        new TradeWidget(this, key, carriedArticleIDSet, pScript->param,
                        roleInfoArray, &carriedCash, articleIDToInfoMap);
    connect(w, SIGNAL(destroyed()), this, SLOT(startAnimation()));
    break;
  }
  case AniAddRole: {
    DBIO dbio;
    if (!dbio.getRoleInfoFromLibOfID(roleInfoArray, pScript->param.toInt()))
      quitApp(ERRORGETROLEINFOFAIL);
    this->startAnimation();
    break;
  }
  case AniSetStoryIDTo:
    storyID = pScript->param.toInt();
    this->startAnimation();
    break;
  case AniSetSideStoryIDTo:
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    sideStoryID = pScript->param.toInt();
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    this->startAnimation();
    break;
  case AniSetSideStoryOn:
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    assert(((sideStoryID & 0xffff00) >> 8) == 0); //此时应该未打开
    sideStoryID |= 0x000100;
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    this->startAnimation();
    break;
  case AniSetSideStoryOff:
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    assert(((sideStoryID & 0xffff00) >> 8) == 0x0101); //此时应该已完成
    sideStoryID &= 0xff00ff;
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    this->startAnimation();
    break;
  case AniSetSideStoryOK:
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    // qDebug()<< ((sideStoryID&0xffff00)>>8);
    assert(((sideStoryID & 0xffff00) >> 8) == 0x0001); //此时应该未完成
    sideStoryID |= 0x010000;
    qDebug() << "sidestoryid:" << hex << sideStoryID;
    this->startAnimation();
    break;
  case AniGetCash:
    carriedCash += (pScript->param.toInt());
    this->startAnimation();
    break;
  case AniGetArticle:
    carriedArticleIDSet.insert(pScript->param.toInt());
    this->startAnimation();
    break;
  case AniQuitToMainMenu:
    this->onMenuSelect(4);
    break;
  case AniMultiTalkFlag:
    assert(0);
    break;
  case AniEnd:
    userControlOn = true;
    scriptVec.clear();
    // scriptVec.swap(vector<Script>());
    // qDebug()<<scriptVec.empty();
    currentScriptIndex = 0;
    this->setFocus();
    break;
  default:
    break;
  }
}
int StoryScene::processNextArrayElement(
    int rolePointX, int rolePointY, DIRECTION dir, bool swapOrNot,
    int &nextMapX,
    int &
        nextMapY) { //有这一步检测就不需要move里面的边缘检测,x指视图的横坐标，但是是矩阵的第二维坐标，y是矩阵的一维坐标
  //返回3个值:下一个矩阵元素,下一个矩阵元素的xy坐标,如果已经在边缘就返回边缘元素的坐标
  // qDebug()<<currentPointY<<" "<<rolePointY;
  int x = (rolePointX) / BASEWIDTH;
  int y = (rolePointY) / BASEHEIGHT;
  nextMapX = x;
  nextMapY = y;

  switch (dir) {
  case UP: //在视图上向上移动相当于矩阵的一维坐标值-1，因此是y-1
    if (y == 0)
      return 1;
    else {
      nextMapY -= 1;
      if (mapArray[y - 1][x] != 0)
        return mapArray[y - 1][x];
      else if (swapOrNot)
        swap(mapArray[y][x], mapArray[y - 1][x]);
      //若下一个元素可以行走并且仍在运动中则交换自己和下一个矩阵元素
    }
    break;
  case DOWN:
    if (y == mapSizeY - 1)
      return 1;
    else {
      nextMapY += 1;
      if (mapArray[y + 1][x] != 0)
        return mapArray[y + 1][x];
      else if (swapOrNot)
        swap(mapArray[y][x], mapArray[y + 1][x]);
    }
    break;
  case LEFT:
    if (x == 0)
      return 1;
    else {
      nextMapX -= 1;
      if (mapArray[y][x - 1] != 0)
        return mapArray[y][x - 1];
      else if (swapOrNot)
        swap(mapArray[y][x], mapArray[y][x - 1]);
    }
    break;
  case RIGHT:
    if (x == mapSizeX - 1)
      return 1;
    else {
      nextMapX += 1;
      if (mapArray[y][x + 1] != 0)
        return mapArray[y][x + 1];
      else if (swapOrNot)
        swap(mapArray[y][x], mapArray[y][x + 1]);
    }
    break;
  default:
    qDebug() << focusDir;
    assert(0);
    break;
  }
  return 0;
}
void StoryScene::onMenuSelect(int index) {
  switch (index) {
  case 0: //状态
  {
    StatusWidget *w =
        new StatusWidget(this, roleInfoArray, key, carriedArticleIDSet,
                         articleIDToInfoMap, carriedCash);
    // qDebug()<<sizeof(StoryScene);
    // RoleInfoWidget *w=new RoleInfoWidget(this,roleInfoArray);
  } break;
  case 1: //存档
  {
    SaveLoadWidget *w = new SaveLoadWidget(this, key, OperationSave);
    // w->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT*2);
    connect(w, SIGNAL(saveToFile()), this, SLOT(onSaveData()));
  } break;
  case 2: //读档
  {
    SaveLoadWidget *w = new SaveLoadWidget(this, key, OperationLoad);
    // w->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT*2);
    connect(w, SIGNAL(loadFrom(QString)), this, SLOT(loadScene(QString)));
  } break;
  case 3: //回主菜单
    this->close();
    emit goBackToMainMenu();
    break;
  default:
    break;
  }
}
void StoryScene::onSaveData() {
  QString currentTimeString =
      QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
  QString saveFilePathName = QApplication::applicationDirPath() + "/save/" +
                             currentTimeString + ".sav";
  QDir saveFile(saveFilePathName);
  if (saveFile.exists()) {
    QMessageBox::warning(this, "凌工路2号", "存档失败,请重试", QMessageBox::Ok);
    return;
  }

  DBIO dbio;

  StorySceneData SelfData(currentPointX, currentPointY, storyID, sideStoryID,
                          mapID, currentBattleID, carriedCash);
  /*//暂时不存script,这段代码注释备用
  uint scriptVecDataSize=0;
  for(int i=0;i<scriptVec.size();++i)
  {
      scriptVecDataSize+=(sizeof(AnimationOperation)+scriptVec[i].param.toUtf8().size()+1);
  }
  //Script段开头4个字节存放script的数量，然后每个script由开头的op（N个字节,N=sizeof(AnimationOperation)）和param（C风格字符串）
  //uint dataSize=sizeof(StorySceneData)+sizeof(int)+scriptVecDataSize;

  uchar *pScriptData=new uchar[scriptVecDataSize];//准备将Script序列化
  uchar *currentPointer=pScriptData;

  *(reinterpret_cast<int *>(currentPointer))=scriptVec.size();
  currentPointer+=sizeof(int);//复制script数量

  for(int i=0;i<scriptVec.size();++i)
  {
      *(reinterpret_cast<AnimationOperation*>(currentPointer))=scriptVec[i].op;//复制op数据

      QByteArray scriptParamData=scriptVec[i].param.toUtf8();
      memcpy(currentPointer+sizeof(AnimationOperation),
             scriptParamData.data(),
             scriptParamData.size()+1);//复制param数据
      currentPointer+=(sizeof(AnimationOperation)+scriptParamData.size()+1);
  }

  assert(pScriptData+scriptVecDataSize==currentPointer);//此时currentPointer应该指向申请内存区域的最后一个位置
  */

  if (!dbio.createSaveFile(saveFilePathName, currentTimeString,
                           CURRENTVERSION)) {
    QMessageBox::warning(this, "凌工路2号", "存档失败,请重试", QMessageBox::Ok);
    return;
  }

  if (!dbio.saveStorySceneToFile(SelfData, roleViewVec, gotArticleSet,
                                 carriedArticleIDSet, roleInfoArray,
                                 mapArray[0], mapSizeX * mapSizeY)) {
    QMessageBox::warning(this, "凌工路2号", "存档失败,请重试",
                         QMessageBox::Ok); //这里应该给出存档失败的提示
    return;
  }
  //完成了storyScene的归档工作
  if (battleScene) {
    if (!battleScene->saveDataTo(dbio)) {
      QMessageBox::warning(this, "凌工路2号", "存档失败,请重试",
                           QMessageBox::Ok);
      return;
    }
    battleScene->setFocus();
  } else {
    this->setFocus();
  }
  QMessageBox::warning(this, "凌工路2号", "存档成功！", QMessageBox::Ok);
  //给出成功提示
  //存档全部完成
}
void StoryScene::startBattle(vector<int> selectedRoleID) {
  if (viewMoveTimerID) {
    killTimer(viewMoveTimerID);
    viewMoveTimerID = 0;
  }
  if (autoRecoverTimerID) {
    killTimer(autoRecoverTimerID);
    autoRecoverTimerID = 0;
  }
  upKeyPressed = false;
  downKeyPressed = false;
  leftKeyPressed = false;
  rightKeyPressed = false;
  //    scriptVec.swap(vector<Script>());
  //    //qDebug()<<scriptVec.empty();
  //    currentScriptIndex=0;
  //清理工作

  for (int i = 0; i < roleViewVec.size(); ++i)
    roleViewVec[i]->hide();
  battleScene = new BattleScene(this, currentBattleID, key, selectedRoleID,
                                roleInfoArray, articleIDToInfoMap);
  connect(battleScene, SIGNAL(battleFail()), this, SLOT(onBattleFail()));
  connect(battleScene, SIGNAL(battleWin()), this, SLOT(onBattleWin()));
}
void StoryScene::onBattleFail() {
  assert(autoRecoverTimerID == 0);
  autoRecoverTimerID = startTimer(AUTORECOVERINTERVAL);
  // battleScene->close();
  userControlOn = true;
  battleScene = NULL; //他会自己销毁自己的

  scriptVec.clear();
  currentScriptIndex = 0;
  DBIO dbio;
  if (!dbio.getBattleFailScriptFromLib(currentBattleID, scriptVec))
    quitApp(ERRORGERBATTLEFAILSCRIPTFAIL);
  currentBattleID = 0;

  FadeScene *fade = new FadeScene(this);
  connect(fade, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  fade->raise();
  this->show();
  for (int i = 0; i < roleViewVec.size(); ++i) {
    // if(targetRole!=roleViewVec[i])
    roleViewVec[i]->show();
  }

  //    this->setFocus();
  // this->setFocus();
}
void StoryScene::onBattleWin() {
  assert(autoRecoverTimerID == 0);
  autoRecoverTimerID = startTimer(AUTORECOVERINTERVAL);

  userControlOn = true;
  battleScene = NULL;

  scriptVec.clear();
  currentScriptIndex = 0;
  DBIO dbio;
  if (!dbio.getBattleWinScriptFromLib(currentBattleID, scriptVec))
    quitApp(ERRORGERBATTLEWINSCRIPTFAIL);
  currentBattleID = 0;

  FadeScene *fade = new FadeScene(this);
  connect(fade, SIGNAL(destroyed()), this, SLOT(startAnimation()));
  fade->raise();
  this->show();
  for (int i = 0; i < roleViewVec.size(); ++i) {
    // if(targetRole!=roleViewVec[i])
    roleViewVec[i]->show();
  }
  //    this->setFocus();
}
void StoryScene::onRoleDisappear(int x, int y) {
  if (x == -1) {
    x = targetRole->getMapX();
    y = targetRole->getMapY();
  }

  int index = ((mapArray[y][x] & 0xff00)) >> 8;
  roleViewVec[index]->close();
  for (int i = index; i < roleViewVec.size() - 1; ++i) {
    roleViewVec[i] = roleViewVec[i + 1];
    assert((mapArray[roleViewVec[i]->getMapY()][roleViewVec[i]->getMapX()] &
            0xff) == static_cast<int>(StoryMapNPC));
    // qDebug()<<(roleViewVec[i]->getMapX())<<(roleViewVec[i]->getMapY());
    //        qDebug()<<hex<<(mapArray[roleViewVec[i]->getMapY()][roleViewVec[i]->getMapX()]);
    //        qDebug()<<((mapArray[roleViewVec[i]->getMapY()][roleViewVec[i]->getMapX()]
    //                    &0xff)==static_cast<int>(StoryMapNPC));
    mapArray[roleViewVec[i]->getMapY()][roleViewVec[i]->getMapX()] -= 0x0100;
    // qDebug()<<hex<<(mapArray[roleViewVec[i]->getMapY()][roleViewVec[i]->getMapX()]);
  }
  roleViewVec.pop_back();
  mapArray[y][x] >>= 16; //原先的地图元素恢复到地图上
}
void StoryScene::focusOutEvent(QFocusEvent *) {
  upKeyPressed = false;
  downKeyPressed = false;
  leftKeyPressed = false;
  rightKeyPressed = false;
}
