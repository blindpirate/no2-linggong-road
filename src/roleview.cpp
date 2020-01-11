#include "roleview.h"
#include "roleanimation.h"

//int RoleView::currentViewPointX=-0x0fffffff;
//int RoleView::currentViewPointY=-0x0fffffff;//这个值不能太小,否则当地图尺寸小于视角尺寸时可能为负数
//QPixmap * RoleView::sweatImage=NULL;//QPixmap();//(QPixmap(QCoreApplication::applicationDirPath() +QString("/data/image/other/sweat.png")));
 //静态成员变量必须通过赋值方式初始化
//1 mapHero
//2~99 英雄NPC
//101~499 普通NPC
//500~1000 反派NPC,他们的id+1500是图像文件名
RoleView::RoleView(QWidget *parent, int mapX, int mapY,int roleID):
    QWidget(parent),
    currentViewPointX(0),currentViewPointY(0),
    type(0),
    mapArray(NULL),
    pPathVec(NULL),
    currentIndexInPathVec(0),
    pointMapX(mapX*BASEWIDTH),pointMapY(mapY*BASEHEIGHT),
    mapX(mapX),mapY(mapY),
    ID(roleID),
    rotateCount(0),
    //animationType(RoleNoAnimation),
    activeImage(NULL),
    inactiveImage(NULL),
    animationTimerID(0),rotateTimerID(0),
    moveTimerID(0),
    flashTimerID(0),
    isActive(true),
    reactOnViewMove(true),
    beingContacted(false),
    dir(NOTMOVING),
    currentPose(STAYFRONT),
    role(NULL),
    pScriptVec(NULL)
{
    if(roleID==1)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(roleID));
        reactOnViewMove=false;
        type=static_cast<int>(StoryMapHero);
    }
    else if(roleID<500)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(roleID));
        type=static_cast<int>(StoryMapNPC);
    }
    else if(roleID<1000)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(roleID+1500));
        type=static_cast<int>(StoryMapNPC);
    }
    else if(roleID<2000)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(ID-1000));
        inactiveImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/inactive_%1.png").arg(ID-1000));
        type=static_cast<int>(BattleHero);
        if(inactiveImage->isNull())
        {
            qDebug()<<roleID;
            quitApp(ERRORGETROLEIMAGEFAIL);
        }
    }
    else if(roleID<3000)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(roleID));
        inactiveImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/inactive_%1.png").arg(roleID));
        type=static_cast<int>(BattleEnemy);
        if(inactiveImage->isNull())
        {
            qDebug()<<roleID;
            quitApp(ERRORGETROLEIMAGEFAIL);
        }
    }
    else
    {
        assert(roleID<4000);
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(roleID-3000));
        inactiveImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/inactive_%1.png").arg(roleID-3000));
        type=static_cast<int>(BattleFriend);
        if(inactiveImage->isNull())
        {
            qDebug()<<roleID;
            quitApp(ERRORGETROLEIMAGEFAIL);
        }
    }

    if(activeImage->isNull())
    {
        qDebug()<<roleID;
        quitApp(ERRORGETROLEIMAGEFAIL);
    }
    this->show();
    this->setAttribute(Qt::WA_DeleteOnClose);
    //this->setGeometry(blockX*BASEWIDTH,blockY*BASEHEIGHT,BASEWIDTH,BASEHEIGHT);
}
RoleView::RoleView(QWidget *parent, uchar *data):
    QWidget(parent),
    currentViewPointX(0),currentViewPointY(0),
    mapArray(NULL),
    pPathVec(NULL),
    currentIndexInPathVec(0),
    pointMapX(0),pointMapY(0),
    mapX(0),mapY(0),
    ID(0),
    rotateCount(0),
    activeImage(NULL),
    inactiveImage(NULL),
    animationTimerID(0),rotateTimerID(0),
    moveTimerID(0),
    flashTimerID(0),
    isActive(true),
    reactOnViewMove(true),
    beingContacted(false),
    dir(NOTMOVING),
    currentPose(STAYFRONT),
    role(NULL),
    pScriptVec(NULL)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();

    RoleViewData *pData=reinterpret_cast<RoleViewData *>(data);
    currentIndexInPathVec=pData->currentIndexInPathVec;
    pointMapX=pData->pointX;
    pointMapY=pData->pointY;
    mapX=pointMapX/BASEWIDTH;
    mapY=pointMapY/BASEHEIGHT;
    ID=pData->ID;
    type=pData->type;
    if(ID<1000&&ID>500)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(ID+1500));
    }
    else if(ID<2000&&ID>1000)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(ID-1000));
    }
    else if(ID>3000)
    {
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(ID-3000));
    }
    else
        activeImage=new QPixmap(QApplication::applicationDirPath() +QString("/data/image/role/%1.png").arg(ID));
    //inactiveImage=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/role/inactive_%1.png").arg(ID));
    if(activeImage->isNull())quitApp(ERRORGETROLEIMAGEFAIL);
    switch(type)
    {
    case static_cast<int>(BattleHero):
        inactiveImage=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/role/inactive_%1.png").arg(ID-1000));
        break;
    case static_cast<int>(BattleEnemy):
        inactiveImage=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/role/inactive_%1.png").arg(ID));
        break;
    case static_cast<int>(BattleFriend):
        inactiveImage=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/role/inactive_%1.png").arg(ID-3000));
        break;
    default:
        break;
    }
    if(inactiveImage&&inactiveImage->isNull())quitApp(ERRORGETROLEIMAGEFAIL);

    currentPose=pData->currentPose;
    dir=pData->dir;
    flashTimerID=0;
    isActive=pData->isActive;
    reactOnViewMove=pData->reactOnViewMove;
    role=NULL;
    contactDirectly=pData->contactDirectly;



    uchar *currentPointer=data+sizeof(RoleViewData);
    //第二段内存的开头4个字节标记了本段pathVec的size
    int pathVecSize=*(reinterpret_cast<int *>(currentPointer));
    assert(pathVecSize>=0);
    currentPointer+=sizeof(int);
    if(pathVecSize)
    {
        pPathVec=new vector<QPoint>;
        while(pathVecSize--)
        {
            pPathVec->push_back(QPoint(*(reinterpret_cast<int *>(currentPointer)),
                                       *(reinterpret_cast<int *>(currentPointer+sizeof(int)))));
            currentPointer+=2*sizeof(int);
        }
    }
    //从存档中读出pathVect
    int scriptVecSize=*(reinterpret_cast<int *>(currentPointer));
    currentPointer+=sizeof(int);

    if(type==static_cast<int>(BattleEnemy)||
            type==static_cast<int>(BattleFriend))
    {
        role=new Role(*(reinterpret_cast<Role *>(currentPointer)));
        currentPointer+=sizeof(Role);
    }
    //第三段开头的4个字节标记本段scriptVec的size
    if(scriptVecSize)
    {
        pScriptVec=new vector<Script>;
        while(scriptVecSize--)
        {
            pScriptVec->push_back(Script(*(reinterpret_cast<AnimationOperation *>(currentPointer)),
                                         QString::fromUtf8(reinterpret_cast<char *>(currentPointer+sizeof(AnimationOperation)))));
            currentPointer+=(sizeof(AnimationOperation)+strlen(reinterpret_cast<char *>(currentPointer+sizeof(AnimationOperation)))+1);//4个字节+字符串长度+结束的NULL字符
        }
    }
    if(pData->moveTimerOn)
    {
        moveTimerID=startTimer(ROLEMOVEINTERVAL);
    }
    if(pData->animationTimerOn)
    {
        animationTimerID=startTimer(ANIMATIONINTERVAL);
    }

}
//void RoleView::hide()
//{
//    this->setVisible(false);
//    hidden=true;
//}
void RoleView::show()
{
    //qDebug()<<"show";
    this->setVisible(true);
    if(moveTimerID==0&&pPathVec)
    {
        moveTimerID=startTimer(ROLEMOVEINTERVAL);
        animationTimerID=startTimer(ANIMATIONINTERVAL);
    }
}
void RoleView::saveToData(uchar **data,int &length)
{
//    qDebug()<<pointMapX
//              <<pointMapY
//                <<ID
//                  <<currentIndexInPathVec
//                    <<type
//                      <<currentPose
//                        <<dir
//                          <<isActive
//                            <<reactOnViewMove
//                              <<contactDirectly
//                                <<animationTimerID;
//    pointMapX;
//    pointMapY;
//    ID;
//    currentIndexInPathVec;
//    type;
//    currentPose;
//    isActive;
//    reactOnViewMove;
//    contactDirectly;
//    animationTimerID;
//    moveTimerID;

    RoleViewData selfData(pointMapX,
                          pointMapY,
                          ID,
                          currentIndexInPathVec,
                          type,
                          currentPose,
                          dir,
                          isActive,
                          reactOnViewMove,
                          contactDirectly,
                          animationTimerID==0?(false):(true),
                          moveTimerID==0?(false):(true));
    int scriptVecDataSize=0;//script数据的字节长度
    if(pScriptVec)
    {
       for(int i=0;i<(pScriptVec->size());++i)
       {
           scriptVecDataSize+=(sizeof(AnimationOperation)+(*pScriptVec)[i].param.toUtf8().size()+1);
       }
    }
    //计算script数据区的长度,不算开头的size的4个字节

    int pathVecDataSize=((pPathVec)?(pPathVec->size()*2*sizeof(int)):(0));
    //计算path数据区的长度,每个QPoint包含2*sizeof(int)个字节,不算开头的size的4个字节


    if(type==static_cast<int>(BattleEnemy)||
            type==static_cast<int>(BattleFriend))//若为这两种不需要存储path和Script,需要额外存储Role数据
    {
        length=sizeof(RoleViewData)+sizeof(int)+sizeof(int)+sizeof(Role);
    }
    else
    {
        length=sizeof(RoleViewData)+sizeof(int)+pathVecDataSize+sizeof(int)+scriptVecDataSize;
    }
    *data=new uchar[length];//由调用方负责释放
    if(!(*data))quitApp(ERRORMEMORYALLOCFAIL);

    uchar *currentPointer=*data;

    memcpy(currentPointer,&selfData,sizeof(RoleViewData));
    currentPointer+=sizeof(RoleViewData);
    //复制RoleViewData数据

    if(type==static_cast<int>(BattleEnemy)||
            type==static_cast<int>(BattleFriend))
    {
        *(reinterpret_cast<int *>(currentPointer))=0;
    }
    else
        *(reinterpret_cast<int *>(currentPointer))=(pPathVec)?(pPathVec->size()):(0);

    currentPointer+=sizeof(int);
    //pathVec的size,不是字节长度,这个size乘以8才是字节长度

    if(type!=static_cast<int>(BattleEnemy)&&
            type!=static_cast<int>(BattleFriend)&&
            pPathVec)
    {
        for(int i=0;i<(pPathVec->size());++i)
        {
            *(reinterpret_cast<int *>(currentPointer))=(*pPathVec)[i].x();
            *(reinterpret_cast<int *>(currentPointer+sizeof(int)))=(*pPathVec)[i].y();
            currentPointer+=2*sizeof(int);
        }
    }
    //pathVec数据归档,battleenemy和battlefriend不需要这一步


    if(type==static_cast<int>(BattleEnemy)||
            type==static_cast<int>(BattleFriend))
    {
        *(reinterpret_cast<int *>(currentPointer))=0;
    }
    else
        *(reinterpret_cast<int *>(currentPointer))=(pScriptVec)?(pScriptVec->size()):(0);
    currentPointer+=sizeof(int);
    //同样,这也是size而不是字节长度

    if(type!=static_cast<int>(BattleEnemy)&&
            type!=static_cast<int>(BattleFriend)&&
            pScriptVec)
    {
        for(int i=0;i<(pScriptVec->size());++i)
        {
            QByteArray scriptParamData=(*pScriptVec)[i].param.toUtf8();
            *(reinterpret_cast<AnimationOperation *>(currentPointer))=(*pScriptVec)[i].op;
            memcpy(currentPointer+sizeof(AnimationOperation),
                   scriptParamData.data(),
                   scriptParamData.size()+1);
            currentPointer+=(sizeof(AnimationOperation)+scriptParamData.size()+1);
        }
    }
    //scriptVec数据归档

    if(type==static_cast<int>(BattleEnemy)||
            type==static_cast<int>(BattleFriend))
    {
        memcpy(currentPointer,role,sizeof(Role));
        currentPointer+=sizeof(Role);
    }
    assert((*data)+length==currentPointer);
}
RoleView::~RoleView()
{
    delete activeImage;
    delete inactiveImage;
    delete pPathVec;
    delete pScriptVec;
    if(type==static_cast<int>(BattleEnemy)||
            type==static_cast<int>(BattleFriend))
    {
        delete role;
    }
}
//public:
void RoleView::startMoving(bool fastMoving)
{
//    for(int i=0;i<pPathVec->size();++i)
//    {
//        qDebug()<<(*pPathVec)[i].x()<<(*pPathVec)[i].y();
//    }
    //this->beingContacted=false;
    if(pPathVec==NULL||pPathVec->empty())return;//如果只有一个点的话移动不?
    assert(moveTimerID==0);
    if(fastMoving)
        moveTimerID=startTimer(ROLEFASTMOVEINTERVAL);
    else moveTimerID=startTimer(ROLEMOVEINTERVAL);
    if(animationTimerID==0)animationTimerID=startTimer(ANIMATIONINTERVAL);
    currentIndexInPathVec=0;
    this->update();
}

