/*
  此类主要用于在战斗中显示"战斗开始""第X回合"等字样
*/

#ifndef BATTLELABEL_H
#define BATTLELABEL_H

#define BATTLELABELWIDTH 200
#define BATTLELABELHEIGHT 80

#include "lgl.h"

class BattleLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentY READ getCurrentY WRITE setCurrentY)
public:
    explicit BattleLabel(QWidget *parent,QString const&text);
    ~BattleLabel(){delete propertyAnimation;}
    void setCurrentY(int y)
    {
        currentY=y;
        this->setGeometry((WINDOWWIDTH-BATTLELABELWIDTH)/2,currentY,BATTLELABELWIDTH,BATTLELABELHEIGHT);
        this->update();
    }
    int getCurrentY(){return currentY;}
signals:
    
public slots:
private:
    QString text;
    QFont font;
    int currentY;
    QPropertyAnimation *propertyAnimation;
    void paintEvent(QPaintEvent *);

    
};

#endif // BATTLELABEL_H
