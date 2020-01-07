#ifndef ROLEINFOWIDGET_H
#define ROLEINFOWIDGET_H
//此类根据RoleInfo指针从堆上读取当前各角色的相关信息，根据接收消息显示不同的信息，被多个widget复用
/*roleInfo是一个指针数组，大小为ROLENUMBER宏定义，每个元素是相应角色当前
  的Role指针，如果当前不拥有该角色则指针为NULL。注意数组索引，roleID是从1开始的。
*/
#define ROLEINFOWIDGETWIDTH 240
#define ROLEINFOWIDGETHEIGHT 480
#include "lgl.h"
#include "role.h"

class RoleInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoleInfoWidget(QWidget *parent,Role **roleInfo);
    void changeToRoleOf(int id);
    
signals:
    
public slots:
private:
    void paintEvent(QPaintEvent *);
    int currentRoleID;
    Role **roleInfo;
    QPixmap lvIcon;
    QPixmap hpIcon;
    QPixmap expIcon;
    QPixmap paIcon;
    QPixmap pdIcon;
    QPixmap iqIcon;
//    QPixmap maIcon;
//    QPixmap mdIcon;
    QPixmap moveIcon;
    QFont font;
};

#endif // ROLEINFOWIDGET_H
