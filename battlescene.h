/**************************
  继承自QWidget,用于处理战斗场景.
**************************/
#ifndef BATTLESCENE_H
#define BATTLESCENE_H
#include "lgl.h"
#include "dbio.h"
#include "role.h"
#include"roleview.h"
#include "battleroleselectscene.h"
//#include "invisibleobject.h"
#include "dialoglabel.h"
#include "battlelabel.h"
#include "saveloadwidget.h"
#include "operationwidget.h"
#include "weaponwidget.h"
#include "cursorwidget.h"
#include "battleaimwidget.h"
#include "menuwidget.h"
#include "descriptionwidget.h"
#include "roleanimation.h"
#include "weapon.h"

typedef enum IfBattleEnd
{
    BattleWin,
    BattleFail,
    BattleNotEnd
}IfBattleEnd;


#define VIEWMOVEINTERVAL 40//值越大移动的越慢
#define MAPMOVEPIXEL 8//每40ms移动8个像素
#define ANIMATIONMOVEPIXEL 30//过场动画中每40ms移动20个像素(连线上)
#define DESCRIPTIONDELAY 1000//超过1000ms不操作可以显示各种菜单的说明


/*
typedef enum RANGETYPE
{
    SELFWALK,
    SELFATTACK,
    ENEMYWALK,
    ENEMYATTACK,
    FRIENDWALK,
    FRIENDATTACK
}RANGETYPE;
*/
class BattleScene:public QWidget
{
    Q_OBJECT
public:
    explicit BattleScene(QWidget *parent,
                         int battleID,KeyboardControl *key,
                         vector<int> &selectedRoleID,
                         Role **roleInfo,
                         std::map<int,ArticleInfo> &articleIDToInfoMap);
    explicit BattleScene(QWidget *parent,
                         DBIO &dbio,
                         KeyboardControl *key,
                         Role **roleInfo,
                         std::map<int,ArticleInfo> &articleIDToInfoMap);
    //roleIDvec是严格的增序
    //explicit BattleScene(QWidget *parent, int battleID, Role **roleInfo);
    //两种构造函数,一种是从存档文件中创建,一种是从传入的参数中创建
    ~BattleScene();
    void loadScene(int battleID,vector<int> &selectedRoleID);
    bool saveDataTo(DBIO &dbio);
signals:
    void viewHasMovedToPoint(int x,int y);

    void battleWin();
    void battleFail();
    void changeActiveStatusTo(bool isActive);
public slots:
//    void changeUpKeyTo(int newUpKey){currentUpKey=newUpKey;}
//    void changeDownKeyTo(int newDownKey){currentDownKey=newDownKey;}
//    void changeLeftKeyTo(int newLeftKey){currentLeftKey=newLeftKey;}
//    void changeRightKeyTo(int newRightKey){currentRightKey=newRightKey;}
//    void changeActionKeyTo(int newActionKey){currentActionKey=newActionKey;}
//    void changeCancelKeyTo(int newCancelKey){currentCancelKey=newCancelKey;}
//    void changeMenuKeyTo(int newMenuKey){currentMenuKey=newMenuKey;}

    //void save();
    void startAnimation();
    //void battleSceneEnd(bool win);
    void fail();
    void win();
    void onMenuSelect(int index);
    void onAIMoved(int pointX,int pointY);

