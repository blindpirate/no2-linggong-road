#include "dialoglabel.h"
#define ANIMATIONVALUE 20//动画载入的递增/递减像素值，越大动画进行的越快
#define ANIAMTIONTIMERINTERVAL 20

DialogLabel::DialogLabel(QWidget *parent,QString &param) :
    QWidget(parent),
    trianglePoints(new QPoint[3]),
    currentEndPos(0),
    triangleIsDown(true),
    triangleExist(false),
    animTimerID(startTimer(ANIAMTIONTIMERINTERVAL)),
    triTimerID(0),
    font(QFont(QString("微软雅黑"),18)),
    animationOn(true),
    dialogHeight(0),
    textIsRed(false),
    currentX(TEXTORIGINX),currentY(TEXTORIGINY)
{
    int roleID=getSubFromQString(param,1,'#').toInt();
    cutHeadFromQString(param,1,'#');
    text=param;
    beginPosVec.push_back(0);
    this->setGeometry((WINDOWWIDTH-DIALOGLABELW)/2,
                      WINDOWHEIGHT-DIALOGLABELH-10,DIALOGLABELW,DIALOGLABELH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    trianglePoints[0]=QPoint(DIALOGLABELW*18/20,DIALOGLABELH*9/10);
    trianglePoints[1]=QPoint(DIALOGLABELW*19/20,DIALOGLABELH*9/10);
    trianglePoints[2]=QPoint(DIALOGLABELW*18.5/20,DIALOGLABELH*9.5/10);
    roleName=getRoleNameFromRoleID(roleID);
    image=QPixmap(QCoreApplication::applicationDirPath() +QString("/data/image/head/%1.png").arg(roleID));
    this->show();
    this->setFocus();
}
DialogLabel::~DialogLabel()
{
    delete []trianglePoints;
}
void DialogLabel::quit()
{
    animationOn=true;
    if(animTimerID==0)animTimerID=startTimer(ANIAMTIONTIMERINTERVAL);
}
void DialogLabel::keyPressEvent(QKeyEvent *)
{
}
void DialogLabel::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()&&!animationOn)
    {
        if(event->key()==Qt::Key_Up||event->key()==Qt::Key_Left)
        {
            if(beginPosVec.size()==1)return;//如果是第一帧什么都不做,直接return
            beginPosVec.pop_back();
            triangleExist=true;
        }
        else //if(event->key()==Qt::Key_Down||event->key()==Qt::Key_Right)
        {
            if(currentEndPos==text.length()-1)
            {
                this->quit();
                return;
            }
            beginPosVec.push_back(++currentEndPos);
        }
        currentX=TEXTORIGINX;
        currentY=TEXTORIGINY;
        this->update();
    }
}
void DialogLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    font.setPointSize(18);
    painter.setFont(font);

    painter.setBrush(QColor(0,0,255,192));
    painter.setPen(QPen(QBrush(Qt::gray),3));

    painter.setRenderHints(QPainter::Antialiasing
                              | QPainter::HighQualityAntialiasing, true);//反锯齿
    QRect rectangle(5,
                     (DIALOGLABELH-dialogHeight)/2,
                     DIALOGLABELW-10,
                      dialogHeight);//left,top,width,height
    //QRect rectangle(5,75,455,75);//
    //qDebug()<<(DIALOGLABELH-dialogHeight)/2<<(DIALOGLABELH+dialogHeight)/2;
    painter.drawRoundedRect(rectangle,10,10);
    if(!animationOn)
    {
        //输出文字
        if(textIsRed)painter.setPen(QPen(QColor(Qt::red)));
        else painter.setPen(QPen(QColor(188,205,197)));
        int textLength=text.length();

        int charSize=QFontInfo(font).pixelSize();
        for(int i=beginPosVec.last(),currentX=TEXTORIGINX,currentY=TEXTORIGINY;i<textLength;++i)
        {
            if(i==textLength-1)
            {
                currentEndPos=i;
                triangleExist=false;
            }
            if(text[i]==QString("『")[0])
            {
                textIsRed=true;
                painter.setPen(QPen(QColor(Qt::red)));
            }


            if(text[i]=='#')
            {
                triangleExist=true;
                if(triTimerID==0)triTimerID=startTimer(100);
                currentEndPos=i;
                break;
            }
            else if(currentX+charSize>DIALOGLABELW-charSize)
            {
                if(currentY+charSize>DIALOGLABELH-charSize)//到达对话框右下角
                {
                    painter.drawText(currentX,currentY,text.mid(i,1));
                    triangleExist=true;

                    if(triTimerID==0)triTimerID=startTimer(100);
                    currentEndPos=i;
                    break;
                }
                else //否则换行
                {
                    painter.drawText(currentX,currentY,text.mid(i,1));
                    currentX=TEXTORIGINX;
                    currentY+=charSize;

                }
            }
            else
            {
                painter.drawText(currentX,currentY,text.mid(i,1));
                currentX+=charSize;

            }
            if(text[i]==QString("』")[0])
            {
                textIsRed=false;
                painter.setPen(QPen(QColor(188,205,197)));
            }
        }
        if(triangleExist)
        {
            painter.setBrush(QBrush(Qt::white));
            painter.drawPolygon(trianglePoints,3);
        }
        painter.drawPixmap(25,15,60,80,image);
        //painter.setPen(QPen(QColor(84,150,136)));
        painter.setPen(Qt::white);
        //QPainterPath path=
        if(roleName.length()>4)
            font.setPointSize(14);
        else
            font.setPointSize(18);
        painter.setFont(font);
        painter.drawText(5,105,100,30,Qt::AlignCenter,roleName);
    }
}
void DialogLabel::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==animTimerID)
    {
        //qDebug()<<currentEndPos<<" "<<text.length()<<" "<<(currentEndPos==text.length()-1);
        if(currentEndPos==text.length()-1)
        {
            if(dialogHeight-ANIMATIONVALUE<0)close();
            else dialogHeight-=ANIMATIONVALUE;
        }
        else
        {
            if(dialogHeight+ANIMATIONVALUE>DIALOGLABELH)
            {
                killTimer(animTimerID);
                animTimerID=0;
                dialogHeight=DIALOGLABELH-10;
                animationOn=false;
            }
            else dialogHeight+=ANIMATIONVALUE;
        }

    }
    else
    {
        if(triangleIsDown)
        {
            trianglePoints[0]-=QPoint(0,4);
            trianglePoints[1]-=QPoint(0,4);
            trianglePoints[2]-=QPoint(0,4);
            triangleIsDown=false;
        }
        else
        {
            triangleIsDown=true;
            trianglePoints[0]+=QPoint(0,4);
            trianglePoints[1]+=QPoint(0,4);
            trianglePoints[2]+=QPoint(0,4);
        }

    }
    this->update();
}
