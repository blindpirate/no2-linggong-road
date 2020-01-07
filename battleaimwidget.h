#ifndef BATTLEAIMWIDGET_H
#define BATTLEAIMWIDGET_H

#include "lgl.h"

class BattleAimWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BattleAimWidget(QWidget *parent,QString &win,QString &fail);
    
signals:
    
public slots:
private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    QString winRequirement;
    QString failRequirement;
    
};

#endif // BATTLEAIMWIDGET_H
