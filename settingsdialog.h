#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "lgl.h"
#include "dbio.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent,KeyboardControl *key);
    ~SettingsDialog();
    
private slots:
    void on_okButton_clicked();

    void on_setDefaultButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::SettingsDialog *ui;
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    bool eventFilter(QObject *, QEvent *);
    QString keyToString(int key);
    int upKeyValue;
    int downKeyValue;
    int leftKeyValue;
    int rightKeyValue;
    int actionKeyValue;
    int cancelKeyValue;
    int menuKeyValue;
    KeyboardControl *key;
};

#endif // SETTINGSDIALOG_H
