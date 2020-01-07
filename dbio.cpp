#include "dbio.h"
#include<iostream>
#include<fstream>
//physicalAttack=A+(level-B)*3;
//physicalDefence=A+(level-B)
//hp=A+(level-B)*4
const int physicalAttackA[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    70,     130,190,    63, 123,  183,60,   120,160,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    55, 115,  175,  55,  115, 175, 50, 110,  170,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    65, 125,   185, 63, 123,183,   50, 110,220
};
const int physicalAttackB[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    0,     20,  40,     0,  20,   40,   0,  20, 40,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    0,     20,  40,  0, 20,    40,  0,  20,  40,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    0,  20,    40,  0,  20,   40,  0,   20,40
};
const int physicalDefenceA[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    40,     60, 80,     29, 49,   69, 32,   59, 79,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    35, 55,   75,   30, 50,   70, 20,  40,   60,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    35, 55,     75, 32, 52, 69,    34, 54, 90,
};
const int physicalDefenceB[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    0,     20,  40,     0,  20,   40,   0,  20,   40,
//宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
 0,     20,  40,  0, 20,    40,  0,  20,  40,
//大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
  0,  20,    40,  0,  20,   40,  0,   20,   40
};
const int HPA[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    70,    160,250,    65,  130, 210,   80,150, 200,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    55, 120,  190,  65,  125, 200, 55,  110, 180,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    65, 150,   230, 60, 150, 210,  60, 110,280
};
const int HPB[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
 0,     20,  40,     0,  20,   40,   0,  20, 40,
//宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
 0,     20,  40,  0, 20,    40,  0,  20,  40,
//大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    0,  20,    40,  0,  20,   40,  0,  20,  40
};
const int IQ[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    99,     99,   105,  99, 100,  110, 110, 120, 130,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    120,130,  140,  120,  130, 140,120, 130,140,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    95,105,     115,110, 120,130, 95,   115,150
};
const int ACCURCY[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    90,    95,  95,    90,  95,   95,   95, 95,   95,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    96, 99,     99,  95,  95,  95, 95,  99,  99,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    110, 120,  130, 90,  93,  95,  95,  95, 99
};
const int DODGE[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    0,    0,    0,    0,    0,    0,   10,   10,  10,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    2,    6,    8,   0,   2,   5,   5,  10,   15,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    0,   6,    10,  2,   2,   2,   3,   7,  20
};
const int RANGE[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    1,     2,   2,      1,  2,    4,    2,  5,  6,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    2,     4,   5,  1,   4,    5,  4,   5,   7,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    3,   8,     9,  1,  2,     4,   1,  4,  7
};
const int WEAPON0[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    0,    0,     0,   0,     0,    0,  0,   0,   0,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    0,    0,     0,   0,  0,   0,   0,  0,   0,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    0,   0,    0,    0,  0,   0,   0,   0,   0
};
const int SPEED[27]=
{//肌肉男,暴力男,气功师,女汉子,霸王花,女王,富二代,土豪,高富帅,
    5,      6,  7,     6,   7,     8,   6,  8,  10,
 //宅男,技术宅,技术神,学渣,自习男,学霸,妹纸,萌妹纸,女神
    5,  6,      7,  5,     6,   7,  6,  7,  10,
 //大妈,菜刀妈,战斗妈,大叔,蜀黍,怪蜀黍,屌丝,碉堡,逆袭
    5,  6,      7,  6,  7,     8,   5,  8  ,12
};
DBIO::DBIO():
    db(NULL),
    statement(NULL)
{
//    QString path=QCoreApplication::applicationDirPath() +QString("/data/lib/data.dat");
//    if(sqlite3_open(path.toAscii().data(),&db)==SQLITE_OK)
//    {
//        databaseReady=true;
//    }

}
DBIO::~DBIO()
{
    //if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    sqlite3_close(db);
}
bool DBIO::getStorySceneDataFromLib(vector<RoleView *> &roleVec,
                                    //vector<InvisibleObject *> &objVec,
                                    vector<Script> &scriptVec,
                                    set<uint> &gotArticleSet,
                                    QWidget *parent,
                                    int storyID,
                                    int mapID,
                                    int **mapArray,
                                    int sideStoryID)
{
    QString path=QCoreApplication::applicationDirPath() +QString("/data/lib/global.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,QString("select inx,iny from exit where inmap=%1").arg(mapID).toUtf8().data(),
                          -1,&statement,NULL)!=SQLITE_OK)
        return false;
    int x=0,y=0;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        x=sqlite3_column_int(statement,0);
        y=sqlite3_column_int(statement,1);
        mapArray[y][x]&=0;
        mapArray[y][x]|=static_cast<int>(StoryMapExit);
    }
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;
    sqlite3_close(db);

    path=QCoreApplication::applicationDirPath() +QString("/data/lib/story.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    assert(roleVec.size()==1);
    assert(scriptVec.empty());
    if(sqlite3_prepare_v2(db,
                          QString("select count(*)from sqlite_master where name='table_%1_%2'")
                          .arg(storyID).arg(mapID).toUtf8().data(),
                          -1,
                          &statement,
                          NULL)!=SQLITE_OK)
        return false;//storyID的主线剧情都是奇数,如果是偶数证明支线剧情被开启了,这时候才把支线剧情NPC读入
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;
    if(sqlite3_column_int(statement,0)==0)
        storyID=0;
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;
//    qDebug()<<databaseReady;
    if(sqlite3_prepare_v2(
                db,
                QString("select * from table_%1_%2;").arg(storyID).arg(mapID).toUtf8().data(),
                -1,
                &statement,
                NULL)!=SQLITE_OK)
        return false;
    QString param;
    RoleView *roleView=NULL;
#ifndef NDEBUG
    qDebug()<<"load story id:"<<storyID<<" map id:"<<mapID;
#endif
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        int type=sqlite3_column_int(statement,0);
        x=sqlite3_column_int(statement,1);
        y=sqlite3_column_int(statement,2);
        param=QString::fromUtf8((char *)(sqlite3_column_text(statement,3)));
        switch(type)
        {
        case static_cast<int>(StoryMapNPC):
#ifndef NDEBUG
        qDebug()<<"MapNPC:"<<x<<" "<<y<<" "<<param;
#endif
            roleView=new RoleView(parent,x,y,getSubFromQString(param,1,'@').toInt());
            cutHeadFromQString(param,1,'@');
            roleView->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
            cutHeadFromQString(param,1,'@');
            roleView->setScriptVec(getSubFromQString(param,1,'@'));
            cutHeadFromQString(param,1,'@');
            roleView->setPathVec(getSubFromQString(param,1,'@'));
            roleVec.push_back(roleView);
            //fade->raise();
            //fade->connect(fade,SIGNAL(destroyed()),roleView,SLOT(show()));
            break;
        case static_cast<int>(StoryMapObject):
            assert(0);
            break;
//            object=new InvisibleObject(parent,x,y,getSubFromQString(param,1,'@').toInt());
//            cutHeadFromQString(param,1,'@');
//            object->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
//            cutHeadFromQString(param,1,'@');

//            object->setScriptVec(getSubFromQString(param,1,'@'));
//            objVec.push_back(object);
//                break;
        case static_cast<int>(StoryMapArticle):
        case static_cast<int>(StoryMapCash):
        {
#ifndef NDEBUG
        qDebug()<<"MapArticle/Cash:"<<x<<" "<<y<<" "<<param;
#endif
            uint element=((static_cast<int>(storyID))<<24)|(mapID<<16)|(x<<8)|y;
            if(gotArticleSet.find(element)!=gotArticleSet.end())
                break;
            //左起4个字节分别是原来地图矩阵的值(1Byte),物品id(2Bytes),storyMapArticle(1Byte)
            mapArray[y][x]<<=24;
            mapArray[y][x]|=(param.toInt()<<8);
            mapArray[y][x]|=type;
            break;
        }

        case static_cast<int>(StorySceneAnimation):
#ifndef NDEBUG
        qDebug()<<"MapAnimation:"<<x<<" "<<y<<" "<<param;
#endif
            assert(x==-1&&y==-1);
            getScriptFromQString(scriptVec,param);
            break;
//        case static_cast<int>(StoryMapExit):
//            mapArray[y][x]|=(param.toInt()<<8);
//            break;
//        case static_cast<int>(NPCSideStoryNotOn):
//            if(sideStoryOK||sideStoryOn)
//                break;
//            roleView=new RoleView(parent,x,y,getSubFromQString(param,1,'@').toInt());
//            cutHeadFromQString(param,1,'@');
//            roleView->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
//            cutHeadFromQString(param,1,'@');
//            roleView->setScriptVec(getSubFromQString(param,1,'@'));
//            cutHeadFromQString(param,1,'@');
//            roleView->setPathVec(getSubFromQString(param,1,'@'));
//            roleVec.push_back(roleView);
//            break;
//        case static_cast<int>(NPCSideStoryOnNotOK):
//            if(!sideStoryOn||sideStoryOK)
//                break;
//            roleView=new RoleView(parent,x,y,getSubFromQString(param,1,'@').toInt());
//            cutHeadFromQString(param,1,'@');
//            roleView->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
//            cutHeadFromQString(param,1,'@');
//            roleView->setScriptVec(getSubFromQString(param,1,'@'));
//            cutHeadFromQString(param,1,'@');
//            roleView->setPathVec(getSubFromQString(param,1,'@'));
//            roleVec.push_back(roleView);
//            break;
//        case static_cast<int>(NPCSideStoryOnAndOK):
//            if(!sideStoryOn||!sideStoryOK)
//                break;
//            roleView=new RoleView(parent,x,y,getSubFromQString(param,1,'@').toInt());
//            cutHeadFromQString(param,1,'@');
//            roleView->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
//            cutHeadFromQString(param,1,'@');
//            roleView->setScriptVec(getSubFromQString(param,1,'@'));
//            cutHeadFromQString(param,1,'@');
//            roleView->setPathVec(getSubFromQString(param,1,'@'));
//            roleVec.push_back(roleView);
//            break;
//        case static_cast<int>(NPCSideStoryOKOff):
//            if(!sideStoryOK||sideStoryOn)
//                break;
//            roleView=new RoleView(parent,x,y,getSubFromQString(param,1,'@').toInt());
//            cutHeadFromQString(param,1,'@');
//            roleView->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
//            cutHeadFromQString(param,1,'@');
//            roleView->setScriptVec(getSubFromQString(param,1,'@'));
//            cutHeadFromQString(param,1,'@');
//            roleView->setPathVec(getSubFromQString(param,1,'@'));
//            roleVec.push_back(roleView);
//            break;
        default:
            assert(0);
            break;
        }
    }
    //qDebug()<<roleVec.size()<<" "<<objVec.size();
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    if(sideStoryID==0)//如果此时不存在支线剧情直接返回
        return true;
