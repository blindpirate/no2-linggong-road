#ifndef WEAPONWIDGET_H
#define WEAPONWIDGET_H
#include "lgl.h"
#include "role.h"
#include "dbio.h"

typedef enum WeaponOrMedicine
{
    ShowNoWeaponNorMedicine,
    ShowWeapon,
    ShowMedicine
}WeaponOrMedicine;


class WeaponWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WeaponWidget(QWidget *parent,map<int,ArticleInfo> *pArticleIDToInfoMap);
    ~WeaponWidget();
    void show(int rolePointX,int rolePointY,Role *roleInfo,WeaponOrMedicine type);
    int indexGoUp();
    int indexGoDown();
signals:
    
public slots:
private:
    WeaponOrMedicine type;
    int x;
    int y;
    Role *role;
    int currentIndex;
    int weaponSum;
    map<int,ArticleInfo> *pArticleIDToInfoMap;
    void paintEvent(QPaintEvent *);
};

#endif // WEAPONWIDGET_H
