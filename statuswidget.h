#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include "lgl.h"
#include "Role.h"
#include "roleinfowidget.h"
#include "dbio.h"
#include "weapon.h"
#define AVAILABLEARTICLECOUNT 4
typedef enum
{
    StatusFocusOnTabIndex,//焦点在标签选择页上
    StatusFocusOnEquipment,//焦点在装备区
    StatusFocusOnItemSelected//焦点在物品上,选择装备/卸下/使用
}StatusFocusOn;
typedef enum
{
    StatusRoleInfo=0,
    StatusWeapon=1,
    StatusShield=2,
    StatusMedicine=3,
    StatusArticle=4
}StatusTabIndex;

class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent,
                          Role **roleInfo,
                          KeyboardControl *key,
                          multiset<int> &carriedArticle,
                          map<int,ArticleInfo> &articleIDToInfoMap,
                          int &carriedCash);
    
signals:

public slots:
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void onActionKey();
    void onCancelKey();
    void onDirectionKey(DIRECTION dir);
    void resetAvailableArticle();
    void updateSelectedItemID();

    KeyboardControl* key;
    Role **roleInfo;
    map<int,ArticleInfo> *pArticleIDToInfoMap;
    multiset<int> *pCarriedArticleSet;
    set<int>availableArticleSet;//筛选出可用的物品ID
    set<int>::iterator availableArticleBeginIter;//可用物品区首条记录对应的set迭代器
    //set<int>::iterator availableArticleEndIter;//可用物品区末条记录对应的迭代器的下一条,对于可用数量少于4的情况,这个值始终为availableset.end()
    RoleInfoWidget *infoWidget;
    int currentRoleID;
    StatusTabIndex currentTabIndex;//0:roleinfo 1:weapon 2:equipment 3:medicine 4:article
    //StatusTabIndex previousTabIndex;//从其他tab切换到roleinfo的时候能保持之前的状态
    int currentEquipCount;//当前装备的数量,最大为4
    int currentAvailableCount;//当前拥有的可有物品数量,合并同类项
    int currentItemIndex;//当该值取1~equitCount时光标在equip区中,equipCount+1~equipCoun+4时在available区中
    int currentSelectedItemID;//当焦点在装备页上时这个值是0
    int carriedCash;
    //bool itemSelected;
    StatusFocusOn focus;
    int currentItemSelectedIndex;//0:装备 1:卸下 2:使用
    int currentItemSelectedCount;//被选中的条目有几条子菜单,例如已经装备的物品子菜单数量=2(卸下/使用)
    QPoint upArrow[3];//向上的箭头
    QPoint downArrow[3];//向下的箭头
    QPoint rightArrow[3];//向右的箭头
};

#endif // STATUSWIDGET_H
