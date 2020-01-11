#include "astar.h"


struct node_F_less:public binary_function<AStarNode *, AStarNode *, bool>
{
    bool operator()(const AStarNode *a, const AStarNode * b)const
    {
        return a->getF()<b->getF();
    }
};

AStarNode* nodeExistInSet(multiset<AStarNode *, node_F_less>& listName,int currentX,int currentY)
{//x,y点是否存在于list中,若存在,返回它的节点指针,否则返回NULL
    for(multiset<AStarNode *, node_F_less>::iterator iter=listName.begin();iter!=listName.end();++iter)
    {
        if((*iter)->getX()==currentX&&(*iter)->getY()==currentY)return *iter;
    }
    return NULL;
}

void AStarPathFinder(const int *mapArray,int column,int row,int startX,int startY,int endX,int endY,vector<QPoint> &path)
{//这个算法里重新计算已经存在于openlist中的节点的FGH值时存在一定问题,另外,path是反序的,从终点指向起点的
    //mapArray实际上是二维指针，但是因为无法指定一维大小，所以当做一维指针来使用
    //x和y是点坐标,寻址应该是[y][x]
    //qDebug()<<mapArray[0];
    //int i=(int)mapArray;
    //qDebug()<<((int **)i)[0][0];

    multiset<AStarNode *, node_F_less> openlist;
    multiset<AStarNode *, node_F_less> closelist;
    //initialize
    AStarNode *currentNode=NULL;
    int currentX=0;
    int currentY=0;

    AStarNode *startNode=new AStarNode(startX,startY);
    openlist.insert(startNode);
    AStarNode *tempNode=NULL;

    while(1)
    {
        //multiset<AStarNode *, node_F_less>::iterator iterNodeWithLeastF=openlist.begin();
        currentNode=*(openlist.begin());
//        QString str;
//        for(multiset<AStarNode *, node_F_less>::iterator iter=openlist.begin();iter!=openlist.end();++iter)
//        {
//            str+=QString(" %1").arg((*iter)->getF());
//        }
        //qDebug()<<str;
        currentX=currentNode->getX();
        currentY=currentNode->getY();

        //qDebug()<<openlist.size();
        openlist.erase(openlist.begin());
        //qDebug()<<openlist.size();
        closelist.insert(currentNode);

//        qDebug()<<currentY<<" "<<currentX;
//        qDebug()<<currentY*column+currentX-1;
//        qDebug()<<(mapArray[currentY*column+currentX-1]);
//        qDebug()<<(nodeExistInSet(closelist,currentX-1,currentY)==NULL);

        if(currentX%column!=0&&
                (mapArray[currentY*column+currentX-1]==0||(currentX-1==endX && currentY==endY))&&
                (nodeExistInSet(closelist,currentX-1,currentY)==NULL))
        {//左边的点

            AStarNode* node=nodeExistInSet(openlist,currentX-1,currentY);
            if(node!=NULL)//已经存在于openlist
            {
                if(currentNode->getF()+1<(node->getF()))//经由当前节点的优于已经存在的node,则更新节点
                {
                    node->changeParent(currentNode);
                    int nodeX=node->getX();
                    int nodeY=node->getY();
                    node->setF(abs(nodeX-currentX)+abs(nodeY-currentY),abs(nodeX-endX)+abs(nodeY-endY));
                }
            }
            else
            {
                openlist.insert(new AStarNode(currentX-1,currentY,currentNode,1,abs(currentY-endY)+abs(currentX-1-endX)));
            }
        }

//        for(multiset<AStarNode *, node_F_less>::iterator iter=closelist.begin();iter!=closelist.end();++iter)
//        {
//            if((*iter)->getX()==currentX&&(*iter)->getY()==currentY);
//        }
        //qDebug()<<mapArray[0][0];
        if(currentX%column!=column-1&&
                (mapArray[currentY*column+currentX+1]==0||(currentX+1==endX &&currentY==endY))&&
                (nodeExistInSet(closelist,currentX+1,currentY)==NULL))
        {
            AStarNode* node=nodeExistInSet(openlist,currentX+1,currentY);
            if(node!=NULL)//已经存在于openlist
            {
                if(currentNode->getF()+1<(node->getF()))//经由当前节点的优于已经存在的node,则更新节点
                {
                    node->changeParent(currentNode);
                    int nodeX=node->getX();
                    int nodeY=node->getY();
                    node->setF(abs(nodeX-currentX)+abs(nodeY-currentY),abs(nodeX-endX)+abs(nodeY-endY));
                }
            }
            else
            {
                openlist.insert(new AStarNode(currentX+1,currentY,currentNode,1,abs(currentY-endY)+abs(currentX+1-endX)));
            }
        }
        if(currentY!=0&&
                (mapArray[(currentY-1)*column+currentX]==0||(currentY-1==endY&&currentX==endX))&&
                (nodeExistInSet(closelist,currentX,currentY-1)==NULL))
        {
            AStarNode* node=nodeExistInSet(openlist,currentX,currentY-1);
            if(node!=NULL)//已经存在于openlist
            {
                if(currentNode->getF()+1<(node->getF()))//经由当前节点的优于已经存在的node,则更新节点
                {
                    node->changeParent(currentNode);
                    int nodeX=node->getX();
                    int nodeY=node->getY();
                    node->setF(abs(nodeX-currentX)+abs(nodeY-currentY),abs(nodeX-endX)+abs(nodeY-endY));
                }
            }
            else
            {
                openlist.insert(new AStarNode(currentX,currentY-1,currentNode,1,abs(currentY-1-endY)+abs(currentX-endX)));
            }
        }
        if(currentY!=row-1&&
                (mapArray[(currentY+1)*column+currentX]==0||(currentY+1==endY && currentX==endX))&&
                (nodeExistInSet(closelist,currentX,currentY+1)==NULL))
        {
            AStarNode* node=nodeExistInSet(openlist,currentX,currentY+1);
            if(node!=NULL)//已经存在于openlist
            {
                if(currentNode->getF()+1<(node->getF()))//经由当前节点的优于已经存在的node,则更新节点
                {
                    node->changeParent(currentNode);
                    int nodeX=node->getX();
                    int nodeY=node->getY();
                    node->setF(abs(nodeX-currentX)+abs(nodeY-currentY),abs(nodeX-endX)+abs(nodeY-endY));
                }
            }
            else
            {
                openlist.insert(new AStarNode(currentX,currentY+1,currentNode,1,abs(currentY+1-endY)+abs(currentX-endX)));
            }
        }
        if(openlist.size()==0)
        {
            //path.swap(vector<QPoint>());
            path.clear();
            for(multiset<AStarNode *, node_F_less>::iterator iter=closelist.begin();iter!=closelist.end();++iter)
            {
                delete (*iter);
                //(*iter)=NULL;
            }
            return;
        }
        tempNode=nodeExistInSet(openlist,endX,endY);
        if(tempNode!=NULL)
        {
            while(tempNode!=NULL)
            {
                path.push_back(QPoint(tempNode->getX(),tempNode->getY()));
                tempNode=tempNode->getParentNode();
            }
            for(multiset<AStarNode *, node_F_less>::iterator iter=closelist.begin();iter!=closelist.end();++iter)
            {
                delete (*iter);
                //(*iter)=NULL;
            }
            for(multiset<AStarNode *, node_F_less>::iterator iter=openlist.begin();iter!=openlist.end();++iter)
            {
                delete (*iter);
                //(*iter)=NULL;
            }
            return;
        }
    }

}
