#ifndef GETGOODSLABEL_H
#define GETGOODSLABEL_H

#include "lgl.h"

#define GETGOODSLABELW 100
#define GETGOODSLABELH 20

class GetGoodsLabel : public QWidget
{
    Q_OBJECT
public:
    explicit GetGoodsLabel(QWidget *parent,QString const &goodsName);
    
signals:

    
public slots:
private:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);
    QString text;
    
};

#endif // GETGOODSLABEL_H
