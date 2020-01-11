/*
  该类显示一个界面提示用户进行读写档操作
*/
#ifndef SAVELOADWIDGET_H
#define SAVELOADWIDGET_H
#include"lgl.h"
#include "dbio.h"
#define FILEWIDGETWIDTH 480
#define SINGLEFILEHEIGHT 120//单条档位的高度
//#define FILEWIDGETHEIGHT 1000


typedef enum SaveOrLoad
{
    OperationLoad=0,OperationSave=1
}SaveOrLoad;

typedef struct SaveFileData
{
    SaveFileData(QString const &fileName,QString const &time,const int roleNum):
        fileName(fileName),
        time(time),
        roleNum(roleNum)
    {}
    QString fileName;
    QString time;
    int roleNum;
}SaveFileData;

class SaveLoadWidget:public QWidget
{
    Q_OBJECT
public:
    SaveLoadWidget(QWidget *parent,KeyboardControl *key,SaveOrLoad saveOrLoad);
    ~SaveLoadWidget();
public slots:
    void onScrollBarChanged(int value);
signals:
    void saveToFile();
    void cancel();
    void loadFrom(QString const& fileName);
private:
    QScrollBar *bar;
    int saveFileCount;
    KeyboardControl *key;
    int currentSelectIndex;//最多有100个档位,0~99
    int currentBottomIndex;//当前最上方的档位索引值
    int currentTopIndex;//当前底部的档位索引值
    SaveOrLoad type;
    //QWidget *fileWidget;
    vector<SaveFileData> saveFileVec;
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void indexIncrease();
    void indexDecrease();//每次增减1
    void indexSelected();
};

#endif // LOADWIDGET_H
