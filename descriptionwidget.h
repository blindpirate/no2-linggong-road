#ifndef DESCRIPTIONWIDGET_H
#define DESCRIPTIONWIDGET_H

#include "lgl.h"
#include "role.h"
typedef enum
{
    ShowRoleInfo,
    ShowOperationDescription,
    ShowRoleInPoison,//中毒状态
    ShowRoleDisabled,//失去行动能力状态
    ShowRoleAttackDown,
    ShowRoleAccurcyDown,
    ShowRoleDefenceDown,
    ShowRoleDodgeDown,
    ShowRoleSpeedDown,
    ShowRoleCritDown
}DescriptionType;

#define DESCRIPTIONDELAY 1000

class DescriptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DescriptionWidget(QWidget *parent);
    void show(int rolePointX,int rolePointY,Role *roleInfo);
    void show(int rolePointX,int rolePointY,int index);
    void show(int rolePointX, int rolePointY, AnimationOperation animationType);
private:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    
signals:
    void animationEnd();
public slots:
private:
    DescriptionType type;
    int index;
    int x;
    int y;
    Role *role;
    QFont font;
    QPixmap *image;
    int delayTimerID;
    //x,y是绘图原点,对于RoleInfo是x,y+36
    //index指operationWidget的index
    
};

#endif // DESCRIPTIONWIDGET_H
