/*************************************
  该类封装了role的属性/持有物品等操作
**************************************/
#ifndef ROLE_H
#define ROLE_H

#include "lgl.h"

struct Role
{
    Role(int id,int job,int level,int HP,int HPMax,int exp,int expMax,int physicalAttack,int physicalDefence,
         int IQ,int accurcy,int speed,int dodge,int range,
         int weapon0,int weapon1,int weapon2,int weapon3,
         int medicine0,int medicine1,int medicine2,int medicine3,
         int shield,int article,int crit);
    ~Role()
    {
//        delete weapon[0];
//        delete weapon[1];
//        delete weapon[2];
//        delete weapon[3];
//        delete medicine[0];
//        delete medicine[1];
//        delete medicine[2];
//        delete medicine[3];
    }
//    int getID(){return ID;}
//    int getLevel(){return level;}
//    int getHP(){return HP;}
//    int getHPMax(){return HPMax;}
//    int getExp(){return exp;}
//    int getExpMax(){return expMax;}
//    int getPhysicalAttack(){return physicalAttack;}
//    int getPhysicalDefence(){return physicalDefence;}
//    int getMagicAttack(){return magicAttack;}
//    int getMagicDefence(){return magicDefence;}
//    int getAccurcy(){return accurcy;}
//    int getSpeed(){return speed;}
//    int getHedge(){return hedge;}
//    int getRange(){return range;}
//    WeaponInfo * getWeaponInfo(int index){return weapon[index];}
//    MedicineInfo * getMedicineInfo(int index){return medicine[index];}
//    int getShield(){return shield;}
//    int getGoods(){return goods;}
//    QString getName(){return getRoleNameFromRoleID(ID);}
    QString getJob();
//signals:
    
//public slots:
//private:
    int ID;
    //QString name;
    ROLEJOB job;
    int level;
    int HP;
    int HPMax;//hp上限
    int exp;
    int expMax;//经验上限
    int physicalAttack;
    int physicalDefence;
    int IQ;
//    int magicAttack;
//    int magicDefence;
    int accurcy;
    int speed;
    int dodge;//躲闪
    int range;//射程
    int weapon[4];
    int medicine[4];
    int shield;
    int article;//可以携带4件武器,4种药物,1件物品和1件防御装备
    int crit;//暴击
};

/*
class EnemyRole:public QObject
{
private:
    int ID;//id只是用来标识图像
    PROFESSION pro;
    int level;
    int HP;
    int HPMax;
    int physicalAttack;
    int physicalDefence;
    int magicAttack;
    int magicDefence;
    int accurcy;
    int speed;
    int hedge;//躲闪
}
*/

#endif // ROLE_H