void RoleView::rewalk()
{
    this->beingContacted=false;
    if(pPathVec==NULL||pPathVec->empty())return;
    assert(moveTimerID==0);
    moveTimerID=startTimer(ROLEMOVEINTERVAL);
    if(animationTimerID==0)animationTimerID=startTimer(ANIMATIONINTERVAL);
    this->update();
}
void RoleView::moveToPointOfMap(int x,int y)
{
    //qDebug()<<currentViewPointX<<" "<<currentViewPointY;
    this->pointMapX=x;
    this->pointMapY=y;
    //if(x%BASEWIDTH==0&&y%BASEHEIGHT==0)
    this->mapX=x/BASEWIDTH;
    this->mapY=y/BASEHEIGHT;

    //保持mapX和mapY为即将进入的格子
    if(dir==DOWN&&y%BASEHEIGHT!=0)
    {
        this->mapY+=1;
    }
    else if(dir==RIGHT&&x%BASEWIDTH!=0)
    {
        this->mapX+=1;
    }


    //if(x>=currentViewPointX&&x-currentViewPointX<WINDOWWIDTH&&y>=currentViewPointY&&y-currentViewPointY<WINDOWHEIGHT)
    {//在可视区域内
        //qDebug()<<x<<" "<<y<<" "<<currentViewPointX<< " "<<currentViewPointY;
        this->move(x-currentViewPointX,y-currentViewPointY);
        this->update();
    }
}
void RoleView::startAnimation(DIRECTION value)
{
    dir=value;
    if(animationTimerID==0)
        animationTimerID=startTimer(ANIMATIONINTERVAL);
    changePose();
}
void RoleView::stopAnimation()
{
    if(animationTimerID)killTimer(animationTimerID);
    animationTimerID=0;
}
void RoleView::changeDir(DIRECTION newDir)
{
    dir=newDir;
}
/*
void RoleView::sweat(bool endWithSignal)
{
    //emit roleAnimationEnd();
    RoleSweat *widget=new RoleSweat(this);
    widget->show();
    if(endWithSignal)
        connect(widget->animation,SIGNAL(finished()),this,SLOT(onAnimationEnd()));
}
void RoleView::attack()
{
//    AnimationWidget *widget=new AnimationWidget(this,pointMapX-currentViewPointX,pointMapX,QString("attackanimation.png"));
//    widget->start();
//    connect(widget,SIGNAL(destroyed()),this,SLOT(onAnimationEnd()));
}
void RoleView::recover(int originalHP,int recoverHP,int fullHP,bool endWithSignal)
{
}
void RoleView::hurt(int originalHP,int hurtHP,int fullHP,bool endWithSignal)
{//hpslot从一开始就显示,但是它要到numberwidget开始动画之后才开始动画
//    HPSlotWidget *hp=new HPSlotWidget(this->parentWidget(),mapX,mapY,originalHP,hurtHP,fullHP);

//    AnimationWidget *widget=new AnimationWidget(this,QString("hurt32.png"));
//    widget->start();
//    this->flash();
//    NumberWidget *numberWidget=new NumberWidget(this,hurtHP);
//    connect(widget,SIGNAL(destroyed()),numberWidget,SLOT(start()));
//    connect(widget,SIGNAL(destroyed()),hp,SLOT(start()));
//    connect(numberWidget,SIGNAL(destroyed()),this,SLOT(onAnimationEnd()));//这里假设numberWidget存在的时间长一点
}
void RoleView::die()
{
//    this->close();
//    AnimationWidget *widget=new AnimationWidget(this,QString("die.png"));

}
*/
void RoleView::flash()
{
    flashTimerID=startTimer(FLASHINTERVAL);
    this->update();
}
void RoleView::dodge(bool endWithSignal)
{
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry",this);
    animation->setDuration(400);
    animation->setStartValue(this->geometry());
    if(mapX==0)//在最左边则向右闪避,否则全部向左闪避
    {
        animation->setKeyValueAt(0.5,QRect(mapX*BASEWIDTH-currentViewPointX+BASEWIDTH,//向右闪避1个格子
                                           mapY*BASEHEIGHT-currentViewPointY,
                                           BASEWIDTH,
                                           BASEHEIGHT));
    }
    else
    {

        animation->setKeyValueAt(0.5,QRect(mapX*BASEWIDTH-currentViewPointX-BASEWIDTH,//向左闪避1个格子
                                           mapY*BASEHEIGHT-currentViewPointY,
                                           BASEWIDTH,
                                           BASEHEIGHT));
    }
    animation->setKeyValueAt(0.9,this->geometry());
    animation->setEndValue(this->geometry());
    animation->start();
    if(endWithSignal)
        connect(animation,SIGNAL(finished()),this->parent(),SLOT(startAnimation()));
    connect(animation,SIGNAL(finished()),animation,SLOT(deleteLater()));
}
void RoleView::rotate()
{
    assert(animationTimerID);
    killTimer(animationTimerID);
    animationTimerID=0;
    assert(moveTimerID==0);
    assert(rotateTimerID==0);
    rotateTimerID=startTimer(ROTATEINTERVAL);
    this->rotateCount=8;
    this->currentPose=STAYFRONT;
    this->update();
}
void RoleView::setPathVec(QString path)
{
    if(path.length()==0)return;

    if(pPathVec==NULL)
    {
        pPathVec=new vector<QPoint>();
    }
    else
        pPathVec->clear();
        //pPathVec->swap(vector<QPoint>());

    getPathFromQString(*pPathVec,path);
}
//public slot:
void RoleView::onViewMovedTo(int x, int y)
{
    currentViewPointX=x;
    currentViewPointY=y;
    if(!reactOnViewMove)return;

    //if(pointMapX>=x&&pointMapX-x<WINDOWWIDTH&&pointMapY>=y&&pointMapY-y<WINDOWHEIGHT)//在可视区域内
    {
        //if(ID==1)qDebug()<<pointMapX<<" "<<currentViewPointX<<" "<<pointMapY<<" "<<currentViewPointY;
        this->move(pointMapX-currentViewPointX,pointMapY-currentViewPointY);
        this->update();
    }
}
void RoleView::onAnimationEnd()
{
    emit roleAnimationEnd();
    //qDebug()<<"sweat end";
}
//private:
void RoleView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(!isActive)
    {
        assert(moveTimerID==0);
        this->stopAnimation();
        painter.drawPixmap(0,0,BASEWIDTH,BASEHEIGHT,*inactiveImage);
        return;
    }
    //if(flashTimerID==0)
        painter.drawPixmap(QPoint(0,0),*activeImage,getRectFromPose(currentPose));
    //else
    {

    }
}
void RoleView::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==animationTimerID)
        changePose();
    else if(event->timerId()==moveTimerID)
    {//若是角色的移动事件
        if(pointMapX%BASEWIDTH==0&&pointMapY%BASEWIDTH==0)
        {
            if(beingContacted||!this->isVisible())
            {
                if(moveTimerID)killTimer(moveTimerID);
                moveTimerID=0;
                if(animationTimerID)killTimer(animationTimerID);
                animationTimerID=0;
                return;
            }

            mapX=pointMapX/BASEWIDTH;
            mapY=pointMapY/BASEHEIGHT;
            int currentPathX=(*pPathVec)[currentIndexInPathVec].x();
            int currentPathY=(*pPathVec)[currentIndexInPathVec].y();//currentxy指已经经过的路径点

            //qDebug()<<currentPathX<<" "<<currentPathY;
            //assert(mapX==nextX||mapY==nextY);
            if(mapX==currentPathX&&mapY==currentPathY)
            { //只有在拐点处才能改变方向
                if(currentIndexInPathVec==pPathVec->size()-1)currentIndexInPathVec=0;
                else ++currentIndexInPathVec;
                int nextPathX=(*pPathVec)[currentIndexInPathVec].x();
                int nextPathY=(*pPathVec)[currentIndexInPathVec].y();
                assert((mapX==nextPathX&&mapY!=nextPathY)
                       ||(mapY==nextPathY&&mapX!=nextPathX)
                       ||(nextPathX==-1&&nextPathY==-1)
                       ||(nextPathX==-2&&nextPathY==-2));//pathVec中相邻的两个点必然位于同一条直线上且不相同
                if(nextPathX==-1)
                {
                    vector<QPoint>().swap(*pPathVec);
                    pPathVec=NULL;
                    //pPathVec->swap(vector<QPoint>());
                    this->stopAnimation();
                    emit roleAnimationEnd();
                    killTimer(moveTimerID);
                    moveTimerID=0;
                    this->currentPose=STAYFRONT;
                    //emit roleAnimationEnd();
                    //isMoving=false;
                    return;
                }
                else if(nextPathX==-2)//-2意味着它应该消失了,通知scene销毁自己以及清理roleVec
                {
                    this->stopAnimation();
                    killTimer(moveTimerID);
                    moveTimerID=0;
                    emit roleDisappear(mapX,mapY);
                    return;
                }
                else if(mapX>nextPathX)
                {//下一个x比当前x靠左
                    dir=LEFT;
                }
                else if(mapX<nextPathX)
                {
                    dir=RIGHT;
                }
                else if(mapY>nextPathY)
                {//下一个y比当前y靠上
                    dir=UP;
                }
                else if(mapY<nextPathY)
                {
                    dir=DOWN;
                }
            }

            if(!processNextArrayElement(mapArray,mapX,mapY,dir))return;
        }
        //不论是否到达一个block内,都按照原定方向移动

        switch(dir)
        {
            case UP:moveToPointOfMap(pointMapX,pointMapY-ROLEMOVEPIXEL);break;
            case DOWN:moveToPointOfMap(pointMapX,pointMapY+ROLEMOVEPIXEL);break;
            case LEFT:moveToPointOfMap(pointMapX-ROLEMOVEPIXEL,pointMapY);break;
            case RIGHT:moveToPointOfMap(pointMapX+ROLEMOVEPIXEL,pointMapY);break;
            default:break;
        }

        emit AIMoved(pointMapX,pointMapY);

    }
    else if(event->timerId()==flashTimerID)
    {
        killTimer(flashTimerID);
        emit roleAnimationEnd();
        flashTimerID=0;
    }
    else if(event->timerId()==rotateTimerID)
    {
        switch(currentPose)
        {
        case STAYFRONT:
            currentPose=STAYLEFT;
            break;
        case STAYLEFT:
            currentPose=STAYBACK;
            break;
        case STAYBACK:
            currentPose=STAYRIGHT;
            break;
        case STAYRIGHT:
            currentPose=STAYFRONT;
            break;
        default:
            qDebug()<<currentPose;
            assert(0);
        }
        if((--rotateCount)==0)
        {
            killTimer(rotateTimerID);
            rotateTimerID=0;
            emit roleAnimationEnd();
        }
    }
    this->update();

}
void RoleView::changePose()
{
    assert(dir);//计时器启动的时候，dir必然已经被赋值四个方向中的一个
    switch(dir)
    {
        case UP:
            if(currentPose==MOVINGBACK_1)currentPose=MOVINGBACK_2;
            else if(currentPose==MOVINGBACK_2||currentPose==STAYBACK)currentPose=MOVINGBACK_1;
            else currentPose=STAYBACK;
            break;
        case DOWN:
            if(currentPose==MOVINGFRONT_1)currentPose=MOVINGFRONT_2;
            else if(currentPose==MOVINGFRONT_2||currentPose==STAYFRONT)currentPose=MOVINGFRONT_1;
            else currentPose=STAYFRONT;
            break;
        case LEFT:
            if(currentPose==MOVINGLEFT_1)currentPose=MOVINGLEFT_2;
            else if(currentPose==MOVINGLEFT_2||currentPose==STAYLEFT)currentPose=MOVINGLEFT_1;
            else currentPose=STAYLEFT;
            break;
        case RIGHT:
            if(currentPose==MOVINGRIGHT_1)currentPose=MOVINGRIGHT_2;
            else if(currentPose==MOVINGRIGHT_2||currentPose==STAYRIGHT)currentPose=MOVINGRIGHT_1;
            else currentPose=STAYRIGHT;
            break;
        default:break;
    }
    this->update();
}
bool RoleView::processNextArrayElement(int **mapArray,int x,int y,DIRECTION dir)
{//这个函数由pathvec确定的路径调用,需要保证pathvec中的点不越界
    switch(dir)
    {
        case UP:
        if(mapArray[y-1][x]==0)
        {
            swap(mapArray[y][x],mapArray[y-1][x]);
            mapY-=1;
            return true;
        }
        return false;
        case DOWN:
        if(mapArray[y+1][x]==0)
        {
            swap(mapArray[y][x],mapArray[y+1][x]);
            mapY+=1;
            return true;
        }
        return false;
        case LEFT:
        if(mapArray[y][x-1]==0)
        {
            swap(mapArray[y][x],mapArray[y][x-1]);
            mapX-=1;
            return true;
        }
        return false;
        case RIGHT:
        if(mapArray[y][x+1]==0)
        {
            swap(mapArray[y][x],mapArray[y][x+1]);
            mapX+=1;
            return true;
        }
        return false;
    default:
        assert(0);
    }
}
void RoleView::setScriptVec(QString param)
{
    if(param.length()==0||param[0]=='@')return;
    if(pScriptVec==NULL)
    {
        pScriptVec=new vector<Script>();
    }
    else
    {
        vector<Script>().swap(*pScriptVec);
        //pScriptVec->swap(vector<Script>());
    }

    getScriptFromQString(*pScriptVec,param);
}
void RoleView::onContact(vector<Script> &vec, DIRECTION dir)
{//如果当前正在移动则停止
    if(ID>300&&ID<500)
        return;//id在300和500之间的不响应任何操作
    switch(dir)
    {
    case UP:
        this->currentPose=STAYFRONT;
        break;
    case DOWN:
        this->currentPose=STAYBACK;
        break;
    case LEFT:
        this->currentPose=STAYRIGHT;
        break;
    case RIGHT:
        this->currentPose=STAYLEFT;
        break;
    default:
        assert(0);
    }
    beingContacted=true;
//    for(int i=0;i<pScriptVec->size();++i)
//    {
//        qDebug()<<(*pScriptVec)[i].op<<" "<<(*pScriptVec)[i].param;
//    }
    if(pScriptVec)
    {
        if((pScriptVec->end()-1)->op==AniMultiTalkFlag)
        {//如果末尾是这个标志的话
            vector<Script>::iterator iter=pScriptVec->end();
            while((--iter)->op!=AniStart)
            {
            }
            vec=vector<Script>(iter,pScriptVec->end()-1);//从上一个AniStart(包括之)到rbegin()之前
            for(int i=0;i<vec.size();++i)
            {
                qDebug()<<vec[i].op<<vec[i].param;
            }
            pScriptVec->erase(iter,pScriptVec->end());
//            while(n--)
//            {
//                pScriptVec->pop_back();
//            }
        }
        else
            vec=*pScriptVec;
    }

}
/*
void RoleView::constructRolePointer(int level)
{
    assert(ID>100);
    assert(!role);
    ROLEJOB;
    int HP,HPMax,physicalAttack,physicalDefence,magicAttack,magicDefence,accurcy,
            speed,hedge,range;
    switch(ID)
    {
    case 2001:
        if(level<20)pro=JIROUNAN;
        else if(level<40)pro=BAOLINAN;
        else pro=QIGONGSHI;
        break;
    case 2002:
        if(level<20)pro=DAMA;
        else if(level<40)pro=CAIDAOMA;
        else pro=ZHANDOUMA;
        break;
    case 2003:
        if(level<20)pro=DASHU;
        else if(level<40)pro=SHUSHU;
        else pro=GUAISHUSHU;
        break;
    case 2004:
        if(level<20)NVHANZI;
        else if(level<40)pro=JIROUNV;
        else pro=BAWANGHUA;
        break;
    case 2005:
        if(level<20)pro=TUHAO;
        else if(level<40)pro=FUERDAI;
        else pro=GAOFUSHUAI;
        break;
    case 2006:
        if(level<20)pro=DIAOSI;
        else if(level<40)pro=DIAOBAO;
        else pro=NIXI;
        break;
    case 2007:
        if(level<20)pro=ZHAINAN;
        else if(level<40)pro=JISHUZHAI;
        else pro=JISHUSHEN;
        break;
    case 2008:
        if(level<20)pro=XUEZHA;
        else if(level<40)pro=ZIXINAN;
        else pro=XUEBA;
        break;
    case 2009:
        if(level<20)pro=QIANGSHOU;
        else if(level<40)pro=JUJISHOU;
        else pro=SHENSHESHOU;
        break;
    case 2010:
        if(level<20)pro=MEIZHI;
        else if(level<40)pro=MENGMEIZHI;
        else pro=NVSHEN;
        break;
    default:
        assert(0);
        break;
    }
    switch(pro)
    {
    case JIROUNAN:
        HP=70+level*4;
        HPMax=HP;
        physicalAttack=70+level*3;
        physicalDefence=40+level;
        magicAttack=53;
        magicDefence=2;
        accurcy=95;
        hedge=1;
        speed=8;
        range=2;

//                ,HPMax,physicalAttack,physicalDefence,magicAttack,magicDefence,accurcy,
//                    speed,hedge,range;
        break;
    case BAOLINAN:
        physicalAttack=130+(level-20)*3;
        physicalDefence=60+level-20;
        HP=160+(level-20)*4;
        HPMax=HP;
        magicAttack=55;
        magicDefence=5;
        accurcy=95;
        hedge=2;
        speed=8;
        range=2;
        break;
    case QIGONGSHI:
        physicalAttack=190+(level-40)*3;
        physicalDefence=80+level-40;
        HP=250+(level-40)*4;
        HPMax=HP;
        magicAttack=57;
        magicDefence=8;
        accurcy=95;
        hedge=2;
        speed=8;
        range=2;
        break;

    case DAMA:
        physicalAttack=65+level*3;
        physicalDefence=35+level;
        HP=65+level*4;
        HPMax=HP;
        magicAttack=55;
        magicDefence=5;
        accurcy=95;
        hedge=10;
        speed=8;
        range=2;
        break;
    case CAIDAOMA:
        physicalAttack=125+(level-20)*3;
        physicalDefence=55+level-20;
        HP=150+(level-20)*4;
        HPMax=HP;
        magicAttack=56;
        magicDefence=10;
        accurcy=97;
        hedge=10;
        speed=8;
        range=2;
        break;
    case ZHANDOUMA:
        physicalAttack=185+(level-40)*3;
        physicalDefence=75+level-40;
        HP=230+(level-40)*4;
        HPMax=HP;
        magicAttack=60;
        magicDefence=12;
        accurcy=99;
        hedge=10;
        speed=8;
        range=2;
        break;

    case DASHU:
        physicalAttack=63+level*3;
        physicalDefence=32+level;
        HP=60+level*4;
        HPMax=HP;
        magicAttack=85;
        magicDefence=35;
        accurcy=90;
        hedge=2;
        speed=8;
        range=2;
        break;
    case SHUSHU:
        physicalAttack=123+(level-20)*3;
        physicalDefence=52+level-20;
        HP=150+(level-20)*4;
        HPMax=HP;
        magicAttack=87;
        magicDefence=37;
        accurcy=93;
        hedge=2;
        speed=8;
        range=2;
        break;
    case GUAISHUSHU:
        physicalAttack=183+(level-40)*3;
        physicalDefence=69+level-40;
        HP=210+(level-40)*4;
        HPMax=HP;
        magicAttack=89;
        magicDefence=39;
        accurcy=95;
        hedge=2;
        speed=8;
        range=2;
        break;

    case NVHANZI:
        physicalAttack=63+level*3;
        physicalDefence=29+level;
        HP=65+level*4;
        HPMax=HP;
        magicAttack=60;
        magicDefence=37;
        accurcy=90;
        hedge=0;
        speed=8;
        range=2;
        break;
    case JIROUNV:
        physicalAttack=123+(level-20)*3;
        physicalDefence=49+level-20;
        HP=130+(level-20)*4;
        HPMax=HP;
        magicAttack=62;
        magicDefence=42;
        accurcy=95;
        hedge=0;
        speed=8;
        range=2;
        break;
    case BAWANGHUA:
        physicalAttack=183+(level-40)*3;
        physicalDefence=69+level-40;
        HP=210+(level-40)*4;
        HPMax=HP;
        magicAttack=60;
        magicDefence=45;
        accurcy=95;
        hedge=0;
        speed=8;
        range=2;
        break;

    case TUHAO:
        physicalAttack=60+level*3;
        physicalDefence=32+level;
        HP=80+level*4;
        HPMax=HP;
        magicAttack=80;
        magicDefence=40;
        accurcy=95;
        hedge=10;
        speed=8;
        range=2;
        break;
    case FUERDAI:
        physicalAttack=120+(level-20)*3;
        physicalDefence=59+level-20;
        HP=150+(level-20)*4;
        HPMax=HP;
        magicAttack=80;
        magicDefence=42;
        accurcy=95;
        hedge=10;
        speed=8;
        range=2;
        break;
    case GAOFUSHUAI:
        physicalAttack=160+(level-40)*3;
        physicalDefence=79+level-40;
        HP=200+(level-40)*4;
        HPMax=HP;
        magicAttack=82;
        magicDefence=45;
        accurcy=95;
        hedge=10;
        speed=8;
        range=2;
        break;

    case DIAOSI:
        physicalAttack=50+level*3;
        physicalDefence=34+level;
        HP=60+level*4;
        HPMax=HP;
        magicAttack=65;
        magicDefence=20;
        accurcy=95;
        hedge=3;
        speed=8;
        range=2;
        break;
    case DIAOBAO:
        physicalAttack=110+(level-20)*5;
        physicalDefence=54+2*(level-20);
        HP=110+(level-20)*8;
        HPMax=HP;
        magicAttack=70;
        magicDefence=26;
        accurcy=95;
        hedge=7;
        speed=8;
        range=2;
        break;
    case NIXI:
        physicalAttack=220+(level-40)*3;
        physicalDefence=90+level-40;
        HP=280+(level-40)*4;
        HPMax=HP;
        magicAttack=75;
        magicDefence=30;
        accurcy=99;
        hedge=20;
        speed=8;
        range=2;
        break;

    case ZHAINAN:
        physicalAttack=55+level*3;
        physicalDefence=35+level;
        HP=55+level*4;
        HPMax=HP;
        magicAttack=90;
        magicDefence=45;
        accurcy=96;
        hedge=2;
        speed=8;
        range=2;
        break;
    case JISHUZHAI:
        physicalAttack=115+(level-20)*3;
        physicalDefence=55+level-20;
        HP=120+(level-20)*4;
        HPMax=HP;
        magicAttack=95;
        magicDefence=48;
        accurcy=99;
        hedge=6;
        speed=8;
        range=2;
        break;
    case JISHUSHEN:
        physicalAttack=175+(level-40)*3;
        physicalDefence=75+level-40;
        HP=190+(level-40)*4;
        HPMax=HP;
        magicAttack=99;
        magicDefence=49;
        accurcy=99;
        hedge=8;
        speed=8;
        range=2;
        break;

    case XUEZHA:
        physicalAttack=55+level*3;
        physicalDefence=30+level;
        HP=65+level*4;
        HPMax=HP;
        magicAttack=85;
        magicDefence=15;
        accurcy=95;
        hedge=0;
        speed=8;
        range=2;
        break;
    case ZIXINAN:
        physicalAttack=115+(level-20)*3;
        physicalDefence=50+level-20;
        HP=125+(level-20)*4;
        HPMax=HP;
        magicAttack=90;
        magicDefence=20;
        accurcy=95;
        hedge=2;
        speed=8;
        range=2;
        break;
    case XUEBA:
        physicalAttack=175+(level-40)*3;
        physicalDefence=70+level-40;
        HP=200+(level-40)*4;
        HPMax=HP;
        magicAttack=95;
        magicDefence=25;
        accurcy=95;
        hedge=5;
        speed=8;
        range=2;
        break;

    case QIANGSHOU:
        physicalAttack=50+level*3;
        physicalDefence=20+level;
        HP=60+level*4;
        HPMax=HP;
        magicAttack=70;
        magicDefence=10;
        accurcy=110;
        hedge=0;
        speed=8;
        range=2;
        break;
    case JUJISHOU:
        physicalAttack=110+(level-20)*3;
        physicalDefence=40+level-20;
        HP=120+(level-20)*4;
        HPMax=HP;
        magicAttack=76;
        magicDefence=16;
        accurcy=120;
        hedge=6;
        speed=8;
        range=2;
        break;
    case SHENSHESHOU:
        physicalAttack=170+(level-40)*3;
        physicalDefence=60+level-40;
        HP=185+(level-40)*4;
        HPMax=HP;
        magicAttack=82;
        magicDefence=20;
        accurcy=130;
        hedge=10;
        speed=8;
        range=2;
        break;

    case MEIZHI:
        physicalAttack=50+level*3;
        physicalDefence=20+level;
        HP=55+level*4;
        HPMax=HP;
        magicAttack=75;
        magicDefence=35;
        accurcy=95;
        hedge=5;
        speed=8;
        range=2;
        break;
    case MENGMEIZHI:
        physicalAttack=110+(level-20)*3;
        physicalDefence=40+level-20;
        HP=110+(level-20)*4;
        HPMax=HP;
        magicAttack=80;
        magicDefence=40;
        accurcy=99;
        hedge=10;
        speed=8;
        range=2;
        break;
    case NVSHEN:
        physicalAttack=170+(level-40)*3;
        physicalDefence=60+level-40;
        HP=180+(level-40)*4;
        HPMax=HP;
        magicAttack=85;
        magicDefence=45;
        accurcy=99;
        hedge=15;
        speed=8;
        range=2;
        break;
    default:assert(0);
    }
    role=new Role(ID,pro,level,HP,HPMax,0,0,physicalAttack,physicalDefence,magicAttack,magicDefence,accurcy,speed,hedge,range,
                  0,0,0,0,
                  0,0,0,0,
                  0,0);

}
*/


