#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <lgl.h>
#define MENUWIDTH 160
#define MENUHEIGHT 120

typedef enum StoryMenuOrBattleMenu
{
    StoryMenu,
    BattleMenu
}StoryMenuOrBattleMenu;


class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent,KeyboardControl *key,StoryMenuOrBattleMenu which);
    
signals:
    void selectIndex(int index);
    
public slots:
private:
    int currentIndex;
    int indexNum;
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    QFont font;
    KeyboardControl *key;
};

#endif // MENUWIDGET_H
