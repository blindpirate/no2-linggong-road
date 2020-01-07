#include "tradewidget.h"

TradeWidget::TradeWidget(QWidget *parent,
                         KeyboardControl *key,
                         multiset<int>& carriedArticleSet,
                         QString const &soldArticleString,
                         Role**roleInfo,
                         int *pCarriedCash,
                         map<int, ArticleInfo> &articleIDToInfoMap):
    QWidget(parent),
    key(key),
    pCarriedArticleSet(&carriedArticleSet),
    roleInfo(roleInfo),
    tradeType(TradeBuy),
    focus(TradeFocusOnBuyAndSell),
    currentSelectedItemIndex(0),
    currentBuySellArticleID(0),
    currentBuySellCount(0),
    pCarriedCash(pCarriedCash),
    pCurrentDisplaySet(NULL),
    pArticleIDToInfoMap(&articleIDToInfoMap),
    rolePic(QApplication::applicationDirPath()+QString("/data/image/role/allrole_64_selected.png")),
    rolePicWB(QApplication::applicationDirPath()+QString("/data/image/role/allrole_64_unselected.png")),
    font(QFont("微软雅黑",12))
{
    this->show();
    this->setFocus();
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    this->setAttribute(Qt::WA_DeleteOnClose);

    upArrow[0]=QPoint(25,75);
    upArrow[1]=QPoint(20,85);
    upArrow[2]=QPoint(30,85);
    downArrow[0]=QPoint(25,165);
    downArrow[1]=QPoint(30,155);
    downArrow[2]=QPoint(20,155);

    int lastPos=-1;//记录上一个'#'的位置
    int currentArticleID=0;
    DBIO dbio;
    for(int i=0;i<soldArticleString.length();++i)
    {
        if(soldArticleString[i]=='#')
        {
            currentArticleID=soldArticleString.mid(lastPos+1,i-lastPos-1).toInt();
            if(pArticleIDToInfoMap->find(currentArticleID)==pArticleIDToInfoMap->end())
            {
                if(!dbio.getArticleInfoFromLib(currentArticleID,pArticleIDToInfoMap))
                    quitApp(ERRORGETARTICLEINFOFAIL);
            }
            soldArticleSet.insert(currentArticleID);
            lastPos=i;
        }
    }//从String中获得所有的物品id

    for(multiset<int>::iterator iter=pCarriedArticleSet->begin();iter!=pCarriedArticleSet->end();++iter)
    {
        currentArticleID=*iter;
        if(pArticleIDToInfoMap->find(currentArticleID)==pArticleIDToInfoMap->end())
        {
            if(!dbio.getArticleInfoFromLib(currentArticleID,pArticleIDToInfoMap))
                quitApp(ERRORGETARTICLEINFOFAIL);
        }
        carriedArticleUniqueSet.insert(currentArticleID);
    }
    //从携带的物品中筛选出不重复的物品集合
    pCurrentDisplaySet=&soldArticleSet;
    currentTopItemIter=pCurrentDisplaySet->begin();
    for(int i=0;i<ROLENUMBER;++i)
    {
        if(roleInfo[i])
            itemCanBeUsedVec.push_back(roleInfo[i]->ID);
    }
    //将当前拥有的角色id整理到一个vector里面
    this->update();
}
void TradeWidget::paintEvent(QPaintEvent *)
{

    if(!pCurrentDisplaySet)
        return;

    QPainter painter(this);

    painter.setFont(font);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing, true);//反锯齿
    painter.setBrush(QBrush(QColor(0,51,114,224)));
    painter.drawRect(this->rect());//先画个背景


    painter.setPen(QPen(Qt::white,1));
    painter.setFont(QFont("黑体",14));
    painter.drawText(240,40,240,50,
                     Qt::AlignVCenter|Qt::AlignVCenter,QString("该物品适用于"));

    //painter.setPen(Qt::NoPen);
    //画边框
    for(int i=0;i<ROLENUMBER;++i)
    {
        painter.setPen(QPen(Qt::white,1));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(224+i%SELECTROWNUMBER*(ROLEPICSIZE),
                         i/SELECTROWNUMBER*(ROLEPICSIZE)+100,
                         (ROLEPICSIZE),
                         (ROLEPICSIZE)
                         );
    }

    //画彩色或者黑白的人物
    for(int i=0;i<itemCanBeUsedVec.size();i++)
    {
        int rolePicX=(itemCanBeUsedVec[i]&0xff-1)%SELECTROWNUMBER*ROLEPICSIZE;
        int rolePicY=(itemCanBeUsedVec[i]&0xff-1)/SELECTROWNUMBER*ROLEPICSIZE;

        if(itemCanBeUsedVec[i]&0xff00)
            painter.drawPixmap(224+i%SELECTROWNUMBER*(ROLEPICSIZE+2),
                               i/SELECTROWNUMBER*(ROLEPICSIZE+2)+100,
                           rolePic,
                           rolePicX,
                           rolePicY,
                           ROLEPICSIZE-4,
                           ROLEPICSIZE-4
                           );
        else
            painter.drawPixmap(224+i%SELECTROWNUMBER*(ROLEPICSIZE+2),
                               i/SELECTROWNUMBER*(ROLEPICSIZE+2)+100,
                           rolePicWB,
                           rolePicX,
                           rolePicY,
                           ROLEPICSIZE-4,
                           ROLEPICSIZE-4
                           );
    }



    painter.setPen(QPen(QBrush(Qt::white),3));
    //painter.setBrush(QBrush(QColor(192,235,215)));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(40,40,180,400,5,5);//画物品栏
    painter.setPen(Qt::red);
    painter.drawText(50,40,100,40,Qt::AlignLeft|Qt::AlignVCenter,"物品");
    painter.drawText(130,40,80,40,Qt::AlignRight|Qt::AlignVCenter,"价格");

    if(tradeType==TradeSell)
        painter.drawText(100,40,60,40,Qt::AlignHCenter|Qt::AlignVCenter,"数量");

    painter.setPen(Qt::yellow);
    painter.drawText(40,440,100,40,Qt::AlignLeft|Qt::AlignVCenter,QString("金钱  %1").arg(*pCarriedCash));


    if(tradeType==TradeBuy)
    {
        painter.setPen(Qt::white);
        painter.drawText(10,90,30,30,Qt::AlignCenter,"买");
        painter.setPen(Qt::gray);
        painter.drawText(10,120,30,30,Qt::AlignCenter,"卖");
        painter.setPen(QPen(QBrush(Qt::red),3));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(10,90,30,30,4,4);
    }//画买卖标签
    else
    {
        painter.setPen(Qt::gray);
        painter.drawText(10,90,30,30,Qt::AlignCenter,"买");
        painter.setPen(Qt::white);
        painter.drawText(10,120,30,30,Qt::AlignCenter,"卖");
        painter.setPen(QPen(QBrush(Qt::red),3));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(10,120,30,30,4,4);
    }

    if(focus==TradeFocusOnBuyAndSell)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawPolygon(upArrow,3);
        painter.drawPolygon(downArrow,3);
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::gray);
        painter.drawPolygon(upArrow,3);
        painter.drawPolygon(downArrow,3);
    }
    if(pCurrentDisplaySet->empty())return;

    painter.setPen(Qt::white);
    int currentPaintArticleID=0;
    set<int>::iterator iter=currentTopItemIter;

    for(int i=0;i<ARTICLECOUNT;++i,iter++)
    {
        if(pCurrentDisplaySet->size()<ARTICLECOUNT)
        {
            if(i>=pCurrentDisplaySet->size())break;
        }
        else
        {
            if(iter==pCurrentDisplaySet->end())
                iter=pCurrentDisplaySet->begin();
        }

        currentPaintArticleID=*(iter);
        //qDebug()<<currentPaintArticleID<<idToNameMap[currentPaintArticleID]<<" "<<idToPriceMap[currentPaintArticleID];
        painter.drawText(50,40*(i+2),100,40,Qt::AlignLeft|Qt::AlignVCenter,(*pArticleIDToInfoMap)[currentPaintArticleID].name);

        if(tradeType==TradeBuy)
            painter.drawText(130,40*(i+2),80,40,Qt::AlignVCenter|Qt::AlignRight,QString("%1").arg((*pArticleIDToInfoMap)[currentPaintArticleID].price));
        else
        {
            painter.drawText(130,40*(i+2),60,40,Qt::AlignHCenter|Qt::AlignVCenter,QString("%1").arg(pCarriedArticleSet->count(currentPaintArticleID)));
            painter.drawText(130,40*(i+2),80,40,Qt::AlignVCenter|Qt::AlignRight,QString("%1").arg((*pArticleIDToInfoMap)[currentPaintArticleID].price*SOLDRATE));

        }

    }
    //画各个条目

    if(focus==TradeFocusOnArticle
            ||focus==TradeFocusOnItemSelected)
    {
        painter.setPen(QPen(QBrush(Qt::red),3));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawRoundedRect(40,40*(currentSelectedItemIndex+2),180,40,4,4);
        QPoint rightArrow[3];
        rightArrow[0]=QPoint(210,40*(currentSelectedItemIndex+1)+55);
        rightArrow[1]=QPoint(210,40*(currentSelectedItemIndex+1)+65);
        rightArrow[2]=QPoint(215,40*(currentSelectedItemIndex+1)+60);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawPolygon(rightArrow,3);

    }


    if(focus==TradeFocusOnItemSelected)
    {
        painter.setPen(QPen(QBrush(Qt::white),2));
        //painter.setBrush(QColor(192,235,215));
        //painter.setBrush(Qt::NoBrush);
        painter.setBrush(QBrush(QColor(0,51,114,224)));
        painter.drawRoundedRect(230,40*(currentSelectedItemIndex+2),60,40,5,5);
        painter.setPen(QPen(Qt::white));
        painter.drawText(230,40*(currentSelectedItemIndex+2),60,40,Qt::AlignCenter,QString("%1").arg(currentBuySellCount));
    }//画数量框



}
void TradeWidget::keyPressEvent(QKeyEvent *e)
{
}
void TradeWidget::keyReleaseEvent(QKeyEvent *e)
{
    assert(pCurrentDisplaySet);
    if(e->isAutoRepeat())return;
    if(e->key()==key->actionKey)
    {
        switch(focus)
        {
        case TradeFocusOnBuyAndSell:
            if(pCurrentDisplaySet->empty())
                break;
            focus=TradeFocusOnArticle;
            break;
        case TradeFocusOnArticle:
            focus=TradeFocusOnItemSelected;
            currentBuySellCount=1;
            break;
        case TradeFocusOnItemSelected:
            //已经选好了数量,按确定就进行买入或者卖出

            if(tradeType==TradeBuy)
            {
                if((*pCarriedCash)-currentBuySellCount*((*pArticleIDToInfoMap)[currentBuySellArticleID].price)<0)
                {
                    QMessageBox::warning(this,"凌工路2号","金钱不足！",QMessageBox::Ok);
                    break;//当前金钱不足以买下这些物品
                }

                (*pCarriedCash)-=currentBuySellCount*((*pArticleIDToInfoMap)[currentBuySellArticleID].price);
                //付钱
                while(currentBuySellCount--)
                {
                    pCarriedArticleSet->insert(currentBuySellArticleID);
                    qDebug()<<"current count is:"<<(pCarriedArticleSet->count(currentBuySellArticleID));
                }
                carriedArticleUniqueSet.insert(currentBuySellArticleID);
                currentBuySellCount=1;
                focus=TradeFocusOnArticle;
            }
            else
            {
                int originalCount=pCarriedArticleSet->count(currentBuySellArticleID);
                assert(originalCount!=0&&originalCount>=currentBuySellCount);
                (*pCarriedCash)+=currentBuySellCount*((*pArticleIDToInfoMap)[currentBuySellArticleID].price)*SOLDRATE;
                //付钱
                pCarriedArticleSet->erase(currentBuySellArticleID);
                //全删掉
                for(int i=0;i<originalCount-currentBuySellCount;++i)
                {
                    pCarriedArticleSet->insert(currentBuySellArticleID);
                }
                //先全删掉,然后插入原有数量-卖出数量个物品
                if(originalCount==currentBuySellCount)
                {//全卖光了,迭代器可能失效
                    pCurrentDisplaySet->erase(currentBuySellArticleID);
                    currentTopItemIter=pCurrentDisplaySet->begin();
                    currentSelectedItemIndex=0;
                }
                if(pCurrentDisplaySet->empty())
                    focus=TradeFocusOnBuyAndSell;
                else focus=TradeFocusOnArticle;
            }
            break;
        default:
            break;
        }
    }
    else if(e->key()==key->cancelKey)
    {
        switch(focus)
        {
        case TradeFocusOnBuyAndSell:
            this->close();
            break;
        case TradeFocusOnArticle:
            focus=TradeFocusOnBuyAndSell;
            break;
        case TradeFocusOnItemSelected:
            focus=TradeFocusOnBuyAndSell;
            break;
        default:
            break;
        }
    }
    else if(e->key()==key->menuKey)
    {
        this->close();
    }
    else if(e->key()==key->upKey)
    {
        switch(focus)
        {
        case TradeFocusOnBuyAndSell:
            if(tradeType==TradeSell)
            {
                tradeType=TradeBuy;
                pCurrentDisplaySet=&soldArticleSet;//购买模式,显示对方出售的物品
            }
            else
            {
                tradeType=TradeSell;
                pCurrentDisplaySet=&carriedArticleUniqueSet;
            }
            currentTopItemIter=pCurrentDisplaySet->begin();
            currentSelectedItemIndex=0;
            break;
        case TradeFocusOnArticle:
            if(pCurrentDisplaySet->size()>ARTICLECOUNT)
            {//若实际的物品比能显示的物品多,
                if(currentSelectedItemIndex!=0)
                    currentSelectedItemIndex--;
                else
                {
                    if(currentTopItemIter==pCurrentDisplaySet->begin())
                    {
                        currentTopItemIter=pCurrentDisplaySet->end();
                    }
                    currentTopItemIter--;
                }
            }
            else
            {
                if(currentSelectedItemIndex!=0)
                    currentSelectedItemIndex--;
                else
                    currentSelectedItemIndex=pCurrentDisplaySet->size()-1;
            }            
            break;
        case TradeFocusOnItemSelected:

            if(!(tradeType==TradeSell&&currentBuySellCount==pCarriedArticleSet->count(currentBuySellArticleID)))
                currentBuySellCount++;
            break;
        default:
            break;
        }
    }
    else if(e->key()==key->downKey)
    {
        switch(focus)
        {
        case TradeFocusOnBuyAndSell:
            if(tradeType==TradeSell)
            {
                tradeType=TradeBuy;
                pCurrentDisplaySet=&soldArticleSet;//购买模式,显示对方出售的物品
            }
            else
            {
                tradeType=TradeSell;
                pCurrentDisplaySet=&carriedArticleUniqueSet;
            }
            currentTopItemIter=pCurrentDisplaySet->begin();
            currentSelectedItemIndex=0;
            break;
        case TradeFocusOnArticle:
            if(pCurrentDisplaySet->size()>ARTICLECOUNT)
            {//若实际的物品比能显示的物品多,
                if(currentSelectedItemIndex!=ARTICLECOUNT-1)
                    currentSelectedItemIndex++;
                else
                {
                    currentTopItemIter++;
                    if(currentTopItemIter==pCurrentDisplaySet->end())
                    {
                        currentTopItemIter=pCurrentDisplaySet->begin();
                    }
                }
            }
            else
            {
                if(currentSelectedItemIndex!=pCurrentDisplaySet->size()-1)
                    currentSelectedItemIndex++;
                else
                    currentSelectedItemIndex=0;
            }
            break;
        case TradeFocusOnItemSelected:
            if(currentBuySellCount>0)
            {
                currentBuySellCount--;
            }
            break;
        default:
            break;
        }
    }
    else if(e->key()==key->leftKey)
    {
        switch(focus)
        {
        case TradeFocusOnBuyAndSell:
            break;
        case TradeFocusOnArticle:
            focus=TradeFocusOnBuyAndSell;
            break;
        case TradeFocusOnItemSelected:
            focus=TradeFocusOnArticle;
            currentBuySellCount=1;
            break;
        default:
            break;
        }
    }
    else if(e->key()==key->rightKey)
    {
        //qDebug()<<*currentTopItemIter;
        switch(focus)
        {
        case TradeFocusOnBuyAndSell:
            if(!pCurrentDisplaySet->empty())
                focus=TradeFocusOnArticle;
            break;
        case TradeFocusOnArticle:
            focus=TradeFocusOnItemSelected;
            currentBuySellCount=1;
            break;
        case TradeFocusOnItemSelected:
            break;
        default:
            break;
        }
    }

    if(focus==TradeFocusOnArticle||focus==TradeFocusOnItemSelected)
    {
        int selectedIndex=currentSelectedItemIndex;
        set<int>::iterator iter=currentTopItemIter;
        while(selectedIndex--)
        {
            iter++;
        }
        currentBuySellArticleID=*iter;
    }
    else
        currentBuySellArticleID=0;
    //若当前焦点在物品上,获取物品的id

    int jobCode=-1;//职业代码
    int id=0;
    for(int i=0;i<itemCanBeUsedVec.size();++i)
    {
        id=itemCanBeUsedVec[i]&0xff;

        jobCode=roleInfo[id-1]->job/3;
        if(currentBuySellArticleID>0&&currentBuySellArticleID<WEAPONUPBOUND)
        {//武器
            if(currentBuySellArticleID/100==jobCode||
                    currentBuySellArticleID/100==jobToWeaponID[jobCode])
            {
                itemCanBeUsedVec[i]=(id|0x100);
                //可以用,那么低8位是id,次8位是1
            }
            else
                itemCanBeUsedVec[i]=(id);
        }
        else if(currentBuySellArticleID==0)
        {
            itemCanBeUsedVec[i]=(id);
        }
        else
        {
            //其他像防具什么的就无所谓了,大家都能用
            itemCanBeUsedVec[i]=(id|0x100);
        }
    }
    this->update();
}

void TradeWidget::close()
{
    this->parentWidget()->setFocus();
    QWidget::close();
}