#ifndef NDEBUG
        qDebug()<<"load side story id:"<<hex<<(sideStoryID);
#endif
    int type=0;
    switch((sideStoryID&0xffff00)>>8)
    {
    case 0x0:
#ifndef NDEBUG
        qDebug()<<"SideStoryNotOn:";
#endif
        type=static_cast<int>(NPCSideStoryNotOn);
        break;
    case 0x0001:
#ifndef NDEBUG
        qDebug()<<"SideStoryOnNotOK:";
#endif
        type=static_cast<int>(NPCSideStoryOnNotOK);
        break;
    case 0x0100:
#ifndef NDEBUG
        qDebug()<<"SideStoryOKOff:";
#endif
        type=static_cast<int>(NPCSideStoryOKOff);
        break;
    case 0x0101:
#ifndef NDEBUG
        qDebug()<<"SideStoryOnAndOK:";
#endif
        type=static_cast<int>(NPCSideStoryOnAndOK);
        break;
    default:
        qDebug()<<sideStoryID;
        assert(0);
    }
    if(sqlite3_prepare_v2(db,
                          QString("select x,y,param from sidestory_%1 where type=%2 and mapid=%3")
                          .arg(sideStoryID&0xff).arg(type).arg(mapID).toUtf8().data(),
                          -1,
                          &statement,
                          NULL)!=SQLITE_OK);
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
#ifndef NDEBUG
        qDebug()<<x<<" "<<y<<" "<<param;
