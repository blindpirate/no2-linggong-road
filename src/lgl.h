#ifndef LGL_H
#define LGL_H
#include <QtGui>
#include <QtWidgets>
#include <assert.h>
#include <vector>
#include <set>
#include <ctime>
#include <map>
using namespace std;
#include "maparray.h"

#define WINDOWWIDTH 480
#define WINDOWHEIGHT 480
#define BASEWIDTH 32
#define BASEHEIGHT 32
#define ROLENUMBER 20//角色数量
#define CURRENTVERSION 0
#define CHANGEJOBLEVEL1 20//转职级别1:20级
#define CHANGEJOBLEVEL2 40//转职级别2:40级

#if 0
#define IGNOREANIDIALOG
#define IGNOREANIDELAY
#define IGNOREANIVIEWMOVE
#define IGNORESWEAT
#define IGNOREATTENTION
#endif

#define MANHATTAN(x1,y1,x2,y2) (abs((x1)-(x2))+abs((y1)-(y2)))

#define NATURALLOGE 2.718281828459045

#define SHIELDUPBOUND 10000
#define SHIELDDOWNBOUND 8000
//防具的id范围,所有职业都可以携带
#define MEDICINEUPBOUND 200000
#define MEDICINEDOWNBOUND 9999
//药物的ID范围
#define ARTICLEUPBOUND 8000
#define ARTICLEDOWNBOUND 6000
//随身物品的ID范围
#define WEAPONUPBOUND 4000


#define MAXWEAPONCOUNT 4
#define MAXMEDICINECOUNT 4
#define MAXSHIELDCOUNT 1
#define MAXARTICLECOUNT 1
//#define DEV
//#define BETA
/*
  0:demo
  1:alpha
  2:beta
  10:1.0
  20:2.0
*/

//#define FADEINANIMATION\
//    QWidget *mask=new QWidget(this);\
//    mask->setGeometry(0,0,480,480);\
//    mask->setAttribute(Qt::WA_DeleteOnClose);\
//    mask->setStyleSheet("background-color:black;");\
//    QPropertyAnimation *fadeInAnimation=new QPropertyAnimation(mask,"windowOpacity",this);\
//    fadeInAnimation->setDuration(1000);\
//    fadeInAnimation->setStartValue(1.0);\
//    fadeInAnimation->setEndValue(0.0);\
//    fadeInAnimation->start();\
//    connect(fadeInAnimation,SIGNAL(finished()),mask,SLOT(close));\


//错误代码
#define ERRORFILENOTFOUND -1
#define ERRORIMAGEFILENOTFOUND -2
#define ERRORSETTINGFILENOTFOUND -3
#define ERRORGETROLEINFOFAIL -4
#define ERRORGETARTICLESETFAIL -5
#define ERRORGETSTORYSCENEDATAFAIL -6
#define ERRORSAVEFILEREADFAIL -7
#define ERRORBATTLEMAPFILENOTFOUND -8
#define ERRORGETMAPIDFROMSAVEFAIL -9
#define ERRORGETBATTLEROLEDATAFROMLIBFAIL -10
#define ERRORGETBATTLEANIDATAFROMLIBFAIL -11
#define ERRORGETROLEIMAGEFAIL -12
#define ERRORGETROLEANIMATIONIMAGEFAIL -13
#define ERRORGETARTICLEINFOFAIL -14
#define ERRORGETEXITINFOFAIL -15
#define ERRORMEMORYALLOCFAIL -16
#define ERRORLIBDATABROKEN -17
#define ERRORINTTOOSHORT -18//要求int至少是4个字节
#define ERRORGERBATTLEFAILSCRIPTFAIL -19
#define ERRORGERBATTLEWINSCRIPTFAIL -20
#define ERRORGETBATTLEWINFAILDATAFAIL -21

