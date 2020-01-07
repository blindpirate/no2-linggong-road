#include "roleinfowidget.h"

RoleInfoWidget::RoleInfoWidget(QWidget *parent, Role **roleInfo) :
    QWidget(parent),
    currentRoleID(1),
    roleInfo(roleInfo),
    font(QString("微软雅黑"),10),
    lvIcon(QApplication::applicationDirPath()+QString("/data/image/other/level.png")),
    hpIcon(QApplication::applicationDirPath()+QString("/data/image/other/hp.png")),
    expIcon(QApplication::applicationDirPath()+QString("/data/image/other/exp.png")),
    paIcon(QApplication::applicationDirPath()+QString("/data/image/other/physicalattack.png")),
    pdIcon(QApplication::applicationDirPath()+QString("/data/image/other/physicaldefence.png")),
    iqIcon(QApplication::applicationDirPath()+QString("/data/image/other/iq.png")),
    moveIcon(QApplication::applicationDirPath()+QString("/data/image/other/speed.png"))
{
    if(lvIcon.isNull()||
            hpIcon.isNull()||
            expIcon.isNull()||
            paIcon.isNull()||
            pdIcon.isNull()||
            iqIcon.isNull()||
            moveIcon.isNull())
        quitApp(ERRORIMAGEFILENOTFOUND);
    this->setGeometry(0,50,ROLEINFOWIDGETWIDTH,ROLEINFOWIDGETHEIGHT);
    this->show();
    //this->setFocus();
}

void RoleInfoWidget::paintEvent(QPaintEvent *)
{
    Role *currentRole=roleInfo[currentRoleID-1];
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing
                           | QPainter::HighQualityAntialiasing, true);
    painter.setPen(QPen(QBrush(QColor(161,175,201)),5));
    //painter.setBrush(QBrush(QColor(23,124,176,224)));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(5,5,200,360,5,5);

    QPixmap head=QPixmap(QApplication::applicationDirPath()+QString("/data/image/head/%1.png").arg(currentRoleID));
    if(head.isNull())quitApp(ERRORIMAGEFILENOTFOUND);
    //头像和名字,职业
    font.setPointSize(20);
    painter.setFont(font);
    painter.drawPixmap(QRect(10,10,60,80),head);
    painter.setPen(QPen(QColor(Qt::white)));
    painter.drawText(80,20,120,40,Qt::AlignLeft&Qt::AlignVCenter,getRoleNameFromRoleID(currentRole->ID));
    font.setPointSize(14);
    painter.setPen(Qt::red);
    painter.setFont(font);
    painter.drawText(80,60,120,40,Qt::AlignLeft&Qt::AlignVCenter,currentRole->getJob());

    //level,hp等信息
    painter.drawPixmap(QRect(10,100,24,24),lvIcon);
    painter.drawPixmap(QRect(10,134,24,24),hpIcon);
    painter.drawPixmap(QRect(10,168,24,24),expIcon);
    painter.drawPixmap(QRect(10,202,24,24),paIcon);
    painter.drawPixmap(QRect(10,236,24,24),pdIcon);
    painter.drawPixmap(QRect(10,270,24,24),iqIcon);
    painter.drawPixmap(QRect(10,304,24,24),moveIcon);

    font.setPointSize(10);
    painter.setFont(font);
    painter.setPen(QPen(QColor(233,255,85)));
    painter.drawText(39,105,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("等级        %1").arg(currentRole->level));
    painter.drawText(39,139,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("生命        %1/%2").arg(currentRole->HP).arg(currentRole->HPMax));
    painter.drawText(39,173,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("经验        %1/%2").arg(currentRole->exp).arg(currentRole->expMax));
    painter.drawText(39,207,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("攻击        %1").arg(currentRole->physicalAttack));
    painter.drawText(39,241,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("防御        %1").arg(currentRole->physicalDefence));
    painter.drawText(39,275,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("智力        %1").arg(currentRole->IQ));
    //painter.drawText(39,299,80,24,Qt::AlignLeft&Qt::AlignVCenter,QString("MgDef %1").arg(currentRole->magicDefence));
    painter.drawText(39,309,160,24,Qt::AlignLeft&Qt::AlignVCenter,QString("移动        %1").arg(currentRole->speed));
    //暂缺hp槽和exp槽
}

void RoleInfoWidget::changeToRoleOf(int id)
{
    assert(id<=ROLENUMBER);
    currentRoleID=id;
    this->update();
}