#endif
        x=sqlite3_column_int(statement,0);
        y=sqlite3_column_int(statement,1);
        param=QString::fromUtf8((char *)(sqlite3_column_text(statement,2)));
        roleView=new RoleView(parent,x,y,getSubFromQString(param,1,'@').toInt());
        cutHeadFromQString(param,1,'@');
        roleView->setContactDirectly(static_cast<bool>(getSubFromQString(param,1,'@').toInt()));
        cutHeadFromQString(param,1,'@');
        roleView->setScriptVec(getSubFromQString(param,1,'@'));
        cutHeadFromQString(param,1,'@');
        roleView->setPathVec(getSubFromQString(param,1,'@'));
        roleVec.push_back(roleView);
    }
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;

    return true;
}
bool DBIO::getBattleDataFromLib(int battleID,
                                vector<RoleView *>&enemyRoleVec,
                                vector<RoleView *>&friendRoleVec,
                                vector<Script> &animationVec,
                                QWidget *parent,
                                int &mapID,
                                int &winRequirement,
                                int &loseRequirement,
                                //int &winCash,
                                //QString &winGoods,
                                //QString &winText,
                                //QString &loseText,
                                int &startLocationX,
                                int &startLocationY,
                                int heroLevel)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/battle.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    assert(enemyRoleVec.empty());
    assert(friendRoleVec.empty());
    assert(animationVec.empty());
    if(sqlite3_prepare_v2(
                db,
                QString("select * from battle_%1;").arg(battleID).toUtf8().data(),
                -1,
                &statement,
                NULL)!=SQLITE_OK)
    {
        //qDebug()<<sqlite3_errmsg(db);
        return false;
    }

    QString param;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        param=QString::fromUtf8((char *)(sqlite3_column_text(statement,1)));
        switch(sqlite3_column_int(statement,0))
        {
        case static_cast<int>(BattleEnemy):
            enemyRoleVec.push_back(new RoleView(parent,
                                                getSubFromQString(param,1,'#').toInt(),
                                                getSubFromQString(param,2,'#').toInt(),
                                                getSubFromQString(param,3,'#').toInt()));
            break;
        case static_cast<int>(BattleFriend):
            friendRoleVec.push_back(new RoleView(parent,
                                                getSubFromQString(param,1,'#').toInt(),
                                                getSubFromQString(param,2,'#').toInt(),
                                                getSubFromQString(param,3,'#').toInt()));
            break;
        case static_cast<int>(BattleSceneAnimation):
            getScriptFromQString(animationVec,param);
            break;
        case static_cast<int>(BattleSceneMapID):
            mapID=param.toInt();
            break;
        case static_cast<int>(BattleSceneWinRequirement):
            winRequirement=param.toInt(NULL,16);
            break;
        case static_cast<int>(BattleSceneLoseRequirement):
            loseRequirement=param.toInt(NULL,16);
            break;
        case static_cast<int>(BattleSceneHeroStartLocation):
            startLocationX=(getSubFromQString(param,1,'#').toInt());
            startLocationY=(getSubFromQString(param,2,'#').toInt());
            break;
//        case static_cast<int>(BattleSceneWinCash):
//            winCash=param.toInt();
//            break;
//        case static_cast<int>(BattleSceneWinGoods):
//            winGoods=param;
//            break;
//        case static_cast<int>(BattleSceneWinText):
//            winText=param;
//            break;
//        case static_cast<int>(BattleSceneLoseText):
//            loseText=param;
//            break;
        default:
            assert(0);
            break;
        }
    }
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    /****接下来从battle_role_XX中读出enemy的Role数据*****/
    Role *role=NULL;
    for(int i=0;i<enemyRoleVec.size();++i)
    {
        if(sqlite3_prepare_v2(
                    db,
                    QString("select * from battle_role_%1 where id=%2;").arg(battleID)
                    .arg(enemyRoleVec[i]->getID()).toUtf8().data(),
                    -1,
                    &statement,
                    NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_step(statement)!=SQLITE_ROW)
            return false;

        role=new Role(sqlite3_column_int(statement,0),//id
                      sqlite3_column_int(statement,1),//job
                      sqlite3_column_int(statement,2),//level
                      sqlite3_column_int(statement,3),//hp
                      sqlite3_column_int(statement,3),//hpmax
                      -1,//exp
                      -1,//expmax
                      sqlite3_column_int(statement,4),//pa
                      sqlite3_column_int(statement,5),//pd
                      sqlite3_column_int(statement,6),//iq
                      sqlite3_column_int(statement,7),//accurcy
                      sqlite3_column_int(statement,8),//speed
                      sqlite3_column_int(statement,9),//dodge
                      sqlite3_column_int(statement,10),//range
                      sqlite3_column_int(statement,11),//weapon0
                      0,0,0,//weapon
                      0,0,0,0,//medicine
                      0,
                      0,
                      0//crit
                      );
        if(role->level==0)
        {//等级为0意味着需要通过heroLevel构造
            //因为是非线性,查表法构造
            role->level=heroLevel;
            role->HP=HPA[static_cast<int>(role->job)]
                    +(role->level-HPB[static_cast<int>(role->job)])*4
                    +getRandomNumberIn(-2,2);
            role->HPMax=role->HP;
            role->physicalAttack=physicalAttackA[static_cast<int>(role->job)]
                    +(role->level-physicalAttackB[static_cast<int>(role->job)])*3
                    +getRandomNumberIn(-3,3);
            role->physicalDefence=physicalDefenceA[static_cast<int>(role->job)]
                    +(role->level-physicalDefenceB[static_cast<int>(role->job)])
                    +getRandomNumberIn(-1,1);
            role->IQ=IQ[static_cast<int>(role->job)]
                    +getRandomNumberIn(-10,10);
            role->accurcy=ACCURCY[static_cast<int>(role->job)]
                    +getRandomNumberIn(-3,3);
            role->dodge=DODGE[static_cast<int>(role->job)]
                    +getRandomNumberIn(-1,1);;
            role->speed=SPEED[static_cast<int>(role->job)];
            role->range=RANGE[static_cast<int>(role->job)];
            role->weapon[0]=WEAPON0[static_cast<int>(role->job)];
            /*
            switch(role->job)
            {
            case JIROUNAN:
                role->HP=70+role->level*4;
                role->HPMax=role->HP;
                role->physicalAttack=70+role->level*3;
                role->physicalDefence=40+role->level;
                role->IQ=99;
                role->accurcy=95;
                role->dodge=1;
                role->speed=8;
                role->range=1;
                break;
            case BAOLINAN:
                role->physicalAttack=130+(role->level-20)*3;
                role->physicalDefence=60+role->level-20;
                role->HP=160+(role->level-20)*4;
                role->HPMax=role->HP;
                role->IQ=99;
                role->accurcy=95;
                role->dodge=2;
                role->speed=8;
                role->range=2;
                break;
            case QIGONGSHI:
                role->physicalAttack=190+(role->level-40)*3;
                role->physicalDefence=80+role->level-40;
                role->HP=250+(role->level-40)*4;
                role->HPMax=role->HP;
                role->IQ=99;
                role->accurcy=95;
                role->dodge=2;
                role->speed=8;
                role->range=2;
                break;
            case NVHANZI:
                role->physicalAttack=63+role->level*3;
                role->physicalDefence=29+role->level;
                role->HP=65+role->level*4;
                role->HPMax=role->HP;
                role->IQ=99;
                role->accurcy=90;
                role->hedge=0;
                role->speed=8;
                role->range=2;
                break;
            case BAWANGHUA:
                role->physicalAttack=123+(role->level-20)*3;
                role->physicalDefence=49+role->level-20;
                role->HP=130+(role->level-20)*4;
                role->HPMax=role->HP;
                role->IQ=99;
                role->accurcy=95;
                role->hedge=0;
                role->speed=8;
                role->range=2;
                break;
            case NVWANG:
                role->physicalAttack=183+(role->level-40)*3;
                role->physicalDefence=69+role->level-40;
                role->HP=210+(role->level-40)*4;
                role->HPMax=role->HP;
                role->accurcy=95;
                role->hedge=0;
                role->speed=8;
                role->range=2;
                break;
            case FUERDAI:
                role->physicalAttack=60+role->level*3;
                role->physicalDefence=32+role->level;
                role->HP=80+role->level*4;
                role->HPMax=role->HP;
                role->accurcy=95;
                role->hedge=10;
                role->speed=8;
                role->range=2;
                break;
            case TUHAO:
                role->physicalAttack=120+(role->level-20)*3;
                role->physicalDefence=59+role->level-20;
                role->HP=150+(role->level-20)*4;
                role->HPMax=role->HP;
                role->accurcy=95;
                role->hedge=10;
                role->speed=8;
                role->range=2;
                break;
            case GAOFUSHUAI:
                role->physicalAttack=160+(role->level-40)*3;
                role->physicalDefence=79+role->level-40;
                role->HP=200+(role->level-40)*4;
                role->HPMax=role->HP;
                role->accurcy=95;
                role->hedge=10;
                role->speed=8;
                role->range=2;
                break;
            case ZHAINAN:
                role->physicalAttack=55+role->level*3;
                role->physicalDefence=35+role->level;
                role->HP=55+role->level*4;
                role->HPMax=role->HP;
                role->accurcy=96;
                role->hedge=2;
                role->speed=8;
                role->range=2;
                break;
            case JISHUZHAI:
                role->physicalAttack=115+(role->level-20)*3;
                role->physicalDefence=55+role->level-20;
                role->HP=120+(role->level-20)*4;
                role->HPMax=role->HP;
                accurcy=99;
                hedge=6;
                speed=8;
                range=2;
                break;
            case JISHUSHEN:
                physicalAttack=175+(level-40)*3;
                physicalDefence=75+level-40;
                HP=190+(level-40)*4;
                HPMax=HP;
                magicAttack=99;
                magicDefence=49;
                accurcy=99;
                hedge=8;
                speed=8;
                range=2;
                break;
            case XUEZHA:
                physicalAttack=55+level*3;
                physicalDefence=30+level;
                HP=65+level*4;
                HPMax=HP;
                magicAttack=85;
                magicDefence=15;
                accurcy=95;
                hedge=0;
                speed=8;
                range=2;
                break;
            case ZIXINAN:
                physicalAttack=115+(level-20)*3;
                physicalDefence=50+level-20;
                HP=125+(level-20)*4;
                HPMax=HP;
                magicAttack=90;
                magicDefence=20;
                accurcy=95;
                hedge=2;
                speed=8;
                range=2;
                break;
            case XUEBA:
                physicalAttack=175+(level-40)*3;
                physicalDefence=70+level-40;
                HP=200+(level-40)*4;
                HPMax=HP;
                magicAttack=95;
                magicDefence=25;
                accurcy=95;
                hedge=5;
                speed=8;
                range=2;
                break;
            case MEIZHI:
                physicalAttack=50+level*3;
                physicalDefence=20+level;
                HP=55+level*4;
                HPMax=HP;
                magicAttack=75;
                magicDefence=35;
                accurcy=95;
                hedge=5;
                speed=8;
                range=2;
                break;
            case MENGMEIZHI:
                physicalAttack=110+(level-20)*3;
                physicalDefence=40+level-20;
                HP=110+(level-20)*4;
                HPMax=HP;
                magicAttack=80;
                magicDefence=40;
                accurcy=99;
                hedge=10;
                speed=8;
                range=2;
                break;
            case NVSHEN:
                physicalAttack=170+(level-40)*3;
                physicalDefence=60+level-40;
                HP=180+(level-40)*4;
                HPMax=HP;
                magicAttack=85;
                magicDefence=45;
                accurcy=99;
                hedge=15;
                speed=8;
                range=2;
                break;
            case DAMA:
                physicalAttack=65+level*3;
                physicalDefence=35+level;
                HP=65+level*4;
                HPMax=HP;
                magicAttack=55;
                magicDefence=5;
                accurcy=95;
                hedge=10;
                speed=8;
                range=2;
                break;
            case CAIDAOMA:
                physicalAttack=125+(level-20)*3;
                physicalDefence=55+level-20;
                HP=150+(level-20)*4;
                HPMax=HP;
                magicAttack=56;
                magicDefence=10;
                accurcy=97;
                hedge=10;
                speed=8;
                range=2;
                break;
            case ZHANDOUMA:
                physicalAttack=185+(level-40)*3;
                physicalDefence=75+level-40;
                HP=230+(level-40)*4;
                HPMax=HP;
                magicAttack=60;
                magicDefence=12;
                accurcy=99;
                hedge=10;
                speed=8;
                range=2;
                break;
            case DASHU:
                physicalAttack=63+level*3;
                physicalDefence=32+level;
                HP=60+level*4;
                HPMax=HP;
                magicAttack=85;
                magicDefence=35;
                accurcy=90;
                hedge=2;
                speed=8;
                range=2;
                break;
            case SHUSHU:
                physicalAttack=123+(level-20)*3;
                physicalDefence=52+level-20;
                HP=150+(level-20)*4;
                HPMax=HP;
                magicAttack=87;
                magicDefence=37;
                accurcy=93;
                hedge=2;
                speed=8;
                range=2;
                break;
            case GUAISHUSHU:
                physicalAttack=183+(level-40)*3;
                physicalDefence=69+level-40;
                HP=210+(level-40)*4;
                HPMax=HP;
                magicAttack=89;
                magicDefence=39;
                accurcy=95;
                hedge=2;
                speed=8;
                range=2;
                break;
            case DIAOSI:
                physicalAttack=50+level*3;
                physicalDefence=34+level;
                HP=60+level*4;
                HPMax=HP;
                magicAttack=65;
                magicDefence=20;
                accurcy=95;
                hedge=3;
                speed=8;
                range=2;
                break;
            case DIAOBAO:
                physicalAttack=110+(level-20)*5;
                physicalDefence=54+2*(level-20);
                HP=110+(level-20)*8;
                HPMax=HP;
                magicAttack=70;
                magicDefence=26;
                accurcy=95;
                hedge=7;
                speed=8;
                range=2;
                break;
            case NIXI:
                physicalAttack=220+(level-40)*3;
                physicalDefence=90+level-40;
                HP=280+(level-40)*4;
                HPMax=HP;
                magicAttack=75;
                magicDefence=30;
                accurcy=99;
                hedge=20;
                speed=8;
                range=2;
                break;
            default:
                qDebug()<<(role->job);
                assert(0);
            }
        */
        }

        enemyRoleVec[i]->setRolePointer(role);
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    }
    for(int i=0;i<friendRoleVec.size();++i)
    {
        if(sqlite3_prepare_v2(
                    db,
                    QString("select *from battle_role_%1 where id=%2").arg(battleID)
                    .arg(friendRoleVec[i]->getID()).toUtf8().data(),
                    -1,
                    &statement,
                    NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_step(statement)!=SQLITE_ROW)
            return false;
        role=new Role(sqlite3_column_int(statement,0),//id
                      sqlite3_column_int(statement,1),//job
                      sqlite3_column_int(statement,2),//level
                      sqlite3_column_int(statement,3),//hp
                      sqlite3_column_int(statement,3),//hpmax
                      -1,//exp
                      -1,//expmax
                      sqlite3_column_int(statement,4),//pa
                      sqlite3_column_int(statement,5),//pd
                      sqlite3_column_int(statement,6),//iq
                      sqlite3_column_int(statement,7),//accurcy
                      sqlite3_column_int(statement,8),//speed
                      sqlite3_column_int(statement,9),//dodge
                      sqlite3_column_int(statement,10),//range
                      sqlite3_column_int(statement,11),//weapon0
                      0,0,0,//weapon
                      0,0,0,0,//medicine
                      0,
                      0,
                      0//crit
                      );
        friendRoleVec[i]->setRolePointer(role);
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    }
    return true;
}

bool DBIO::getBattleWinFailTextFromLib(int id,QString &win,QString &fail)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/battle.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,QString("select win,fail from battletext where id=%1;")
                          .arg(id).toUtf8().data(),
                          -1,
                          &statement,
                          NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
    {//没找到相应的记录,用默认记录代替
        if(sqlite3_finalize(statement)!=SQLITE_OK)
            return false;
        if(sqlite3_prepare_v2(db,"select win,fail from battletext where id=0;",
                              -1,
                              &statement,
                              NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_step(statement)!=SQLITE_ROW)
            return false;
    }
    win=QString::fromUtf8((char*)sqlite3_column_text(statement,0));
    fail=QString::fromUtf8((char *)sqlite3_column_text(statement,1));
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;
    return true;
}

bool DBIO::getRoleInfoFromLibOfID(Role **roleInfoArray,int id)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/global.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    QString sql=QString("select * from roleinfo where id=%1;").arg(id);
    if(sqlite3_prepare_v2(db,sql.toUtf8().data(),-1,&statement,NULL)!=SQLITE_OK)return false;
    Role *newrole=NULL;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;

    newrole=new Role(id,//id
                     sqlite3_column_int(statement,1),//pro
                     sqlite3_column_int(statement,2),//level
                     sqlite3_column_int(statement,3),//hp
                     sqlite3_column_int(statement,4),//hpmax
                     sqlite3_column_int(statement,5),//exp
                     sqlite3_column_int(statement,6),//expmax
                     sqlite3_column_int(statement,7),//pa
                     sqlite3_column_int(statement,8),//pd
                     sqlite3_column_int(statement,9),//iq
                     sqlite3_column_int(statement,10),//acc
                     sqlite3_column_int(statement,11),//speed
                     sqlite3_column_int(statement,12),//dodge
                     sqlite3_column_int(statement,13),//range
                     sqlite3_column_int(statement,14),//weapon
                     sqlite3_column_int(statement,15),
                     sqlite3_column_int(statement,16),
                     sqlite3_column_int(statement,17),
                     sqlite3_column_int(statement,18),//medicine
                     sqlite3_column_int(statement,19),
                     sqlite3_column_int(statement,20),
                     sqlite3_column_int(statement,21),
                     sqlite3_column_int(statement,22),//shield
                     sqlite3_column_int(statement,23),//article
                     sqlite3_column_int(statement,24)//crit
                     );
    roleInfoArray[id-1]=newrole;

//    QString sql("select count(*) from sqlite_master where name='roleinfo';");
//    if(sqlite3_prepare_v2(db,sql.toUtf8().data(),-1,&statement,NULL)!=SQLITE_OK)return false;
//    sqlite3_step(statement);
//    qDebug()<<sqlite3_column_int(statement,0);

    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    return true;
}
bool DBIO::getGlobalGoodsIDFromLib(set<int> &goodsIDSet)
{
    //注意!!!!!!!!!!!!!!!!!!!!!!!这个函数还没有完成!!!!!!!!!!!!!!!!
    return true;
}
bool DBIO::getArticleInfoFromLib(int id,map<int,ArticleInfo> *pArticleIDToInfoMap)
{

    assert(pArticleIDToInfoMap->find(id)==pArticleIDToInfoMap->end());
    QString path=QApplication::applicationDirPath()+QString("/data/lib/global.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,
                          QString("select * from article where id=%1").arg(id).toUtf8().data(),
                          -1,
                          &statement,
                          NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;
    (*pArticleIDToInfoMap)[id]=ArticleInfo(QString::fromUtf8((char*)sqlite3_column_text(statement,1)),//name
                                           sqlite3_column_int(statement,2),//attack
                                           sqlite3_column_int(statement,3),//accurcy
                                           sqlite3_column_int(statement,4),//price
                                           sqlite3_column_int(statement,5),//special
                                           QString::fromUtf8((char*)sqlite3_column_text(statement,6))
                                           );
    if(sqlite3_finalize(statement))return false;
    return true;
}
bool DBIO::getBattleWinScriptFromLib(int battleID,vector<Script>&scriptVec)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/battle.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,
                          QString("select param from battlewin where id=%1").arg(battleID).toUtf8().data(),
                          -1,
                          &statement,
                          NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;
    QString str=QString::fromUtf8((char*)sqlite3_column_text(statement,0));
    getScriptFromQString(scriptVec,str);
    return true;
}
bool DBIO::getBattleFailScriptFromLib(int battleID,vector<Script>&scriptVec)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/battle.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,
                          QString("select param from battlefail where id=%1").arg(battleID).toUtf8().data(),
                          -1,
                          &statement,
                          NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;
    QString str=QString::fromUtf8((char*)sqlite3_column_text(statement,0));
    getScriptFromQString(scriptVec,str);
    return true;
}
bool DBIO::getStorySceneDataFromSaveFile(QWidget*parent,
                                         QString const &fileName,
                                         StorySceneData &storySceneData,
                                         vector<RoleView*>&roleViewVec,
                                         Role **roleInfoArray,
                                         set<uint>&gotArticleSet,
                                         multiset<int>&carriedArticleIDSet,
                                         int **mapArray)
{
    QString path=QApplication::applicationDirPath()+QString("/save/")+fileName;
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;

    //*************读取scenedata
    if(sqlite3_prepare_v2(db,"select data from storyscene where item='scene';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    const StorySceneData *data=reinterpret_cast<const StorySceneData*>(sqlite3_column_blob(statement,0));
    memcpy(&storySceneData,data,sizeof(StorySceneData));
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //********************读取所有的RoleView********************
    if(sqlite3_prepare_v2(db,"select data from storyscene where item like 'role%%';",-1,&statement,NULL)!=SQLITE_OK)return false;
    uchar *roleData=NULL;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        roleData=(uchar *)(sqlite3_column_blob(statement,0));

        roleViewVec.push_back(new RoleView(parent,roleData));
    }
    if(sqlite3_finalize(statement))return false;

    //********************读取所有的Object********************
//    if(sqlite3_prepare_v2(db,"select data from storyscene where item like 'object%%';",-1,&statement,NULL)!=SQLITE_OK)return false;
//    uchar *objectData=NULL;
//    while(sqlite3_step(statement)==SQLITE_ROW)
//    {
//        objectData=(uchar *)(sqlite3_column_blob(statement,0));
//        //ofstream fs(QString("D:\\load%1.data").arg(i++).toAscii().data());
//        //fs.write((char *)objectData,sqlite3_column_bytes(statement,0));
//        //qDebug()<<sqlite3_column_bytes(statement,0);
//        objectVec.push_back(new InvisibleObject(parent,objectData));
//    }
//    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //********************读取mapArray********************
    if(sqlite3_prepare_v2(db,"select data from storyscene where item='maparray';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    int mapArrayBytes=sqlite3_column_bytes(statement,0);
    *mapArray=new int[mapArrayBytes/sizeof(int)];
    memcpy(*mapArray,sqlite3_column_blob(statement,0),mapArrayBytes);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //********************读取所有的RoleInfo********************
    if(sqlite3_prepare_v2(db,"select * from roleinfo;",-1,&statement,NULL)!=SQLITE_OK)return false;
    Role *newRole=NULL;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        assert(sqlite3_column_int(statement,0)<ROLENUMBER);
        newRole=new Role(*((Role*)sqlite3_column_blob(statement,1)));
        //用默认的拷贝构造函数，sqlite3_step会自动释放返回的内存数据
        roleInfoArray[sqlite3_column_int(statement,0)-1]=newRole;
    }
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //********************读取所有的articleid********************
    if(sqlite3_prepare_v2(
                db,
                "select data from storyscene where item='gotarticle';",
                -1,
                &statement,
                NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    uint *p=(uint *)sqlite3_column_blob(statement,0);
    int gotArticleSize=sqlite3_column_bytes(statement,0)/sizeof(uint);
    while(gotArticleSize--)
    {
        gotArticleSet.insert(*p++);
    }
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    if(sqlite3_prepare_v2(
                db,
                "select data from storyscene where item='carriedarticle';",
                -1,
                &statement,
                NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    int *q=(int *)sqlite3_column_blob(statement,0);
    int carriedArticleSize=sqlite3_column_bytes(statement,0)/sizeof(int);
    while(carriedArticleSize--)
    {
        carriedArticleIDSet.insert(*q++);
    }
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    return true;
}

/*
    //**********************读取script********************
    if(sqlite3_prepare_v2(db,"select data from storyscene where item='script';",-1,&statement,NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    uchar *scriptData=(uchar *)(sqlite3_column_blob(statement,0));
    uchar *currentPointer=scriptData;
    //获得script的实际数据
    int dataLength=sqlite3_column_bytes(statement,0);
    int scriptCount=*(reinterpret_cast<int *>(scriptData));
    currentPointer+=sizeof(int);

    AnimationOperation op;
    QString param;
    while(scriptCount--)
    {
        op=*(reinterpret_cast<AnimationOperation *>(currentPointer));
        currentPointer+=sizeof(AnimationOperation);
        param=QString::fromUtf8(reinterpret_cast<char *>(currentPointer));
        currentPointer+=(strlen(reinterpret_cast<char *>(currentPointer)+1));
        scriptVec.push_back(Script(op,param));
    }
    assert(scriptData+dataLength==currentPointer);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
*/


bool DBIO::getBattleDataFromSaveFile(QWidget *parent,
                                     vector<Script> &scriptVec,
                                     vector<RoleView*>&selfViewVec,
                                     vector<RoleView*>&enemyViewVec,
                                     vector<RoleView*>&friendViewVec,
                                     BattleSceneData &battleSceneData,
                                     int **mapArray,
                                     int &winRequirement,
                                     int &loseRequirement)
{
    assert(db);
    assert(scriptVec.empty()&&selfViewVec.empty()&&enemyViewVec.empty()&&friendViewVec.empty());
    /*
    //**********************读取script********************
    if(sqlite3_prepare_v2(db,"select data from battlescene where item='script';",-1,&statement,NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    uchar *scriptData=(uchar *)(sqlite3_column_blob(statement,0));
    uchar *currentPointer=scriptData;
    //获得script的实际数据
    uint dataLength=sqlite3_column_bytes(statement,0);
    uint scriptCount=*(reinterpret_cast<uint *>(scriptData));
    currentPointer+=sizeof(uint);

    AnimationOperation op;
    QString param;
    while(scriptCount--)
    {
        op=*(reinterpret_cast<AnimationOperation *>(currentPointer));
        currentPointer+=sizeof(AnimationOperation);
        param=QString::fromUtf8(reinterpret_cast<char *>(currentPointer));
        currentPointer+=(strlen(reinterpret_cast<char *>(currentPointer))+1);
        scriptVec.push_back(Script(op,param));
    }
    assert(scriptData+dataLength==currentPointer);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    */
    //********************读取所有的RoleView********************
    if(sqlite3_prepare_v2(db,"select data from battlescene where item like 'self%%';",-1,&statement,NULL)!=SQLITE_OK)return false;
    uchar *roleData=NULL;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        roleData=(uchar *)(sqlite3_column_blob(statement,0));
        selfViewVec.push_back(new RoleView(parent,roleData));
    }
    if(sqlite3_finalize(statement))return false;

    if(sqlite3_prepare_v2(db,"select data from battlescene where item like 'enemy%%';",-1,&statement,NULL)!=SQLITE_OK)return false;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        roleData=(uchar *)(sqlite3_column_blob(statement,0));
        enemyViewVec.push_back(new RoleView(parent,roleData));
    }
    if(sqlite3_finalize(statement))return false;

    if(sqlite3_prepare_v2(db,"select data from battlescene where item like 'friend%%';",-1,&statement,NULL)!=SQLITE_OK)return false;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        roleData=(uchar *)(sqlite3_column_blob(statement,0));
        friendViewVec.push_back(new RoleView(parent,roleData));
    }
    if(sqlite3_finalize(statement))return false;


    //********************读取battleSceneData********************
    if(sqlite3_prepare_v2(db,"select data from battlescene where item='scene';",-1,&statement,NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    memcpy(&battleSceneData,sqlite3_column_blob(statement,0),sizeof(BattleSceneData));
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //*******************读取flagData
//    if(sqlite3_prepare_v2(db,"select data from battlescene where item='flag';",-1,&statement,NULL)!=SQLITE_OK)return false;
//    sqlite3_step(statement);
//    length=sqlite3_column_bytes(statement,0);
//    (*flagData)=new uchar[length];
//    memcpy(*flagData,sqlite3_column_blob(statement,0),length);
//    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //****************读取mapArrayData
    if(sqlite3_prepare_v2(db,"select data from battlescene where item='maparray';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    int mapArrayBytes=sqlite3_column_bytes(statement,0);
    *mapArray=new int[mapArrayBytes/sizeof(int)];
    memcpy(*mapArray,sqlite3_column_blob(statement,0),mapArrayBytes);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;

    //此时关闭存档文件的数据库连接,打开lib文件读取相应数据
    sqlite3_close(db);

    QString path=QApplication::applicationDirPath()+QString("/data/lib/battle.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    //*****************胜利失败条件等*************
    if(sqlite3_prepare_v2(
                db,
                QString("select * from battle_%1 where type in (%2,%3);")
                .arg(battleSceneData.battleID)
                .arg(BattleSceneWinRequirement)
                .arg(BattleSceneLoseRequirement)
                .toUtf8().data(),
                -1,
                &statement,
                NULL)!=SQLITE_OK)
        return false;
    QString param;
    while(sqlite3_step(statement)==SQLITE_ROW)
    {
        param=QString::fromUtf8((char *)sqlite3_column_text(statement,1));
        switch(sqlite3_column_int(statement,0))
        {
        case static_cast<int>(BattleSceneWinRequirement):
            winRequirement=param.toInt(NULL,16);
            break;
        case static_cast<int>(BattleSceneLoseRequirement):
            loseRequirement=param.toInt(NULL,16);
            break;
        default:
            qDebug()<<param;
            assert(0);
        }

    }

    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_close(db)!=SQLITE_OK)return false;
    db=NULL;
    //现在db打开的实际上是库文件,为防止误用手动关闭,此时读档全部完成
    return true;
    //结束
}

bool DBIO::checkSaveFile(const QString &fileName, QString &time, int &roleNum)
{
    QString path=QApplication::applicationDirPath() +QString("/save/")+fileName;
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select count(*) from roleinfo;",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;
    roleNum=sqlite3_column_int(statement,0);
    if(roleNum==0)return false;
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;
    if(sqlite3_prepare_v2(db,"select value from global where item='time';",-1,&statement,NULL)!=SQLITE_OK)return false;
    time=QString::fromUtf8((const char *)sqlite3_column_text(statement,0));

    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    return true;
}

bool DBIO::isASaveFile(QString const &fileName,QString &time,int &roleNum)
{
    if(db)
        sqlite3_close(db);
    QString path=QApplication::applicationDirPath() +QString("/save/")+fileName;
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)
        return false;
//    sqlite3_prepare_v2(db,"select name from sqlite_master",-1,&statement,NULL);
//    while(sqlite3_step(statement)==SQLITE_ROW)
//    {
//        qDebug()<<(char *)sqlite3_column_text(statement,0);
//    }
//    return false;
    //sqlite3_prepare_v2(db,"select count(*) from roleinfo;",-1,&statement,(const char**)&error);
    //qDebug()<<error;
    if(sqlite3_prepare_v2(db,"select count (*) from roleinfo;",-1,&statement,NULL)!=SQLITE_OK)
    {
        //qDebug()<<sqlite3_errmsg(db);
        return false;
    }
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    roleNum=sqlite3_column_int(statement,0);
    assert(roleNum);
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;

    if(sqlite3_prepare(db,"select value from global where item='time'",-1,&statement,NULL)!=SQLITE_OK)
            return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;

    time=QString::fromUtf8((char *)sqlite3_column_text(statement,0));

    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;
    QDateTime t=QDateTime::fromString(time,"yyyyMMddhhmmss");
    time=t.toString("yyyy-MM-dd hh:mm:ss");
    return true;
}
bool DBIO::getKeySettings(int &upKey,int &downKey,int &leftKey,int &rightKey,
                               int &actionKey,int &cancelKey,int &menuKey)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/settings.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='up';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    upKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='down';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    downKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='left';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    leftKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='right';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    rightKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='action';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    actionKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='cancel';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    cancelKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    if(sqlite3_prepare_v2(db,"select * from setting where key='menu';",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
    menuKey=sqlite3_column_int(statement,1);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
}
bool DBIO::setKeySettings(int upKey, int downKey, int leftKey, int rightKey, int actionKey, int cancelKey, int menuKey)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/settings.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
    QString sql=QString("update setting set value=%1 where key='up'").arg(upKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    sql=QString("update setting set value=%1 where key='down'").arg(downKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    sql=QString("update setting set value=%1 where key='left'").arg(leftKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    sql=QString("update setting set value=%1 where key='right'").arg(rightKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    sql=QString("update setting set value=%1 where key='action'").arg(actionKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    sql=QString("update setting set value=%1 where key='cancel'").arg(cancelKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    sql=QString("update setting set value=%1 where key='menu'").arg(menuKey);
    if(sqlite3_exec(db,sql.toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)return false;
    return true;
}
bool DBIO::createSaveFile(const QString &filePathName, const QString &time, int version)
{
    if(sqlite3_open(filePathName.toUtf8().data(),&db)!=SQLITE_OK)return false;
    //创建数据表
    if(sqlite3_exec(db,"create table global(item text,value text);",NULL,NULL,NULL)!=SQLITE_OK)return false;
    if(sqlite3_exec(db,QString("insert into global values('time','%1');").arg(time).toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_exec(db,QString("insert into global values('version','%1');").arg(version).toUtf8().data(),NULL,NULL,NULL)!=SQLITE_OK)
        return false;
    return true;

}
bool DBIO::saveStorySceneToFile(const StorySceneData &sceneData,
                                vector<RoleView *> &roleVec,
                                set<uint> gotArticleSet,
                                multiset<int> carriedArticleIDSet,
                                Role **roleInfo,
                                int *mapArray,
                                int mapArraySize)
{
    assert(db);
    if(sqlite3_exec(db,"create table storyscene(item text,data blob);",NULL,NULL,NULL)!=SQLITE_OK)return false;

//**********************scenedata数据归档**************************
    if(sqlite3_prepare_v2(db,"insert into storyscene values('scene',?);",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_bind_blob(statement,1,(&sceneData),sizeof(sceneData),NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_DONE)return false;
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    /*
//**********************script数据归档**************************
    if(sqlite3_prepare_v2(db,"insert into storyscene values('script',?);",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_bind_blob(statement,1,scriptData,scriptDataLength,NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    */
//**********************roleview数据归档**************************
    uchar *pObjectData=NULL;
    int objectDataLength=0;

    for(int i=0;i<roleVec.size();++i)
    {
        roleVec[i]->saveToData(&pObjectData,objectDataLength);
        if(sqlite3_prepare_v2(db,
                           QString("insert into storyscene values('role%1',?);").arg(i).toUtf8().data(),
                           -1,
                           &statement,
                           NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_bind_blob(statement,1,pObjectData,objectDataLength,NULL)!=SQLITE_OK)return false;
        if(sqlite3_step(statement)!=SQLITE_DONE)return false;
        delete []pObjectData;
        pObjectData=NULL;
        objectDataLength=0;
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    }
//*************************mapArray数据归档*************************
    if(sqlite3_prepare_v2(db,
                       "insert into storyscene values('maparray',?);",
                       -1,
                       &statement,
                       NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_bind_blob(statement,1,mapArray,sizeof(int)*mapArraySize,NULL)!=SQLITE_OK)return false;
    if(sqlite3_step(statement)!=SQLITE_DONE)return false;
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
//**********************invisibleobject数据归档**************************
//    for(int i=0;i<objectVec.size();++i)
//    {
//        objectVec[i]->saveToData(&pObjectData,objectDataLength);
//        //ofstream fs(QString("D:\\save%1.data").arg(i).toAscii().data());
//        //fs.write((char *)pObjectData,objectDataLength);
//        if(sqlite3_prepare_v2(db,
//                           QString("insert into storyscene values('object%1',?);").arg(i).toUtf8().data(),
//                           -1,
//                           &statement,
//                           NULL)!=SQLITE_OK)return false;
//        if(sqlite3_bind_blob(statement,1,pObjectData,objectDataLength,NULL)!=SQLITE_OK)return false;
//        sqlite3_step(statement);
//        delete []pObjectData;
//        pObjectData=NULL;
//        objectDataLength=0;
//        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
//    }
//**********************roleinfo数据归档**************************
//    char sql[]="create table roleinfo(id integer,pro integer level integer,hp integer,hpmax integer,exp integer,expmax integer,"
//            "pa integer,pd integer,ma integer,md integer,accurcy integer,speed interger,hedge integer,range integer,"
//            "weapon0 integer,weapon1 integer,weapon2 integer,weapon3 integer,medicine0 integer,medicine1 integer,"
//            "medicine2 integer,medicine3 integer,shield integer,goods integer);";
    if(sqlite3_exec(db,"create table roleinfo(id integer,data blob);",NULL,NULL,NULL)!=SQLITE_OK)return false;
    for(int i=0;i<ROLENUMBER;++i)
    {
        if(roleInfo[i])
        {
            if(sqlite3_prepare_v2(db,
                               QString("insert into roleinfo values('%1',?);").arg(i+1).toUtf8().data(),
                               -1,
                               &statement,
                               NULL)!=SQLITE_OK)return false;
            if(sqlite3_bind_blob(statement,1,roleInfo[i],sizeof(Role),NULL)!=SQLITE_OK)return false;
            if(sqlite3_step(statement)!=SQLITE_DONE)return false;
            if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
        }
    }
//**********************article数据归档**************************
    if(!gotArticleSet.empty())//如果是空自然不需要存档
    {
        uint *gotArticleData=new uint[gotArticleSet.size()];
        uint *p=gotArticleData;
        for(set<uint>::iterator iter=gotArticleSet.begin();iter!=gotArticleSet.end();++iter,++p)
        {
            *p=*iter;
        }
        if(sqlite3_prepare_v2(db,
                           "insert into storyscene values('gotarticle',?);",
                           -1,
                           &statement,
                           NULL)!=SQLITE_OK)return false;
        if(sqlite3_bind_blob(statement,1,gotArticleData,sizeof(uint)*gotArticleSet.size(),NULL)!=SQLITE_OK)return false;
        if(sqlite3_step(statement)!=SQLITE_DONE)return false;
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
        delete []gotArticleData;
    }
    if(!carriedArticleIDSet.empty())
    {
        int *carriedArticleData=new int[carriedArticleIDSet.size()];
        int *p=carriedArticleData;
        for(multiset<int>::iterator iter=carriedArticleIDSet.begin();iter!=carriedArticleIDSet.end();++iter,++p)
        {
            *p=*iter;
        }
        if(sqlite3_prepare_v2(db,
                           "insert into storyscene values('carriedarticle',?);",
                           -1,
                           &statement,
                           NULL)!=SQLITE_OK)return false;
        if(sqlite3_bind_blob(statement,1,carriedArticleData,sizeof(int)*carriedArticleIDSet.size(),NULL)!=SQLITE_OK)return false;
        if(sqlite3_step(statement)!=SQLITE_DONE)return false;
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
        delete []carriedArticleData;
    }

    return true;
}
bool DBIO::saveBattleSceneToFile(const BattleSceneData &sceneData,
                                 vector<RoleView *> &selfViewVec,
                                 vector<RoleView *> &enemyViewVec,
                                 vector<RoleView *> &friendViewVec,
                                 int *mapArray,
                                 int mapArraySize)
{//battleScene存档必定发生在storyScene之后
    if(sqlite3_exec(db,"create table battlescene(item text,data blob);",NULL,NULL,NULL)!=SQLITE_OK)return false;
    //**********************scene数据归档**************************
    if(sqlite3_prepare_v2(db,"insert into battlescene values('scene',?);",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_bind_blob(statement,1,(&sceneData),sizeof(BattleSceneData),NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    /*
    //**********************script数据归档**************************
    if(sqlite3_prepare_v2(db,"insert into battlescene values('script',?);",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_bind_blob(statement,1,scriptData,scriptDataLength,NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    */
    //**********************roleview数据归档**************************
    uchar *pObjectData=NULL;
    int objectDataLength=0;

    for(int i=0;i<selfViewVec.size();++i)
    {
        selfViewVec[i]->saveToData(&pObjectData,objectDataLength);
        if(sqlite3_prepare_v2(db,
                              QString("insert into battlescene values('self%1',?);").arg(i).toUtf8().data(),
                              -1,
                              &statement,
                              NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_bind_blob(statement,1,pObjectData,objectDataLength,NULL)!=SQLITE_OK)return false;
        sqlite3_step(statement);
        delete []pObjectData;
        pObjectData=NULL;
        objectDataLength=0;
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    }


    for(int i=0;i<enemyViewVec.size();++i)
    {
        enemyViewVec[i]->saveToData(&pObjectData,objectDataLength);
        if(sqlite3_prepare_v2(db,
                              QString("insert into battlescene values('enemy%1',?);").arg(i).toUtf8().data(),
                              -1,
                              &statement,
                              NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_bind_blob(statement,1,pObjectData,objectDataLength,NULL)!=SQLITE_OK)return false;
        sqlite3_step(statement);
        delete []pObjectData;
        pObjectData=NULL;
        objectDataLength=0;
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    }


    for(int i=0;i<friendViewVec.size();++i)
    {
        friendViewVec[i]->saveToData(&pObjectData,objectDataLength);
        if(sqlite3_prepare_v2(db,
                              QString("insert into battlescene values('friend%1',?);").arg(i).toUtf8().data(),
                              -1,
                              &statement,
                              NULL)!=SQLITE_OK)
            return false;
        if(sqlite3_bind_blob(statement,1,pObjectData,objectDataLength,NULL)!=SQLITE_OK)return false;
        sqlite3_step(statement);
        delete []pObjectData;
        pObjectData=NULL;
        objectDataLength=0;
        if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
    }

//***********************mapArray数据归档*************************
    if(sqlite3_prepare_v2(db,"insert into battlescene values('maparray',?);",-1,&statement,NULL)!=SQLITE_OK)return false;
    if(sqlite3_bind_blob(statement,1,mapArray,sizeof(int)*mapArraySize,NULL)!=SQLITE_OK)return false;
    sqlite3_step(statement);
    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;



    return true;
}
bool DBIO::getExitInfoFromLib(int mapID,int inx,int iny,int &toMapID,int &toX,int &toY)
{
    QString path=QApplication::applicationDirPath()+QString("/data/lib/global.dat");
    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)
        return false;
    if(sqlite3_prepare_v2(db,QString("select tomap,tox,toy from exit where inmap=%1 and inx=%2 and iny=%3;")
                          .arg(mapID).arg(inx).arg(iny).toUtf8().data(),
                          -1,&statement,NULL)!=SQLITE_OK)
        return false;
    if(sqlite3_step(statement)!=SQLITE_ROW)
        return false;
    toMapID=sqlite3_column_int(statement,0);
    toX=sqlite3_column_int(statement,1);
    toY=sqlite3_column_int(statement,2);
    if(sqlite3_finalize(statement)!=SQLITE_OK)
        return false;
    return true;
}

//bool DBIO::getWeaponInfoFromLib(int weaponID,WeaponInfo **info)
//{
//    assert(weaponID);
//    QString path=QApplication::applicationDirPath()+QString("/data/lib/global.dat");
//    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
//    QString sql=QString("select * from weapon where id=%1").arg(weaponID);
//    if(sqlite3_prepare_v2(db,sql.toUtf8().data(),-1,&statement,NULL)!=SQLITE_OK)return false;
//    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
//    WeaponInfo *newInfo=new WeaponInfo();
//    newInfo->name=QString::fromUtf8((char *)sqlite3_column_text(statement,3));
//    newInfo->accurcy=sqlite3_column_int(statement,1);
//    newInfo->attack=sqlite3_column_int(statement,2);
//    newInfo->description=QString::fromUtf8((char *)sqlite3_column_text(statement,4));
//    *info=newInfo;
//    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
//    return true;
//}
//bool DBIO::getMedicineInfoFromLib(int medicineID, MedicineInfo **info)
//{
//    assert(medicineID);
//    QString path=QApplication::applicationDirPath()+QString("/data/lib/global.dat");
//    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
//    QString sql=QString("select * from medicine where id=%1").arg(medicineID);
//    if(sqlite3_prepare_v2(db,sql.toUtf8().data(),-1,&statement,NULL)!=SQLITE_OK)return false;
//    if(sqlite3_step(statement)!=SQLITE_ROW)return false;
//    MedicineInfo *newInfo=new MedicineInfo();
//    newInfo->name=QString::fromUtf8((char *)sqlite3_column_text(statement,1));
//    newInfo->description=QString::fromUtf8((char *)sqlite3_column_text(statement,2));
//    *info=newInfo;
//    if(sqlite3_finalize(statement)!=SQLITE_OK)return false;
//    return true;
//}

//int **DBIO::getMapArrayPointerFromBattleID(int battleID,int mapSizeX,int mapSizeY)
//{
//    QString path=QApplication::applicationDirPath()+QString("/data/lib/battle.dat");
//    if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return NULL;
//    if(sqlite3_prepare_v2(db,
//                          QString("select data from battlemap where id=%1;").arg(battleID).toUtf8().data(),
//                          -1,
//                          &statement,
//                          NULL)!=SQLITE_OK)return NULL;
//    if(sqlite3_step(statement)!=SQLITE_ROW)return NULL;
//    int *data=new int[mapSizeX*mapSizeY];
//    int **p=new int *[mapSizeY];
//    memcpy(data,sqlite3_column_blob(statement,0),sizeof(int)*mapSizeX*mapSizeY);
//    for(int i=0;i<mapSizeY;++i)
//    {
//        p[i]=data+mapSizeX*i;
//    }
//    if(sqlite3_finalize(statement)!=SQLITE_OK)return NULL;
//    return p;
//}
//bool DBIO::getArticleNameAndPriceFromLib(int id,int &price,QString &name)
//{
//    if(!db)
//    {
//        QString path=QApplication::applicationDirPath()+QString("/data/lib/global.dat");
//        if(sqlite3_open(path.toUtf8().data(),&db)!=SQLITE_OK)return false;
//    }
//    QString tableName;
//    if(id<WEAPONUPBOUND)
//    {
//        tableName="weapon";
//    }
//    else if(id<ARTICLEUPBOUND)
//    {
//        tableName="article";
//    }
//    else if(id<SHIELDUPBOUND)
//    {
//        tableName="shield";
//    }
//    else if(id<MEDICINEUPBOUND)
//    {
//        tableName="medicine";
//    }

//    if(sqlite3_prepare_v2(db,
//                          QString("select name,price from %1 where id=%2;").arg(tableName).arg(id).toUtf8().data(),
//                          -1,
//                          &statement,
//                          NULL)!=SQLITE_OK)
//        return false;
//    if(sqlite3_step(statement)!=SQLITE_ROW)
//        return false;
//    name=QString::fromUtf8((const char *)sqlite3_column_text(statement,0));
//    price=sqlite3_column_int(statement,1);
//    if(sqlite3_finalize(statement)!=SQLITE_OK)
//        return false;
//    return true;

//}
