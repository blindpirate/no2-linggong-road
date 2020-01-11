#include "battleroleselectscene.h"

BattleRoleSelectScene::BattleRoleSelectScene(QWidget *parent,KeyboardControl *key,Role **roleInfo,int battleID):
    QWidget(parent),
    rolePic(QApplication::applicationDirPath()+QString("/data/image/role/allrole_64_selected.png")),
    rolePicWB(QApplication::applicationDirPath()+QString("/data/image/role/allrole_64_unselected.png")),
    cursor(QApplication::applicationDirPath()+QString("/data/image/other/cursor_64.png")),
    roleInfoWidget(this,roleInfo),
    roleInfo(roleInfo),
    currentIndex(0),
    //maxNum(maxNum),
    key(key),
    //battleName("综合楼之战"),
    font(QFont("黑体",20)),
    currentSelectedNum(1)//默认第一个被选中,因此初值为1
{
    BattleInfo info=getBattleInfoFromBattleID(battleID);
    maxNum=info.maxNum;
    battleName=info.name;
    this->setGeometry(0,0,WINDOWWIDTH,WINDOWHEIGHT);
    if(rolePic.isNull()||rolePicWB.isNull()||cursor.isNull())quitApp(ERRORIMAGEFILENOTFOUND);
    for(int i=0;i<ROLENUMBER;i++)
    {
        if(roleInfo[i]==NULL)continue;
        roleVec.push_back(RoleAndSelect(roleInfo[i]->ID,false));
    }
    roleVec[0].isSelected=true;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFocus();
    this->show();
//    memset(roleArray,0,sizeof(char)*20);
//    assert(vec[0].id==1);//第一个必然是主角
//    roleArray[0]=ROLESELECTED;
//    rolePicVec[0]=new QPixmap();
//    rolePicWBVec[0]=new QPixmap();
//    currentIndex++;
//    for(int i=1;i<roleInfoVec.size();++i)
//    {
//        roleArray[currentIndex]=ROLEUNSELECTED;
//        roleArray[currentIndex]=new QPixmap();
//        roleArray[currentIndex]=new QPixmap();
//    }
//    currentIndex=0;
}
BattleRoleSelectScene::~BattleRoleSelectScene()
{
//    assert(rolePicVec.size()==rolePicWBVec.size());
//    for(int i=0;i<rolePicVec.size();++i)
//    {
//        delete rolePicVec[i];
//        delete rolePicWBVec[i];
//    }
}
void BattleRoleSelectScene::paintEvent(QPaintEvent *)
{

    this->roleInfoWidget.changeToRoleOf(roleVec[currentIndex].id);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing
                           | QPainter::HighQualityAntialiasing, true);
    painter.setBrush(QBrush(QColor(0,51,114,224)));
    painter.drawRect(this->rect());
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    painter.drawText(5,5,200,50,Qt::AlignCenter,battleName);
    painter.drawText(240,410,200,50,Qt::AlignRight,QString("出战人数  %1/%2").arg(currentSelectedNum).arg(maxNum));

    //painter.setPen(Qt::NoPen);
    for(int i=0;i<ROLENUMBER;++i)
    {
        painter.setPen(QPen(Qt::white,1));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(224+i%SELECTROWNUMBER*(ROLEPICSIZE),
                         i/SELECTROWNUMBER*(ROLEPICSIZE)+60,
                         (ROLEPICSIZE),
                         (ROLEPICSIZE)
                         );
    }
    //画边框

    for(int i=0;i<roleVec.size();i++)
    {
        int rolePicX=(roleVec[i].id-1)%SELECTROWNUMBER*ROLEPICSIZE;
        int rolePicY=(roleVec[i].id-1)/SELECTROWNUMBER*ROLEPICSIZE;

        if(roleVec[i].isSelected)
            painter.drawPixmap(224+i%SELECTROWNUMBER*(ROLEPICSIZE+2),
                               i/SELECTROWNUMBER*(ROLEPICSIZE+2)+60,
                           rolePic,
                           rolePicX,
                           rolePicY,
                           ROLEPICSIZE-4,
                           ROLEPICSIZE-4
                           );
        else
            painter.drawPixmap(224+i%SELECTROWNUMBER*(ROLEPICSIZE+2),
                               i/SELECTROWNUMBER*(ROLEPICSIZE+2)+60,
                           rolePicWB,
                           rolePicX,
                           rolePicY,
                           ROLEPICSIZE-4,
                           ROLEPICSIZE-4
                           );
    }
    painter.drawPixmap(224+currentIndex%SELECTROWNUMBER*ROLEPICSIZE,
                       currentIndex/SELECTROWNUMBER*ROLEPICSIZE+60,
                       ROLEPICSIZE,
                       ROLEPICSIZE,
                       cursor);

}
void BattleRoleSelectScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==key->menuKey)
    {
        vector<int> selectedRoleID;
        for(int i=0;i<roleVec.size();i++)
        {
            if(roleVec[i].isSelected)
            {
                selectedRoleID.push_back(roleVec[i].id+1000);
            }
        }
        if(currentSelectedNum==0)return;
        /*
        if(currentSelectedNum<maxNum)
        {
            if(QMessageBox::warning(this,"警告","选择人数未达上限,是否仍然开始?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::No)
            {
                return;
            }
        }
        */
        //这里可以加一个提示,若选择人数不满员是否仍然开始
        emit startBattle(selectedRoleID);
        close();
    }
    else if(event->key()==key->actionKey)
    {
        if(currentIndex==0)
            return;
        if(roleVec[currentIndex].isSelected)
        {//被选中的话,按下action取消之
            roleVec[currentIndex].isSelected=false;

            currentSelectedNum--;
        }
        else
        {//否则检查是否达到上限
            if(currentSelectedNum==maxNum)return;
            roleVec[currentIndex].isSelected=true;
            currentSelectedNum++;
        }
    }
    else if(event->key()==key->cancelKey)
    {
        if(currentIndex==0)
            return;
        if(roleVec[currentIndex].isSelected)
        {//被选中的话,按下action取消之
            roleVec[currentIndex].isSelected=false;
            currentSelectedNum--;
        }
    }
    else if(event->key()==key->upKey)
    {
        if(currentIndex<SELECTROWNUMBER);
        else currentIndex-=SELECTROWNUMBER;
    }
    else if(event->key()==key->downKey)
    {
        if(currentIndex+SELECTROWNUMBER>=roleVec.size());
        else currentIndex+=SELECTROWNUMBER;
    }
    else if(event->key()==key->leftKey)
    {
        if(currentIndex==0)currentIndex=roleVec.size()-1;
        else currentIndex-=1;
    }
    else if(event->key()==key->rightKey)
    {
        if(currentIndex==roleVec.size()-1)currentIndex=0;
        else currentIndex+=1;
    }
    //qDebug()<<currentIndex;
    update();
}
void BattleRoleSelectScene::keyPressEvent(QKeyEvent *)
{}
void BattleRoleSelectScene::timerEvent(QTimerEvent *)
{}
