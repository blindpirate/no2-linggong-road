#ifndef BATTLEROLESELECTSCENE_H
#define BATTLEROLESELECTSCENE_H

#include "lgl.h"
#include "role.h"
#include "roleinfowidget.h"
#define SELECTCOLUMNNUMBER 5
#define SELECTROWNUMBER 4
#define ROLEPICSIZE 64

typedef struct RoleAndSelect
{
    RoleAndSelect(int id,bool isSelected):
        id(id),
        isSelected(isSelected){}
    int id;
    bool isSelected;
}RoleAndSelect;



class BattleRoleSelectScene : public QWidget
{
    Q_OBJECT
public:
    explicit BattleRoleSelectScene(QWidget *parent,KeyboardControl *key,Role**roleInfo,int battleID);
    ~BattleRoleSelectScene();
signals:
    void startBattle(vector<int> );
    
public slots:
private:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void keyPressEvent(QKeyEvent *);
    QPixmap rolePic;
    QPixmap rolePicWB;
    QPixmap cursor;
    KeyboardControl *key;
    Role **roleInfo;
    vector<RoleAndSelect> roleVec;
    //存放当前存在的roleid和是否被选中信息
    int currentIndex;
    int maxNum;//选择人数的上限
    int currentSelectedNum;
    RoleInfoWidget roleInfoWidget;
    QString battleName;//五舍之战
    QFont font;
    //bool roleSelected[ROLENUMBER];
    //某个id的人是否被选中,注意id是索引+1

    //char roleArray[20];
    //char currentIndex;
    //vector<Role*> roleVec;
    //vector<QPixmap*> rolePicVec;
    //vector<QPixmap*> rolePicWBVec;//黑白与彩色人物,与roleVec严格对应
};

#endif // BATTLEROLESELECTSCENE_H