//void RoleView::loadInactiveImage()
//{
//    assert(inactiveImage==NULL);
//    inactiveImage=new QPixmap(QApplication::applicationDirPath()+QString("/data/image/role/inactive_%1.png").arg(ID));
//    if(activeImage->isNull())quitApp(ERRORGETROLEIMAGEFAIL);
//}

/*
void RoleView::sweat()
{
    RoleSweat *sweat=new RoleSweat(this);
    sweat->show();
    connect(sweat,SIGNAL(destroyed()),this,SLOT(onSweatDestroyed()));
}
void RoleView::onSweatDestroyed()
{
    emit hasStoppedSweat();
}
*/
/*
Hero::Hero(QWidget*parent,int mapX,int mapY,QString&param1,QString &param2):
    RoleView(parent,mapX,mapY,getSubFromQString(param1,1,'$').toInt(),param2)
{
}
void Hero::onViewMovedTo(int x, int y)
{
    if(!reactOnViewMove)return;
    RoleView::onViewMovedTo(x,y);
}
void Hero::onContact(){}
void Hero::dodge()
{
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry",this);
    animation->setDuration(500);
    animation->setStartValue(this->rect());
    if(pointMapX==0)//在最左边则向右闪避,否则全部向左闪避
    {
        animation->setKeyValueAt(0.5,QRect(pointMapX*BASEWIDTH-currentViewPointX+BASEWIDTH/2,//向右闪避半个格子
                                           pointMapY*BASEHEIGHT-currentViewPointY,
                                           BASEWIDTH,
                                           BASEHEIGHT));
    }
    else
    {
        animation->setKeyValueAt(0.5,QRect(pointMapX*BASEWIDTH-currentViewPointX-BASEWIDTH/2,//向左闪避半个格子
                                           pointMapY*BASEHEIGHT-currentViewPointY,
                                           BASEWIDTH,
                                           BASEHEIGHT));
    }
    animation->setEndValue(this->rect());
    animation->start();
    connect(animation,SIGNAL(finished()),this,SLOT(onAnimationEnd()));
    connect(animation,SIGNAL(finished()),animation,SLOT(deleteLater()));
}
void Hero::onAnimationEnd()
{

    switch(operation)
    {
    case AniRoleSweat:
        emit hasStoppedSweat();
        break;
    case AniRoleAttack:
        emit hasStoppedAttack();
        break;
    case AniRoleRecover:
        emit hasStoppedRecover();
        break;
    case AniRoleHurt:
        emit hasStoppedHurt();
        break;
    case AniRoleDie:
        emit hasStoppedDie();
        break;
    case AniRoleDodge:
        emit hasStoppeDodge();
        case Ani
    }
}
/8*
void Hero::attack()
{
    RoleAttack *attack=new RoleAttack(this);
    attack->show();
    connect(attack,SIGNAL(destroyed),this,SLOT(onAttackDestroyed()));
}
void Hero::onAttackDestroyed()
{
    emit hasStoppedAttack();
}
void Hero::recovery()
{
    RoleRecovery *recovery=new RoleRecovery(this);
    recovery->show();
    connect(recovery,SIGNAL(destroyed),this,SLOT(onRecoveryDestroyed()));
}
void Hero::onRecoveryDestroyed()
{
    emit hasStoppedRecover();
}
void Hero::hurt()
{
    RoleHurt *hurt=new RoleHurt(this);
    hurt->show();
    connect(hurt,SIGNAL(destroyed()),this,SLOT(onHurtDestroyed()));
}
void Hero::onHurtDestroyed()
{
    emit hasStoppedHurt();
}
void Hero::die()
{
    RoleDie *die=new RoleDie(this);
    die->show();
    connect(die,SIGNAL(destroyed()),this,SLOT(onDieDestroyed()));
}
void Hero::onDieDestroyed()
{
    this->destroy();
}
*8/
//&******************NPC类的实现
NPC::NPC(QWidget *parent, int mapX, int mapY,QString&param1,QString&param2):
    RoleView(parent,mapX,mapY,static_cast<int>(MapNPC),getSubFromQString(param1,1,'$').toInt(),param2)
{
    cutHeadFromQString(param1,1,'$');
    //qDebug()<<mapX<<" "<<mapY;
}

void NPC::onContact()
{
    void startAnimation(vector<Script> &);
}

*/
