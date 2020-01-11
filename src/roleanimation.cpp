#include "roleanimation.h"
/******RoleAnimation******/
RoleAnimation::RoleAnimation(QWidget *parent, int rolePointX, int rolePointY, AnimationOperation op):
    QWidget(parent),
    type(op),
    x(rolePointX),y(rolePointY),
    frameSize(0),
    animationTimerID(0),
    currentFrameNum(0),
    frameNum(0),
    image(QPixmap())
{
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
    switch(op)
    {
    case AniRoleSweat:
        image.load(QApplication::applicationDirPath()+"/data/image/other/sweatanimation.png");
        break;
    case AniRoleAttack:
        image.load(QApplication::applicationDirPath()+"/data/image/other/attackanimation.png");
        break;
    case AniRoleDie:
        image.load(QApplication::applicationDirPath()+"/data/image/other/dieanimation.png");
        break;
    case AniRoleLevelUp:
        image.load(QApplication::applicationDirPath()+"/data/image/other/levelupanimation.png");
        break;
    case AniRoleChangeJob:
        image.load(QApplication::applicationDirPath()+"/data/image/other/changejobanimation.png");
        break;
    case AniRoleAttention:
        image.load(QApplication::applicationDirPath()+"/data/image/other/attentionanimation.png");
        break;
    case AniRoleRecover:
        image.load(QApplication::applicationDirPath()+"/data/image/other/recoveranimation.png");
        break;
    case AniRoleHurt:
        image.load(QApplication::applicationDirPath()+"/data/image/other/hurtanimation.png");
        break;
    case AniRoleCure:
        image.load(QApplication::applicationDirPath()+"/data/image/other/cureanimation.png");
        break;
    case AniRoleUseMagic:
        image.load(QApplication::applicationDirPath()+"/data/image/other/magicanimation.png");
        break;
    default:
        qDebug()<<op;
        assert(0);
    }
}


void RoleAnimation::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(x,y,image,currentFrameNum*frameSize,0,frameSize,frameSize);
}
void RoleAnimation::timerEvent(QTimerEvent *)
{
    if(currentFrameNum==frameNum-1)
    {
        killTimer(animationTimerID);
        animationTimerID=0;
        emit animationEnd();
        this->close();
    }
    else
    {
        currentFrameNum++;
        this->update();
    }
}

/*****************RoleSweat**********************/
/*
RoleSweat::RoleSweat(QWidget *parent) :
    QWidget(parent),
    animation(NULL),
    sweatImage(QPixmap(QApplication::applicationDirPath() +QString("/data/image/other/sweat.png")))
{
    if(sweatImage.isNull())quitApp(ERRORGETROLEANIMATIONIMAGEFAIL);
    this->setGeometry(0,0,32,32);
    this->setAttribute(Qt::WA_DeleteOnClose);
    animation=new QPropertyAnimation(this,"geometry",this);
    animation->setStartValue(QRect(18,0,10,9));
    animation->setEndValue(QRect(18,0,10,25));
    animation->setKeyValueAt(0.7,QRect(18,0,10,25));
    animation->setDuration(800);
    animation->start();
    connect(animation,SIGNAL(finished()),this,SLOT(close()));
}

void RoleSweat::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(),sweatImage);
}
*/
/*****************RoleAttack**********************/
/*
RoleAttack::RoleAttack(QWidget *parent):
    QWidget(parent),
    attackImage1(new QPixmap()),
    attackImage2(new QPixmap())
{
    this->setGeometry(0,0,BASEWIDTH,BASEHEIGHT);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QPropertyAnimation *animation=new QPropertyAnimation(this,"attackImage");
    animation->setStartValue(*attackImage1);
    animation->setEndValue(*attackImage2);
    animation->setDuration(500);
    connect(animation,SIGNAL(finished()),this,SLOT(close()));
}
void RoleAttack::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(),attackImage);
}
*/
/*****************RoleHurt**********************/
/*****************RoleRecover**********************/
/*********************AnimationWidget*****************************/
/*
AnimationWidget::AnimationWidget(QWidget *parent,int roleViewPointX,int roleViewPointY,const QString &fileName):
    QWidget(parent),
    image(new QPixmap(QApplication::applicationDirPath()+QString("/data/image/other/")+fileName)),
    frameNum(0),
    currentFrameNum(0),
    animationTimerID(0)
{
    if(image->isNull())quitApp(ERRORGETROLEANIMATIONIMAGEFAIL);
    animationSize=image->height();
    x=roleViewPointX-((animationSize-BASEWIDTH)/2);
    y=roleViewPointY-((animationSize-BASEHEIGHT)/2);
    //qDebug()<<x<<" "<<y;
    //这个xy是动画显示的左上角点,因为父窗口是View,所以需要根据角色坐标调整动画显示的左上角点,默认将角色窗口包含在里面
    //默认动画都是正方形显示
    frameNum=image->width()/image->height();
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
}
AnimationWidget::~AnimationWidget()
{
    delete image;
}
void AnimationWidget::start()
{
    if(animationTimerID==0)animationTimerID=this->startTimer(90);
}
void AnimationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(x,y,*image,currentFrameNum*animationSize,0,animationSize,animationSize);
}
void AnimationWidget::timerEvent(QTimerEvent *)
{
    if(currentFrameNum==frameNum-1)
    {
        killTimer(animationTimerID);
        animationTimerID=0;
        this->close();
    }
    else
    {
        currentFrameNum++;
        this->update();
    }
}
*/
/****************NumberWidget*******************/
/*
NumberWidget::NumberWidget(QWidget *parent,int number,int roleViewPointX,int roleViewPointY) :
    QWidget(parent),
    numberString(QString("%1").arg(number)),
    font(QFont(QString("黑体"),12,QFont::Bold))
{
    font.setLetterSpacing(QFont::AbsoluteSpacing,-3);
    this->setGeometry(roleViewPointX,roleViewPointY,32,32);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
}

void NumberWidget::start()
{
    this->show();
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry",this);
    animation->setDuration(1000);
    animation->setStartValue(QRect(0,0,32,32));
    animation->setKeyValueAt(0.2,QRect(0,-16,32,32));
    animation->setKeyValueAt(0.4,QRect(0,0,32,32));
    animation->setKeyValueAt(0.5,QRect(0,16,32,32));
    animation->setKeyValueAt(0.7,QRect(0,3,32,32));
    animation->setKeyValueAt(0.8,QRect(0,8,32,32));
    animation->setKeyValueAt(0.9,QRect(0,16,32,32));
    animation->setKeyValueAt(0.95,QRect(0,12,32,32));
    animation->setEndValue(QRect(0,12,32,32));
    animation->start();
    connect(animation,SIGNAL(finished()),animation,SLOT(deleteLater()));
    connect(animation,SIGNAL(finished()),this,SLOT(close()));
}

void NumberWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::white)));
    painter.setFont(font);
    painter.drawText(this->rect().x(),this->rect().y(),this->rect().width(),this->rect().height(),
                     Qt::AlignCenter,numberString);
}
*/
