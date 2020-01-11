/******
  各类角色的绘图实现
******/
#ifndef ROLEVIEW_H
#define ROLEVIEW_H

#include "lgl.h"
#include "role.h"
//#include "roleanimation.h"
#include "hpslotwidget.h"
#define ANIMATIONINTERVAL 200//值越大移动的越慢
#define ROLEMOVEINTERVAL 40//25fps
#define ROLEFASTMOVEINTERVAL 20//50fps
#define ROLEMOVEPIXEL 4
#define SWEATINTERVAL 400
#define FLASHINTERVAL 100
#define ROTATEINTERVAL 80


struct RoleViewData
{
    RoleViewData(int pointMapX,
                 int pointMapY,
                 int ID,
                 int currentIndexInPathVec,
                 int type,
                 ROLEPOSE currentPose,
                 DIRECTION dir,
                 bool isActive,
                 bool reactOnViewMove,
                 bool contactDirectly,
                 bool animationTimerOn,
                 bool moveTimerOn):
        pointX(pointMapX),
        pointY(pointMapY),
        ID(ID),
        currentIndexInPathVec(currentIndexInPathVec),
        type(type),
        currentPose(currentPose),
        dir(dir),
        isActive(isActive),
        reactOnViewMove(reactOnViewMove),
        contactDirectly(contactDirectly),
        animationTimerOn(animationTimerOn),
        moveTimerOn(moveTimerOn)
    {}
    int pointX;//左上角点坐标
    int pointY;
    int ID;
    int currentIndexInPathVec;
    int type;
    ROLEPOSE currentPose;
    DIRECTION dir;
    bool isActive;
    bool reactOnViewMove;
    bool contactDirectly;
    bool animationTimerOn;
    bool moveTimerOn;
};

//typedef enum RoleAnimationType
//{
//    RoleNoAnimation,
//    RoleSweat,
//    RoleAttack,
//    RoleRecover,
//    RoleHurt,
//    RoleDie,
//    RoleDodge
//}RoleAnimationType;

class RoleView : public QWidget
{
    Q_OBJECT
public:
    explicit RoleView(QWidget *parent,int mapX,int mapY,int roleID);
    explicit RoleView(QWidget *parent,uchar *data);
    //这段内存由调用者释放
    //从存档中读取出完整的
    //指定roleID之后,它的图像和类型就完全确定
    //param2是path,
    ~RoleView();
    void saveToData(uchar **data,int &length);
    //序列化为一段内存
    int getPointX(){return pointMapX;}//view的点坐标
    int getPointY(){return pointMapY;}
    void setPose(ROLEPOSE newPose){currentPose=newPose;}
    void setViewPointXY(int x,int y);
    void setPointXY(int x,int y);
    void setOriginalState();//用于在战斗场景中,如果按cancel键,自身状态恢复默认
    int getMapX(){return mapX;}
    int getMapY(){return mapY;}
    int getType(){return type;}
    int getID(){return ID;}
    DIRECTION getDirection(){return dir;}
    void setReactOnViewMove(bool value){reactOnViewMove=value;}
    bool getReactOnViewMove(){return reactOnViewMove;}
    Role *getRolePointer()const{return role;}
    void setRolePointer(Role **roleInfo){role=roleInfo[ID-1001];}
    void setRolePointer(Role *newRole){role=newRole;}
    void setMapArray(int **p){mapArray=p;}
    void startMoving(bool fastMoving);//令role按照给定的path移动,true时快速移动
    void moveToPointOfMap(int mapX,int mapY);//将自身移动到map的xy处
    void startAnimation(DIRECTION);
    void stopAnimation();
    void show();
    void rewalk();//动画结束后,令NPC重新开始行走
    //void hide();
    void changeDir(DIRECTION);
    //void sweat(bool endWithSignal);//流汗,true为完成后发射信号,false为不发送,这样就可以实现多动画同时进行而只需要发射一个信号
    //void attack();//只是展示动画效果
    //void recover(int originalHP,int deltaHP,int fullHP,bool endWithSignal);//原始体力,回复体力,满体力
    //void hurt(int originalHP,int deltaHP,int fullHP,bool endWithSignal);//原始体力,扣减体力,满体力
    //void die();
    void dodge(bool endWithSignal);//闪避
    void flash();//闪烁
    void rotate();
    void setPathVec(QString);
    //inline void loadInactiveImage();
    //因为有些角色是不需要加载不活动图像的
    void setPathVec(vector<QPoint> *pVec);
    void onContact(vector<Script> &vec,DIRECTION dir);
    //dir是当前主角的方向
    bool canBeContactedDirectly(){return contactDirectly;}
    void setContactDirectly(bool value){contactDirectly=value;}
    void setScriptVec(QString param);
    //void constructRolePointer(int level);
//    void setControlViewMove(bool value){controlViewMove=value;}

