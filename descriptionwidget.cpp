#include "descriptionwidget.h"

DescriptionWidget::DescriptionWidget(QWidget *parent) :
    QWidget(parent),
    index(0),
    x(0),
    y(0),
    role(NULL),
    font("微软雅黑",10),
    image(NULL),
    delayTimerID(0)
{
    this->hide();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    connect(this,SIGNAL(animationEnd()),this->parent(),SLOT(startAnimation()));
}
void DescriptionWidget::show(int rolePointX, int rolePointY, int index)
{
    this->setVisible(true);
    this->raise();
    type=ShowOperationDescription;
    x=rolePointX;
    y=rolePointY+36;
    this->index=index;
    this->update();
}
void DescriptionWidget::show(int rolePointX, int rolePointY, Role *role)
{
    this->setVisible(true);
    this->raise();
    type=ShowRoleInfo;
    x=rolePointX-24;
    y=rolePointY-48;
    this->role=role;
    this->update();
}
void DescriptionWidget::show(int rolePointX, int rolePointY, AnimationOperation animationTye)
{
    this->setVisible(true);
    this->raise();
    if(image)
        delete image;
    delayTimerID=startTimer(DESCRIPTIONDELAY);
    switch(animationTye)
    {
    case AniRolePoison:
        type=ShowRoleInPoison;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/kuloutou.png"));
        break;
    case AniRoleDisable:
        type=ShowRoleDisabled;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/zzz.png"));
        break;
    case AniRoleAttackDown:
        type=ShowRoleAttackDown;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/attackdown.png"));
        break;
    case AniRoleAccurcyDown:
        type=ShowRoleAccurcyDown;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/accurcydown.png"));
        break;
    case AniRoleCritDown:
        type=ShowRoleCritDown;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/critdown.png"));
        break;
    case AniRoleDefenceDown:
        type=ShowRoleDefenceDown;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/defencedown.png"));
        break;
    case AniRoleDodgeDown:
        type=ShowRoleDodgeDown;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/dodgedown.png"));
        break;
    case AniRoleSpeedDown:
        type=ShowRoleSpeedDown;
        image=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/speeddown.png"));

        break;
    default:
        qDebug()<<type;
        assert(0);
    }
    x=rolePointX+2;
    y=rolePointY-30;
    this->update();
}
void DescriptionWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing
                              | QPainter::HighQualityAntialiasing, true);
    QPoint t[3];
    switch(type)
    {
    case ShowOperationDescription:
        painter.setPen(QPen(QBrush(Qt::black),1));
        painter.setBrush(QBrush(Qt::white));
        painter.drawRoundedRect(x-1+(index)*24,y-15,30,15,1,1);
        switch(index)
        {
        case 0:
            painter.drawText(x-1+(index)*24,y-15,30,15,Qt::AlignCenter,QString("攻击"));
            break;
        case 1:
            painter.drawText(x-1+(index)*24,y-15,30,15,Qt::AlignCenter,QString("移动"));
            break;
        case 2:
            painter.drawText(x-1+(index)*24,y-15,30,15,Qt::AlignCenter,QString("待命"));
            break;
        case 3:
            painter.drawText(x-1+(index)*24,y-15,30,15,Qt::AlignCenter,QString("物品"));
            break;
        default:
            qDebug()<<index;
            assert(0);
        }
        break;
    case ShowRoleInfo:
        font.setPointSize(10);
        painter.setFont(font);
        painter.setBrush(QColor(255,255,255,224));
        painter.setPen(QPen(QBrush(Qt::black),1));
        painter.drawRoundedRect(x+2,y+2,80,40,2,2);
        painter.drawLine(x+40,y+48,x+32,y+42);
        painter.drawLine(x+40,y+48,x+48,y+42);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255,255,255,224));

        t[0]=QPoint(x+40,y+48);
        t[1]=QPoint(x+30,y+40);
        t[2]=QPoint(x+50,y+40);
        painter.drawPolygon(t,3);
        painter.setPen(Qt::black);
        painter.setFont(font);
        painter.drawText(x+4,y+4,50,16,Qt::AlignVCenter|Qt::AlignLeft,getRoleNameFromRoleID(role->ID));
        font.setPointSize(8);
        painter.setFont(font);
        painter.drawText(x+60,y+4,30,16,Qt::AlignVCenter|Qt::AlignLeft,QString("Lv.%1").arg(role->level));

        painter.drawText(x+4,y+20,30,20,Qt::AlignVCenter|Qt::AlignLeft,role->getJob());

        font.setPointSize(6);
        painter.setFont(font);
        painter.drawText(x+32,y+28,45,10,Qt::AlignCenter,QString("%1/%2").arg(role->HP).arg(role->HPMax));
        painter.setPen(QPen(QBrush(Qt::gray),1));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(x+36,y+24,40,3);
        painter.setPen(Qt::NoPen);
        if((role->HP)/(role->HPMax)>0.8)
        {
            painter.setBrush(Qt::green);
        }
        else if((role->HP)/(role->HPMax)>0.3)
        {
            painter.setBrush(Qt::yellow);
        }
        else
            painter.setBrush(Qt::red);
        painter.drawRect(x+36,y+24,40*(role->HP)/(role->HPMax),3);
        break;
    case ShowRoleDisabled:
    case ShowRoleInPoison:
    case ShowRoleAttackDown:
    case ShowRoleAccurcyDown:
    case ShowRoleSpeedDown:
    case ShowRoleDodgeDown:
    case ShowRoleCritDown:
    case ShowRoleDefenceDown:

        painter.setBrush(QColor(255,255,255,224));
        painter.setPen(QPen(QBrush(Qt::black),1));
        painter.drawRoundedRect(x+1,y+1,26,26,2,2);
        painter.drawLine(x+14,y+30,x+12,y+27);
        painter.drawLine(x+14,y+30,x+16,y+27);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255,255,255,224));
        t[0]=QPoint(x+14,y+30);
        t[1]=QPoint(x+11,y+26);
        t[2]=QPoint(x+17,y+26);
        painter.drawPolygon(t,3);
        painter.drawPixmap(x+2,y+2,*image);
        break;
    default:
        qDebug()<<type;
        assert(0);
    }
}

void DescriptionWidget::timerEvent(QTimerEvent *)
{
    this->hide();
    killTimer(delayTimerID);
    emit animationEnd();
}
