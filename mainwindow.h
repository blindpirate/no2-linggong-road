#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lgl.h"
#include "storyscene.h"
#include "battlescene.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "saveloadwidget.h"
#include "role.h"
#include "startbutton.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int logoHeight READ getLogoHeight WRITE setLogoHeight)
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getLogoHeight(){return logoHeight;}
    void setLogoHeight(int value){logoHeight=value;this->update();}
    //void getRoleInfo(vector<RoleInfo> &);
    
private:
    Ui::MainWindow *ui;
    StoryScene *story;
    KeyboardControl *key;
    QPixmap background;
    QPixmap neverawayLogo;
    QPixmap outlight;
    int logoHeight;
    StartButton *startButton;
    StartButton *loadButton;
    StartButton *aboutButton;
    StartButton *quitButton;
    int currentSelectButton;//0~3
    bool userControlOn;
public slots:
    void onBackToMainMenu();
    void on_about_triggered();
    void onStartGame();
    void onQuitGame();
    void onLoadGame();
    void on_option_triggered();
//private slots:
//    void on_action_triggered();

//    void on_action_2_triggered();

private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    bool eventFilter(QObject *, QEvent *);

//    void onBattleSceneBuilt(BattleScene *);
//    void onBattleSceneDestroyed();
private:
    Role *roleInfo[20];
};

#endif // MAINWINDOW_H
