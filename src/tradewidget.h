#ifndef TRADEWIDGET_H
#define TRADEWIDGET_H
//该类实现物品的买卖
#include "lgl.h"
#include "Role.h"
#include "dbio.h"
#define SOLDRATE 0.5 //卖出物品折价比率
#define ARTICLECOUNT 10//买卖物品栏显示物品的数量

#define SELECTCOLUMNNUMBER 5
#define SELECTROWNUMBER 4
#define ROLEPICSIZE 64


typedef enum
{
    TradeSell,
    TradeBuy
}TradeType;

typedef enum
{
    TradeFocusOnBuyAndSell,
    TradeFocusOnArticle,
    TradeFocusOnItemSelected
}TradeFocus;



class TradeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TradeWidget(QWidget *parent,
                         KeyboardControl *key,
                         multiset<int> &carriedArticleSet,
                         QString const &soldArticle,
                         Role** roleInfo,
                         int *pCarriedCash,
                         map<int,ArticleInfo> &articleIDToInfoMap);
    //soldArticle形式是1#2#3#
    
signals:
    
public slots:
private:
    KeyboardControl *key;
    TradeType tradeType;
    TradeFocus focus;
    multiset<int> *pCarriedArticleSet;
    set<int> soldArticleSet;
    set<int> carriedArticleUniqueSet;
    set<int> *pCurrentDisplaySet;//当前显示的是哪个set
    //合并同类项后的article集合
    set<int>::iterator currentTopItemIter;
    //显示在第一行的物品条目的迭代器
    map<int,ArticleInfo> *pArticleIDToInfoMap;
    vector<int> itemCanBeUsedVec;//低8位是角色id,次8位是当前物品是否可以被使用的标志(0/1)
    Role **roleInfo;
    int *pCarriedCash;
    int currentSelectedItemIndex;//0~ARTICLECOUNT-1
    int currentBuySellCount;//买下和卖出的的数量
    int currentBuySellArticleID;//当前买卖的物品ID
    QPoint upArrow[3];
    QPoint downArrow[3];
    QFont font;
    QPixmap rolePic;
    QPixmap rolePicWB;

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void close();
    
};

#endif // TRADEWIDGET_H