typedef enum ROLEPOSE
{
    MOVINGFRONT_1,STAYFRONT,MOVINGFRONT_2,
             MOVINGLEFT_1,STAYLEFT,MOVINGLEFT_2,
             MOVINGRIGHT_1,STAYRIGHT,MOVINGRIGHT_2,
             MOVINGBACK_1,STAYBACK,MOVINGBACK_2
}
ROLEPOSE;
typedef enum DIRECTION{NOTMOVING=0,UP=1,DOWN=2,LEFT=3,RIGHT=4}DIRECTION;
//typedef enum SCENE{MAIN1,MAIN2,MAIN3,MAIN4,WEST1,WEST2}SCENE;
//typedef enum MapObject{Reachable,Unreachable,
//                      MAIN1ENTRANCE1,MAIN1ENTRANCE2,MAIN1ENTRANCE3,
//                      MAIN2ENTRANCE1,MAIN2ENTRANCE2,MAIN2ENTRANCE3,
//                      MAIN2ENTRANCE4,MAIN2ENTRANCE5,MAIN2ENTRANCE6,
//                      MAIN3ENTRANCE1,MAIN3ENTRANCE2,MAIN3ENTRANCE3,
//                      MAIN3ENTRANCE4,MAIN3ENTRANCE5,MAIN3ENTRANCE6,MAIN3ENTRANCE7,
//                      MAIN4ENTRANCE1,MAIN4ENTRANCE2,MAIN4ENTRANCE3,MAIN4ENTRANCE4,
//                      WEST1ENTRANCE1,WEST1ENTRANCE2,WEST1ENTRANCE3,WEST1ENTRANCE4,
//                      WEST2ENTRANCE1,WEST2ENTRANCE2
//                      }MapObject;
//main1入口:底部3个,从左往右依次是1,2,3.
//main2入口:顶部3个底部3个,共六个,左上往右下依次是123456
//main3入口:顶部3个底部3个左侧1个,左上往右下依次是123456,左侧入口为7
//main4入口:顶部3个,从左往右依次是123,左侧1个,4号
//west1入口:右侧2个,顶部2个,顺时针依次是1234
//west2入口:底部2个,从左往右依次是12

//storyscene
typedef enum StorySceneObject{
    //Reachable,Unreachable,
    StoryMapHero=1,StoryMapNPC=2,StoryMapObject=3,
    StorySceneAnimation=4,//过场动画
    StoryMapArticle=5,//只可以拾取一次的物品
    StoryMapExit=6,
    NPCSideStoryNotOn=7,//支线剧情未触发
    NPCSideStoryOnNotOK=8,//支线剧情被触发但是未完成
    NPCSideStoryOnAndOK=9,//支线剧情完成但是未关闭
    NPCSideStoryOKOff=10,//支线剧情完成并且关闭
    StoryMapCash=11//在map中获得若干金钱,上限是65536
    //之所以把article分离出来,是因为相对于其他物品它更特殊
    //SideStoryMapNPC=7//支线剧情的NPC
}StorySceneObject;

typedef enum BattleSceneObject
{
    BattleHero=101,BattleEnemy=102,BattleFriend=103,//这是地图上的物体类型
    BattleSceneAnimation=104,
    BattleSceneMapID=105,
    BattleSceneWinRequirement=106,//非常重要!!!!参数用十六进制显示!!!!!
    BattleSceneLoseRequirement=107,//非常重要!!!!参数用十六进制显示!!!!!
    //BattleSceneStartViewPoint=108,//初始视角位置
    BattleSceneHeroStartLocation=109//出生点
    //BattleSceneWinCash=110,//获得现金
    //BattleSceneWinGoods=111,
    //BattleSceneWinText=112,//胜利条件的文本显示
    //BattleSceneLoseText=113
}BattleSceneObject;


//物理系:肌肉男,女汉子
//魔法系:浪子,宅男,妹纸,学渣
//远程:大妈
//平衡系:屌丝,大叔
typedef enum ROLEJOB
{
    JIROUNAN=0,BAOLINAN=1,QIGONGSHI=2,//肌肉男,暴力男,气功师,职业代码0,使用1~99的武器
    NVHANZI=3,BAWANGHUA=4,NVWANG=5,//女汉子,霸王花,女王,职业代码1,使用101~199
    //亦可使用1001~1099的物理系武器

    FUERDAI=6,TUHAO=7,GAOFUSHUAI=8,//富二代,土豪,高富帅,职业代码2,使用201~299的武器
    ZHAINAN=9,JISHUZHAI=10,JISHUSHEN=11,//宅男,技术宅,技术神,职业代码3,301~399
    XUEZHA=12,ZIXINAN=13,XUEBA=14,//学渣,自习男,学霸,职业代码4,401~499
    MEIZHI=15,MENGMEIZHI=16,NVSHEN=17,//妹纸,萌妹纸,女神,职业代码5,501~599
    //亦可使用1101~1199的魔法系武器

    DAMA=18,CAIDAOMA=19,ZHANDOUMA=20,//大妈,菜刀妈,战斗妈,职业代码6,601~699
    //亦可使用1201~1299的远程系武器

    DASHU=21,SHUSHU=22,GUAISHUSHU=23,//大叔,蜀黍,怪蜀黍,职业代码7,701~799
    DIAOSI=24,DIAOBAO=25,NIXI=26//屌丝,碉堡,逆袭,职业代码8,801~899
    //亦可使用1301~1399的酱油系武器

    //QIANGSHOU=24,JUJISHOU=25,SHENSHESHOU=26,//枪手,狙击手,神射手,职业代码8


}ROLEJOB;

