#ifndef ASTAR_H
#define ASTAR_H
#include "lgl.h"

class AStarNode
{
public:
    AStarNode(int x,int y,AStarNode *parent=NULL,int G=0,int H=0):
        x(x),
        y(y),
        parent(parent),
        G(G),
        H(H)
    {
        F=G+H;
    }
    ~AStarNode()
    {
    }
    int getF()const{return F;}
    int getX()const{return x;}
    int getY()const{return y;}
    AStarNode *getParentNode()const{return parent;}
    void changeParent(AStarNode *newParent){parent=newParent;}
    void setF(int newG,int newH){G=newG;H=newH;F=G+H;}
//    bool operator<(const AStarNode &anotherNode)const
//    {
//        return F<anotherNode.F;
//    }
private:
    int x;
    int y;
    int F;
    int G;
    int H;
    AStarNode *parent;

};
#endif // ASTAR_H