    //void battleSceneBuilt(BattleScene *);//向窗口发送消息使得自己能与选项改变的信号连接

private:
    /************************************************************/
    /************************地图元素加载部分***********************/
    int mapSizeX;//地图的长宽block数
    int mapSizeY;
    int mapSizePointX;//地图的像素点数
    int mapSizePointY;
    int currentPointX;//视角左上角像素点在整个地图中的坐标
    int currentPointY;
    int **mapArray;//地图矩阵,这是整个地图响应的核心
    int focusRoleMapXCache;
    int focusRoleMapYCache;//这两个变量是为了实现角色在walk时若按下取消键返回起点用的,存储的是起点坐标
    //int battleID;
    int viewAutoMoveEndX;
    int viewAutoMoveEndY;
    int battleID;
    int mapID;
    QPixmap map;
    bool cursorOn;//是否显示光标,这个标志位暂时保留,因为似乎所有状态下都要绘制光标
    bool cursorControlOn;//true的时候操作对象是光标,false时是focusRole
    vector<RoleView *>selfViewVec;//场景绘制在地图上的我方人物
    vector<RoleView *>enemyViewVec;//敌人
    vector<RoleView *>friendViewVec;//友军
    BattleDisplayState displayState;//地图显示哪些东西
    RoleView *focusRole;
    RoleView *targetRole;
    Role **roleInfo;
    //Role *currentRole;//当前正在操作的Role指针
    /****************AI处理********************/
    void processEnemyAction();//将敌人动画加入scriptVec
    void processFriendAction();
    void processSelfAction();
    /*******动画处理部分********/
    vector<Script> scriptVec;
    int currentScriptIndex;
    /*************对战斗等操作的响应**************/
    void roleRecover();//某个角色恢复
    void roleAttack(int attackerX,int attackerY);//某个角色攻击另外一个角色,因为添加时候无法预知未来攻击发生的位置,所以需要将参数传递进去
    void roleCure(int special);
    void roleCure(int x,int y,int recoverHP);
    void roleDie(int x, int y);//负责处理x,y坐标处角色死亡的一切销毁工作
    void selectTarget(DIRECTION dir);

    /************************键盘响应处理部分***********************/
private:
    KeyboardControl *key;
    bool userControlOn;//允许键盘响应
    bool directionKeyPressed;
//    int currentUpKey;
//    int currentDownKey;
//    int currentLeftKey;
//    int currentRightKey;
//    int currentActionKey;
//    int currentCancelKey;
//    int currentMenuKey;
    bool upKeyPressed;
    bool downKeyPressed;
    bool leftKeyPressed;
    bool rightKeyPressed;

    void onDirectionKeyPress(DIRECTION);
    void onDirectionKeyRelease(DIRECTION);
    void onActionKey();
    void onCancelKey();
    void onMenuKey();
    /***************移动事件处理***************/
    DIRECTION focusDir;//焦点移动方向
    DIRECTION focusDirCache;
    bool viewIsMoving;
    void move();
    void viewMove(DIRECTION,int,int);//移动的同时还要设置focusRole的mapPoint
    void focusRoleMove(DIRECTION dir, int focusRolePointX, int focusRolePointY);
    void cursorMove(DIRECTION);
    /********TimerID********/
    int viewMoveTimerID;
    int viewDelayTimerID;
    int viewAnimationMoveTimerID;
    int descriptionTimerID;
    /**************************继承的事件响应函数部分***********************/
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    void focusOutEvent(QFocusEvent *);
    /*************************菜单/射程/行走范围显示***********************/
    int labelSelectedIndex;//在显示操作栏的当前选中的索引值,从0开始
    int weaponSelectedIndex;
    int medicineSelectedIndex;
    vector<QPoint> rangeVec;//需要显示的射程/行走范围等
    void addRangeIntoVector();//true时为行走范围，false为射程
    /************获胜条件与失败条件***************/
    IfBattleEnd checkBattleEnd();
    int winRequirement;
    int loseRequirement;
    //int winCash;
    //QString winGoods;
    //QString winRequirementText;
    //QString loseRequirementText;//用于显示
    /*************其他***********/
    bool nextStepIsInRange(int ,int,DIRECTION);
    //uint currentLiveRoleNum;
    uint currentActiveHeroNum;//当前活动的角色数量
    uint currentActiveEnemyNum;
    uint currentActiveFriendNum;
    uint currentRoundNum;//当前回合数
    void cursorMoveToFocusRole();//在scriptVec中添加一些动画
    /***************子窗口部件*******************/
    WeaponWidget *weaponWidget;
    OperationWidget *operationWidget;
    CursorWidget *cursorWidget;
    DescriptionWidget *descriptionWidget;
    /***********武器特效***********/
    //int articleSpecial;//存储当前使用的武器/药物特效,以允许光标选择己方或者地方人物
    std::map<int,ArticleInfo> *pArticleIDToInfoMap;

    void focusRoleDonotMove();

};

inline void BattleScene::focusRoleDonotMove()
{
    scriptVec.push_back(Script(AniRoleRotate,QString("%1#%2#")
                               .arg(focusRole->getMapX()).arg(focusRole->getMapY())));
    scriptVec.push_back(Script(AniFocusRoleBeInactive));
}

#endif // BATTLESCENE_H
