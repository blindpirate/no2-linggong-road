#include "weaponwidget.h"

WeaponWidget::WeaponWidget(QWidget *parent,map<int,ArticleInfo> *pArticleIDToInfoMap) :
    QWidget(parent),
    pArticleIDToInfoMap(pArticleIDToInfoMap),
    type(ShowNoWeaponNorMedicine),
    x(0),
    y(0),
    role(NULL),
    currentIndex(0),
    weaponSum(0)
{
    this->hide();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
}
WeaponWidget::~WeaponWidget()
{
}

int WeaponWidget::indexGoDown()
{
    assert(this->isVisible());
    if(currentIndex==weaponSum-1)currentIndex=0;
    else currentIndex++;
    this->update();
    return currentIndex;
}
int WeaponWidget::indexGoUp()
{
    assert(this->isVisible());
    if(currentIndex==0)currentIndex=weaponSum-1;
    else currentIndex--;
    this->update();
    return currentIndex;
}

void WeaponWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing
                              | QPainter::HighQualityAntialiasing, true);
    painter.setPen(QPen(QBrush(QColor(Qt::blue)),2));
    painter.setBrush(QBrush(QColor(233,240,246,224)));
    if(type==ShowMedicine)
    {
        painter.drawRoundedRect(x,y,120,120,5,5);

        for(int i=0;i<4;++i)
        {
            if(!(role->medicine[i]))continue;
            painter.drawText(x,y+(i+1)*20,120,20,Qt::AlignCenter,(*pArticleIDToInfoMap)[role->medicine[i]].name);
        }
        if(weaponSum)
        {
            painter.setPen(QPen(QBrush(QColor(Qt::red)),2));
            painter.drawText(x,y+(currentIndex+1)*20,120,20,Qt::AlignCenter,(*pArticleIDToInfoMap)[role->medicine[currentIndex]].name);
        }

        painter.setPen(QColor(23,124,176));
        painter.drawText(x,y,120,20,Qt::AlignCenter,"药物");

    }
    else
    {
        painter.drawRoundedRect(x,y,120,120,5,5);

        for(int i=0;i<4;++i)
        {
            if(!(role->weapon[i]))continue;
            painter.drawText(x,y+(i+1)*20,120,20,Qt::AlignCenter,
                             QString("%1 %2 %3")
                             .arg((*pArticleIDToInfoMap)[role->weapon[i]].name)
                             .arg((*pArticleIDToInfoMap)[role->weapon[i]].attack)
                             .arg((*pArticleIDToInfoMap)[role->weapon[i]].accurcy)
                             );
        }
        if(weaponSum)
        {
            painter.setPen(QPen(QBrush(QColor(Qt::red)),2));
            painter.drawText(x,y+(currentIndex+1)*20,120,20,Qt::AlignCenter,
                             QString("%1 %2 %3")
                             .arg((*pArticleIDToInfoMap)[role->weapon[currentIndex]].name)
                             .arg((*pArticleIDToInfoMap)[role->weapon[currentIndex]].attack)
                             .arg((*pArticleIDToInfoMap)[role->weapon[currentIndex]].accurcy));
        }

        painter.setPen(QColor(23,124,176));
        painter.drawText(x,y,120,20,Qt::AlignCenter,"武器 攻击 命中");

    }
}

void WeaponWidget::show(int rolePointX,int rolePointY,Role *roleInfo,WeaponOrMedicine newType)
{
    assert(roleInfo);
    x=rolePointX;
    y=rolePointY+36;
    role=roleInfo;
    type=newType;
    currentIndex=0;
    weaponSum=0;

    if(type==ShowMedicine)
    {   
        for(int i=0;i<4;++i)
        {
            if(role->medicine[i]==0)
            {
                continue;
            }
            weaponSum++;
            if(pArticleIDToInfoMap->find(role->medicine[i])==pArticleIDToInfoMap->end())
            {
                DBIO dbio;
                if(!dbio.getArticleInfoFromLib(role->medicine[i],pArticleIDToInfoMap))
                    quitApp(ERRORGETARTICLEINFOFAIL);
            }
        }
    }
    else
    {
        for(int i=0;i<4;++i)
        {
            if(role->weapon[i]==0)
            {
                continue;
            }
            weaponSum++;
            if(pArticleIDToInfoMap->find(role->weapon[i])==pArticleIDToInfoMap->end())
            {
                DBIO dbio;
                if(!dbio.getArticleInfoFromLib(role->weapon[i],pArticleIDToInfoMap))
                    quitApp(ERRORGETARTICLEINFOFAIL);
            }
            //qDebug()<<"special"<<(*pArticleIDToInfoMap)[role->weapon[i]].special;
        }
    }
    this->setVisible(true);
    this->raise();
    this->update();
}
