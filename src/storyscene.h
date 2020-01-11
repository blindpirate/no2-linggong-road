/**************************
  继承自QWidget,用于处理情节场景.storyscene与battlescene不同,它是始终存在的,而battle是完成后即被销毁的.
  从存档中读取出battlescene的时候也是先创建storyscene然后再创建battlescene
**************************/
#ifndef STORYSCENE_H
#define STORYSCENE_H

#include "lgl.h"
#include "dbio.h"
#include "roleview.h"
//#include "invisibleobject.h"
#include "role.h"
#include "battlescene.h"
#include "saveloadwidget.h"
#include "fadescene.h"

#define AUTOHPRECOVER 0.05
#define AUTORECOVERINTERVAL 60000//每分钟回复5%的HP
#define FADEINTERVAL 40



class StoryScene:public QWidget
{
    Q_OBJECT
public:
    explicit StoryScene(QWidget *parent,KeyboardControl *key,bool gameInit);
    //建立storyscene.最后一个参数为真时是"开始游戏",为假时是"载入储存游戏"
    ~StoryScene();
public slots:
    void loadScene(int storyID,int mapID,int heroX,int heroY);//载入后视角左上角在整个map中的坐标
    //按照默认的设置装载场景,在初始化和场景切换的时候使用
    void loadScene(QString const &fileName);
    //从存档文件装载场景,因为存档和初始化场景的差异,所以必须使用这个方法装载
    //void startConversation(QString);
    //void startGetGoods(QString const &name);
    void startAnimation();
    void onMenuSelect(int index);
    void startBattle(vector<int> selectedRoleID);
    void onBattleWin();
    void onBattleFail();
    void onRoleDisappear(int x,int y);

//    void changeUpKeyTo(int newUpKey){currentUpKey=newUpKey;}
//    void changeDownKeyTo(int newDownKey){currentDownKey=newDownKey;}
//    void changeLeftKeyTo(int newLeftKey){currentLeftKey=newLeftKey;}
//    void changeRightKeyTo(int newRightKey){currentRightKey=newRightKey;}
//    void changeActionKeyTo(int newActionKey){currentActionKey=newActionKey;}
//    void changeCancelKeyTo(int newCancelKey){currentCancelKey=newCancelKey;}
//    void changeMenuKeyTo(int newMenuKey){currentMenuKey=newMenuKey;}
    void onSaveData();
signals:
    void viewHasMovedToPoint(int x,int y);
    void battleSceneBuilt(BattleScene *);//战斗场景创建的时候发出此信号,否则无法将其对应到mainwindow中的成员变量
    void battleSceneDestroyed();//战斗场景结束时发出此信号
    void animationStart();//发出此信号令菜单消失
    void goBackToMainMenu();
    //向视图中的所有非focusRole发射,令其在视图中的显示进行相应调整.参数是视图当前左上角像素在map中的位置
private:
    /************************地图元素加载部分***********************/
    //DBIO dbio;
    int mapSizeX;//地图的长宽block数
    int mapSizeY;
    int mapSizePointX;//地图的像素点数
    int mapSizePointY;
    int currentPointX;//视角左上角像素点在整个地图中的坐标
    int currentPointY;
    int storyID;//主线剧情
    int sideStoryID;//支线剧情,右一字节是ID,右二字节是是否打开标志,右三字节是是否完成标志
    //例如,0x000001支线剧情1未打开未完成,0x000101支线剧情1打开未完成,0x010001支线剧情1因完成而关闭,0x010101支线剧情1打开且完成
    int mapID;
    int **mapArray;//地图矩阵,这是整个地图响应的核心
    QPixmap map;
    vector<RoleView *> roleViewVec;//场景绘制在地图上的人物
    //vector<InvisibleObject *>objectVec;//不可见的地图元素
    vector<Script> scriptVec;
    int currentScriptIndex;
    //bool storyAnimationOn;//过场动画开启,此时屏蔽一切键盘响应
    int animationEndX;
    int animationEndY;//动画中场景移动的终点
    int viewDelayTimerID;

    RoleView *focusRole;//某一时刻仅有一个焦点角色，地图可以随着它移动
    RoleView *targetRole;//接触一个敌人时候将其设置成该敌人,然后可以用script令其消失
    BattleScene *battleScene;
    DIRECTION focusDir;
    DIRECTION focusDirCache;
    int viewMoveTimerID;
    int viewAnimationMoveTimerID;//这个定时器是用于view的自动移动用的
    int autoRecoverTimerID;
//    int fadeTimerID;//画面的渐入渐出定时器
//    uchar currentAlpha;
    bool viewIsMoving;//true表示focusrole在地图正中间,视角随着它移动
    /************************键盘响应处理部分***********************/
//    int currentUpKey;
//    int currentDownKey;
//    int currentLeftKey;
//    int currentRightKey;
//    int currentActionKey;
//    int currentCancelKey;
//    int currentMenuKey;
    KeyboardControl *key;

    bool upKeyPressed;
    bool downKeyPressed;
    bool leftKeyPressed;
    bool rightKeyPressed;

    void onDirectionKeyPress(DIRECTION);
    void onDirectionKeyRelease(DIRECTION);
    void onActionKey();
    void onCancelKey();
    void onMenuKey();
    bool userControlOn;
    /**************************全局变量部分**************************/
    Role **roleInfoArray;//指针数组,当前拥有的所有主角的状态,若不拥有某个角色则为NULL,索引值+1=roleID,存在于堆上
    set<uint> gotArticleSet;//角色已经获取的物品集合,此集合与角色身上携带的物品相互独立,存放storyID|mapID|x|y,每个占一个字节
    multiset<int> carriedArticleIDSet;//角色身上现有的物品
    std::map<int,ArticleInfo> articleIDToInfoMap;
    int carriedCash;//携带金钱
    /**************************继承的事件响应函数部分***********************/
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *event);
    void focusOutEvent(QFocusEvent *);
    /**************************用于代码复用的函数部分***********************/
    void move();//move函数用于处理角色或者视图的移动事件
    void focusRoleMove(DIRECTION dir,int focusRolePointX,int focusRolePointY);
    void viewMove(DIRECTION dir,int x,int y);
    int processNextArrayElement(int focusRolePointX,int focusRolePointY,DIRECTION dir,bool swapOrNot,int &nextMapX,int &nextMapY);
    //true时会交换自己与下一个元素,返回下一个元素的全局坐标
    /********菜单相关部分*********/
    /***
      状态
      装备
      存档
      读档
    ****/
    /**************战斗场景****************/
    int currentBattleID;

};

#endif // STORYSCENE_H