const int jobToWeaponID[9]=
{10,10,11,11,11,11,12,13,13};
//职业代码转换为武器代码

typedef struct KeyboardControl
{
    int upKey;
    int downKey;
    int leftKey;
    int rightKey;
    int actionKey;
    int cancelKey;
    int menuKey;
}KeyboardControl;


struct ArticleInfo
{
    //物品
    ArticleInfo(QString name,
                int attack,
                int accurcy,
                int price,
                int special,
                QString des):
        accurcy(accurcy),
        attack(attack),
        special(special),
        price(price),
        name(name),
        description(des){}
    ArticleInfo():
        accurcy(0),
        attack(0),
        special(0),
        price(0)
    {}
    QString name;
    int accurcy;
    int attack;//同时也是defence
    int special;
    int price;
    QString description;
};
typedef enum AnimationOperation
{//枚举量具有ANI开头
    AniStart=1,//动画开始,无参数
    //以下的参数前两位均为xy
    AniViewMoveTo=2,//view快速移动到某处,参数为中心点地图坐标如10#20#,如果是-1#-1#则设置为当前主角位置
    AniRoleSetPath=3,//为某角色设定path,假定该角色为npc或者enemy或者friend类.参数亏如2#3#4##3#第一个点是role所在位置
    AniRoleMove=4,//普通移动,参数为role坐标,可以为多个role的坐标同时开始行走,若为-1,-1则令targetRole开始移动
    AniRoleFastMove=5,//攻击时的移动,更快,参数同上
    AniRoleSweat=6,//流汗
    AniRoleAttack=7,//role开始攻击动画,参数为坐标
    AniRoleRecover=8,//role开始恢复,参数为坐标#原始体力#恢复体力#满体力
    AniRoleHurt=9,//role开始受伤害,参数为坐标#原始体力#扣减体力#满体力
    AniRoleDie=10,//role死亡,参数为坐标
    AniRoleDodge=11,//role闪避,参数为坐标
    //以上参数前两位均为xy
    AniViewStartGetGoods=12,//参数是要显示的所有文字,无#
    AniViewStartDialog=13,//开始对话,参数为dialog参数
    AniViewStartDelay=14,//延时,参数为毫秒无#
    AniShowBattleStart=15,//显示"战斗开始"字样
    AniShowRoundX=16,//显示"第X回合"字样,参数为X(无#)
    AniShowBattleFail=17,//显示"战斗失败"字样
    AniShowBattleWin=18,//显示"胜利"字样
    AniShowSelfAction=19,//我军行动
    AniShowEnemyAction=20,//敌军行动
    AniShowFriendAction=21,//友军行动
    AniStartBattle=23,//开始战斗,参数是battleID无#
    AniSwitchScene=24,//切换场景,参数是storyID#mapID#viewX#viewY#
    AniEnd=25,//动画结束,无参数
    AniCursorMoveToViewPoint=26,//参数是点坐标200#320#
    AniFocusRoleBeInactive=27,//无参数,令focusRole停止活动
    AniRoleDisappear=28,//在story场景中消失,若为-1#-1#则是野怪坐标
    AniStartTrade=29,//开始贸易,参数是articleID#articleID#articleID#
    AniRoleLevelUp=30,//参数是2#2#4#3#,每两个进行一个分割
    AniRoleChangeJob=31,//参数是x#y#
    AniRoleAttention=32,//参数是x#y#
    AniRoleDisable=33,//参数是x#y#
    AniRolePoison=34,//参数是x#y#
    AniRoleAttackDown=35,
    AniRoleAccurcyDown=36,
    AniRoleCritDown=37,
    AniRoleDefenceDown=38,
    AniRoleDodgeDown=39,
    AniRoleSpeedDown=40,
    AniRoleRotate=41,
    AniRoleCure=42,
    AniRoleUseMagic=43,
    AniAddRole=44,//参数是新加入的roleID,无#
    AniSetStoryIDTo=45,//设置storyID,无#
    AniRoleFlash=46,//闪烁,注意,它是单参数的,就是说,它后面必须是1#2#不能是1#2#3#4#
    //AniSetSideStoryOn=46,//某个支线剧情被触发,参数为支线剧情id,无#
    //AniSetSideStoryOK=47,//某个支线剧情被完成,无参数
    AniGetCash=48,//无#
    AniGetArticle=49,//无#
    //AniSetSideStoryOff=50,//支线剧情完成,关闭之,无参数
    AniQuitToMainMenu=51,//无参数
    AniSetSideStoryIDTo=52,//设置sideStoryID(关闭sideStory置0即可),默认清零右二和右三字节的标志位,参数无#
    AniSetSideStoryOn=53,//无参数,打开支线剧情(右二字节置1)
    AniSetSideStoryOK=54,//无参数,支线剧情完成(右三字节置1)
    AniSetSideStoryOff=55,//无参数,关闭支线剧情(右二字节置0)
    AniMultiTalkFlag=222/*这个标志位标志了NPC的动画分为多段,无参数.如果最后一个元素是它,向前搜索到上一个AniEnd,
将之后的script全部砍掉*/

}AnimationOperation;