    bool getActiveStatus(){return isActive;}
public slots:
    void changeActiveStatusTo(bool value);
    void onViewMovedTo(int x,int y);//若视角移动到x,y像素点处,需要自身作出一定的调整
    //void onSweatDestroyed();//接收sweat的销毁信号发射sweatEnd信号
    void onAnimationEnd();
signals:
    void roleAnimationEnd();
    void AIMoved(int,int);
    void roleDisappear(int x,int y);
    //void sendScript(vector<Script> *p);
    //void startAnimation();
    /*
    void hasStoppedMoving();//移动(而不是动画)事件结束后发出
    void hasStoppedSweat();//流汗事件结束后发出
    void hasStoppedAttack();//攻击动画结束后发出
    void hasStoppedRecover();//回复体力动画结束后发出
    void hasStoppedHurt();//受伤动画结束后发出
    void hasStoppeDodge();//闪避结束后发出
    void hasStoppedDie();//死亡结束后发出
    */

private:
    int currentViewPointX;
    int currentViewPointY;
    int **mapArray;
    vector<QPoint> *pPathVec;//移动路径点vector,循环显示,到达最后一个点之后重新从第一个点开始移动.如果不需要循环移动,则最后一个点坐标是(-1,-1)
    int currentIndexInPathVec;
    int pointMapX;
    int pointMapY;
    int mapX;//在地图中的坐标
    int mapY;
    int ID;
    int type;
    QPixmap *activeImage;//自身图像
    QPixmap *inactiveImage;
    ROLEPOSE currentPose;
    DIRECTION dir;
    //RoleAnimationType animationType;//这个标志位是为了在动画结束后
    int animationTimerID;//自身动画效果的计时器,同时也是动画效果的标志
    int moveTimerID;//在视图中移动的定时器
    int flashTimerID;//闪烁定时器
    int rotateTimerID;
    int rotateCount;//旋转计数
    bool isActive;//是否是活动的
    bool reactOnViewMove;//true的时候响应ViewHasMoveTo信号,此时不是focusRole;
    bool beingContacted;
    //bool hidden;//当battlescene开始时,storyscene中的NPC等处于hidden状态.
    //bool controlViewMove;//true的时候view将会随其移动
    Role *role;//在storyscene中,这个变量是NULL,在
    //有些情况下,如过场动画,需要hero响应viewMoveTo信号
    //hero不实现onContact函数,额外实现了onViewMoveTo函数
    //int sweatTimerID;
    //int sweatSize;//5x9,11x18,20x32
    //bool isMoving;
    bool contactDirectly;//这个标志位为真的话,只要移动的下一格是该view,就会触发onContact()函数,否则需要按action键才能触发onContact函数
private:

    void changePose();
    bool processNextArrayElement(int **,int,int,DIRECTION);
    //下一个元素可以行走,则交换两个元素,返回true,否则返回false

    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    vector<Script> *pScriptVec;

};

inline void RoleView::setViewPointXY(int x,int y)
{
    currentViewPointX=x;
    currentViewPointY=y;
}
inline void RoleView::setPointXY(int x,int y)
{
    pointMapX=x;
    pointMapY=y;
    mapX=x/BASEWIDTH;
    mapY=y/BASEHEIGHT;
}
inline void RoleView::setOriginalState()
{
    assert(isActive);
    reactOnViewMove=true;
    this->startAnimation(DOWN);
}
inline void RoleView::setPathVec(vector<QPoint> *pVec)
{
    if(pPathVec)delete pPathVec;
    pPathVec=pVec;
}
inline void RoleView::changeActiveStatusTo(bool value)
{
    if(value)
    {
        this->startAnimation(DOWN);
    }
    isActive=value;
    this->update();
}


/*
class Hero:public RoleView
{//在storyscene中的主角和battlescene中的所有角色都是Hero类的
    Q_OBJECT
public:
    explicit Hero(QWidget*parent,int mapX,int mapY,QString &,QString &);//两个参数分别是param1和param2
    ~Hero();
    void setReactOnViewMove(bool value){reactOnViewMove=value;}
    Role *getRole(){return role;}
    void attack();//只是展示动画效果
    void recovery(int,int,int);//原始体力,回复体力,满体力
    void hurt(int,int,int);//原始体力,扣减体力,满体力
    void die();
    void dodge();//闪避
    bool getActiveStatus(){return isActive;}
    AnimationOperation operation;//刚刚进行的动画操作,从而发出相应的信号
signals:

    void hasStoppedAttack();//攻击动画结束后发出
    void hasStoppedRecover();//回复体力动画结束后发出
    void hasStoppedHurt();//受伤动画结束后发出
    void hasStoppeDodge();//闪避结束后发出
    void hasStoppedDie();
    //void hasDied();
    //int getRoleWeaponID(int index){return role->getWeaponID(index);}
    //int getRoleMedicineID(int index){return role->getMedicineID(index);}
public slots:
    void onViewMovedTo(int x, int y);
    void onAnimationEnd();
//    void onAttackDestroyed();
//    void onRecoveryDestroyed();
//    void onHurtDestroyed();
//    void onDieDestroyed();
    void changeActiveStatusTo(bool status)
    {
        isActive=status;
        this->update();
    }
private:
    QPixmap *inactiveImage;
    bool isActive;//是否是活动的
    bool reactOnViewMove;//true的时候响应ViewHasMoveTo信号,此时不是focusRole;
    void onContact();
    Role *role;//在storyscene中,这个变量是NULL,在
    //有些情况下,如过场动画,需要hero响应viewMoveTo信号
    //hero不实现onContact函数,额外实现了onViewMoveTo函数
};

class NPC:public RoleView
{
    Q_OBJECT
public:
    explicit NPC(QWidget *parent,int mapX,int mapY,QString &,QString &);
    void onContact();
    //hero在地图上与npc接近时,由scene调用该函数,该函数根据自身特点向scene发射相应信号
public slots:
    //void onViewMovedTo(int x, int y);
signals:
    //void startConversation(QString);
    void startAnimation(vector<Script> &);
    //这个animation既可以包含对话又可以包含动画,还可以包含战斗
    //void startBattle(BattleParam &sth);
private:
    vector<Script>scriptVec;
};
*/

#endif // ROLEVIEW_H

