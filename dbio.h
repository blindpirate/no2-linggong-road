#ifndef DBIO_H
#define DBIO_H
#include "lgl.h"
#include "sqlite3.h"
#include "roleview.h"
//#include "invisibleobject.h"
#include "role.h"

class DBIO
{
public:
    DBIO();
    ~DBIO();
    //static int get
    //bool isReady()const{return databaseReady;}
//    int getCountOfNPC(){return countOfNPC;}
//    int getCountOfGoods();
    bool getStorySceneDataFromLib(vector<RoleView *> &roleVec,
                                  vector<Script> &scriptVec,
                                  set<uint> &gotArticleSet,
                                  QWidget *parent,
                                  int storyID,
                                  int mapID,
                                  int **mapArray,
                                  int sideStoryID);
    //从库中获得storyScene,若articleID已经存在于gotArticleIDSet中则不将它加入objVec
    bool getBattleDataFromLib(int battleID,
                              vector<RoleView *> &enemyRoleVec,
                              vector<RoleView *> &friendRoleVec,
                              vector<Script> &animationVec,
                              QWidget *parent, //battleScene指针
                              int &mapID, //装载的mapID
                              int &winRequirement,//胜利条件
                              int &loseRequirement, //失败条件
                              //int &winCash, //胜利获得金钱
                              //QString &winGoods,//胜利获得物品 1#2#3#
                              //QString &winText,
                              //QString &loseText,
                              int &startLocationX,
                              int &startLocationY,//出生点
                              int heroLevel);//如果是野怪,根据hero等级构造一个
    bool getBattleWinFailTextFromLib(int id,QString &win,QString &fail);
    bool getGlobalGoodsIDFromLib(set<int> &goodsIDSet);
    //bool getWeaponInfoFromLib(int weaponID,WeaponInfo **info);
    //bool getMedicineInfoFromLib(int medicineID,MedicineInfo **info);
    bool getRoleInfoFromLibOfID(Role **roleInfoArray,int id);
    bool getArticleInfoFromLib(int id,map<int,ArticleInfo> *pArticleIDToInfoMap);
    bool getBattleWinScriptFromLib(int battleID,vector<Script>&scriptVec);
    bool getBattleFailScriptFromLib(int battleID,vector<Script>&scriptVec);

    bool getStorySceneDataFromSaveFile(QWidget *parent,
                                       QString const &fileName,
                                       StorySceneData &storySceneData,
                                       vector<RoleView*>&roleViewVec,
                                       //vector<InvisibleObject *>&objectVec,
                                       Role **roleInfoArray,
                                       set<uint> &gotArticleSet,
                                       multiset<int>&carriedArticleIDSet,
                                       int **mapArray);//注意这是二级指针,指向的是scene中的mapArray(0)

    bool isASaveFile(QString const &fileName,QString &time,int &roleNum);
    bool checkSaveFile(const QString &fileName,QString &time,int &roleNum);
    //检查一个存档文件,将创建日期和角色数量返回
    bool getKeySettings(int &upKey,int &downKey,int &leftKey,int &rightKey,
                        int &actionKey,int &cancelKey,int &menuKey);
    bool setKeySettings(int upKey,int downKey,int leftKey,int rightKey,
                        int actionKey,int cancelKey,int menuKey);


    //int **getMapArrayPointerFromBattleID(int battleID,int mapSizeX,int mapSizeY);

    bool createSaveFile(QString const &filePathName,QString const &time,int version);
    bool saveStorySceneToFile(StorySceneData const&sceneData,
                              vector<RoleView *> &roleVec,
                              set<uint> gotArticleSet,
                              multiset<int> carriedArticleIDSet,
                              Role **roleInfo,
                              int *mapArray,
                              int mapArraySize);//mapArray的元素数量,没有乘4
    //本函数将storyScene存档入指定文件
    bool saveBattleSceneToFile(BattleSceneData const &sceneData,
                               vector<RoleView *>&selfViewVec,
                               vector<RoleView*>&enemyViewVec,
                               vector<RoleView *> &friendViewVec,
                               int *mapArray,
                               int mapArraySize);
    //按照调用顺序排序,因为StoryScene已经打开了数据库文件,所以BattleScene就只管往里存就行了

    bool getBattleDataFromSaveFile(QWidget *parent,
                                   vector<Script> &scriptVec,
                                   vector<RoleView*>&selfViewVec,
                                   vector<RoleView*>&enemyViewVec,
                                   vector<RoleView*>&friendViewVec,
                                   BattleSceneData &battleSceneData,
                                   int **mapArray,
                                   int &winRequirement,//胜利条件
                                   int &loseRequirement);
    //此时数据库连接已经确定,不需要存档文件名,另外,这个函数是读档时进行的最后一步,在读出mapID之后,
    //关闭存档文件的数据库连接,重新根据mapID从lib中读取胜利失败条件的一些数据

    bool getExitInfoFromLib(int mapID,int inx,int iny,int &toMapID,int &toX,int &toY);


private:
    //int storyID;
    //int mapID;
    //bool databaseReady;
    sqlite3 *db;
    sqlite3_stmt *statement;
};
/*
  角色的职业、hp等数据分为两种方式存放：
  如果是默认的数据库，即某个角色出场时的数据，存放于
  /data/lib/roledata.dat中。否则存在于存档文件中。
  这些数据存在于roleinfo数据表中，其数据类型和顺序
  */

#endif // DBIO_H