typedef enum BattleDisplayState{
    BattleShowAnimation,//正在显示动画
    BattleNoExtraDisplay,//没有额外显示
    BattleShowLabel,//显示操作栏
    BattleShowWeapon,//显示武器选择界面
    BattleShowSelfWalkRange,//显示行走范围
    BattleShowSelfRange,//显示射程
    BattleShowSelfMedicineRange,
    BattleShowEnemyRange,
    BattleShowEnemyWalkRange,
    BattleShowFriendRange,
    BattleShowFriendWalkRange,
    BattleShowMedicine
}BattleDisplayState;

typedef struct Script
{
    Script():
        op(AniStart)
    {}
    Script(AnimationOperation op,QString const param=QString()):
        op(op),
        param(param)
    {
    }
    QString param;
    AnimationOperation op;
}Script;


typedef struct StorySceneData
{
    StorySceneData(int currentPointX,
                   int currentPointY,
                   int storyID,
                   int sideStoryID,
                   int mapID,
                   int currentBattleID,
                   int cash):
        currentPointX(currentPointX),
        currentPointY(currentPointY),
        storyID(storyID),
        sideStoryID(sideStoryID),
        mapID(mapID),
        currentBattleID(currentBattleID),
        carriedCash(cash)
    {}
    int currentPointX;
    int currentPointY;
    int storyID;
    int sideStoryID;
    int mapID;
    int currentBattleID;
    int carriedCash;
}StorySceneData;
typedef struct BattleSceneData
{
    BattleSceneData(int currentPointX,
                    int currentPointY,
                    int battleID,
                    int mapID,
                    int cursorPointX,
                    int cursorPointY,
                    int currentRoundNum):
        currentPointX(currentPointX),
        currentPointY(currentPointY),
        battleID(battleID),
        mapID(mapID),
        cursorPointX(cursorPointX),
        cursorPointY(cursorPointY),
        currentRoundNum(currentRoundNum)
    {}
    int currentPointX;
    int currentPointY;
    int battleID;
    int mapID;
    int cursorPointX;
    int cursorPointY;
    int currentRoundNum;
}BattleSceneData;

void getRangeVectorFromRange(int range,vector<QPoint> &rangeVec,int x,int y,int mapSizeX,int mapSizeY);
//根据range类型和当前点确定rangeVec中的点
bool checkIfInRange(const QPoint &attacker,const QPoint &defencer,int range);
QRect getRectFromPose(ROLEPOSE pose);
QRect getRectFromMapIndex(int xIndex,int yIndex);
QString getSubFromQString(QString const &,int,char);
void swap(int &x,int &y);
QString &cutHeadFromQString(QString&,int,char);

