#ifndef DIALOGLABEL_H
#define DIALOGLABEL_H

#include "lgl.h"
#include <vector>

#define DIALOGLABELW 460
#define DIALOGLABELH 150
#define TEXTORIGINX 105
#define TEXTORIGINY 39

class DialogLabel : public QWidget
{
    Q_OBJECT
public:
    explicit DialogLabel(QWidget *parent,QString &param);
    //explicit DialogLabel(QWidget *parent,QString const &,QPixmap const &);
    ~DialogLabel();    
private:
    QString text;
    QString roleName;
    QPixmap image;
    QFont font;
    QVector <int> beginPosVec;//每一帧的开始字符的index
    int currentEndPos;//指向当前显示的帧的结束字符的index
    int currentX;
    int currentY;//当前输出的汉字左下角在widget坐标系中的坐标初始应该是15,39
    //注意是左下角
    int triTimerID;
    int animTimerID;
    bool animationOn;//加载和退出时候的动画标志，显示动画时不响应键盘，不显示文字
    bool textIsRed;//文字高亮标志,这个标志主要是为了能在翻页的时候红色字体连续显示
    int dialogHeight;
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    bool triangleIsDown;
    bool triangleExist;
    void quit();
    QPoint *trianglePoints;
};

#endif // DIALOGLABEL_H