typedef struct BattleInfo
{
    BattleInfo(int maxNum,QString const &name):
        maxNum(maxNum),
        name(name){}
    int maxNum;
    QString name;
}BattleInfo;

BattleInfo getBattleInfoFromBattleID(int id);
//int processNextArrayElement(int **mapArray,bool isMoving,int currrentX,int currentY,int mapSizeX,int mapSizeY,DIRECTION dir);
//返回下一个mapArray的元素,如果下一个矩阵元素是0且当前该角色正在移动则交换两者值
//其中currentXY是当前的矩阵坐标,mapsizexy是矩阵坐标上限


//int QStringToInt(QString const &);
//砍掉string开头的前n个分隔符之前的字符

//获得第n个#和第n-1个分隔符之间的子串,例如f("100#200#",2)="200"
//int getBattleMapIDFromBattleID(int battleID);
//从battleID获取mapID,因为多个battleID可能对应同一张地图
void getPathFromQString(vector<QPoint> &vec,QString &str);
QString getRoleNameFromRoleID(int);
void quitApp(int errorCode);

//QString getMenuStringFromIndex(int index);
void getScriptFromQString(vector<Script> &vec, QString &str);
bool getRandomOfPercent(int x);//对于输入的x,返回true或false,true的概率为百分之x
int getRandomNumberIn(int x,int y);//返回x,y区间内的一个随机数


const double jobCoefficient[9][9]=
{//肌肉男    女汉子    浪子    宅男     学渣    妹纸     大妈    大叔    屌丝 ←攻击方↓防御方
    1.0,    2.0,     0.8,    0.8,    0.8,    2.0,    1.0,    0.9,   0.8,//肌肉男
    1.2,    1.0,     0.9,    1.2,    1.2,    0.7,    1.1,    1.1,   2.0,//女汉子
    1.4,    1.2,     1.0,    0.9,    0.8,    0.9,    1.8,    1.2,   0.8,//浪子
    1.6,    0.95,    1.2,    1.0,    1.1,    2.0,    1.1,    0.9,   1.1,//宅男
    1.6,    0.95,    1.2,    1.1,    1.0,    2.0,    1.1,    0.9,   1.1,//学渣
    0.6,    1.1,     1.8,    0.5,    0.6,    1.0,    1.3,    1.8,   0.5,//妹纸
    1.1,    1.0,     1.1,    0.9,    0.9,    0.6,    1.0,    1.8,   1.0,//大妈
    1.2,    1.0,     0.9,    1.1,    1.1,    1.2,    0.9,    1.0,   1.1,//大叔
    1.2,    0.95,    2.0,    1.1,    1.1,    2.5,    1.2,    1.0,   1.0//屌丝
};
//{//肌肉男    大妈    大叔    女汉子   浪子    屌丝      宅男    学渣     妹纸 ←攻击方↓防御方
//    1.0,    1.0,    0.9,    2.0,    0.8,    0.8,    0.8,    0.8,   2.0,//肌肉男
//    1.1,    1.0,    1.8,    1.0,    1.1,    1.0,    0.9,    0.9,    0.6,//大妈
//    1.2,    0.9,    1.0,    1.0,    0.9,    1.1,    1.1,    1.1,   1.2,//大叔
//    1.2,    1.1,    1.1,    1.0,    0.9,    2.0,    1.2,    1.2,     0.7,//女汉子
//    1.4,    1.8,    1.2,    1.2,    1.0,    0.8,    0.9,    0.8,    0.9,//浪子
//    1.2,    1.2,    1.0,    0.95,   2.0,    1.0,    1.1,    1.1,     2.5,//屌丝
//    1.6,    1.1,    0.9,    0.95,   1.2,    1.1,    1.0,    1.1,       2,//宅男
//    1.6,    1.1,    0.9,    0.95,   1.2,    1.1,    1.1,    1.0,      2,//学渣
//    0.6,    1.3,    1.8,    1.1,    1.8,    0.5,    0.5,    0.6,     1.0//妹纸
//};
//int getTypeFromRoleID(int id);
//int getSideStoryIDFromStoryID(int storyID);


#endif // LGL_H
