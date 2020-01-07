#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, KeyboardControl *key)
    : QDialog(parent), ui(new Ui::SettingsDialog), upKeyValue(key->upKey),
      downKeyValue(key->downKey), leftKeyValue(key->leftKey),
      rightKeyValue(key->rightKey), actionKeyValue(key->actionKey),
      cancelKeyValue(key->cancelKey), menuKeyValue(key->menuKey), key(key) {
  ui->setupUi(this);
  ui->upKey->setText(keyToString(upKeyValue));
  ui->downKey->setText(keyToString(downKeyValue));
  ui->leftKey->setText(keyToString(leftKeyValue));
  ui->rightKey->setText(keyToString(rightKeyValue));
  ui->actionKey->setText(keyToString(actionKeyValue));
  ui->cancelKey->setText(keyToString(cancelKeyValue));
  ui->menuKey->setText(keyToString(menuKeyValue));

  ui->upKey->installEventFilter(this);
  ui->downKey->installEventFilter(this);
  ui->leftKey->installEventFilter(this);
  ui->rightKey->installEventFilter(this);
  ui->actionKey->installEventFilter(this);
  ui->cancelKey->installEventFilter(this);
  ui->menuKey->installEventFilter(this);

  this->setWindowTitle(QString("设置"));
}

QString SettingsDialog::keyToString(int key) {
  if (key >= Qt::Key_A && key <= Qt::Key_Z)
    return QString('A' + key - Qt::Key_A);
  switch (key) {
  case Qt::Key_Left:
    return QString("Left");
  case Qt::Key_Right:
    return QString("Right");
  case Qt::Key_Up:
    return QString("Up");
  case Qt::Key_Down:
    return QString("Down");
  case Qt::Key_Space:
    return QString("Space");
  case Qt::Key_Escape:
    return QString("Esc");
  case Qt::Key_Return:
    return QString("Enter");
  default:
    return QString("");
  }
}

void SettingsDialog::keyPressEvent(QKeyEvent *event) {}
void SettingsDialog::keyReleaseEvent(QKeyEvent *event) {
  //    if(ui->upKey->hasFocus())ui->upKey->setText(keyToString(event->key()));
  //    else
  //    if(ui->downKey->hasFocus())ui->downKey->setText(keyToString(event->key()));
  //    else
  //    if(ui->rightKey->hasFocus())ui->rightKey->setText(keyToString(event->key()));
  //    else
  //    if(ui->leftKey->hasFocus())ui->leftKey->setText(keyToString(event->key()));
  //    else
  //    if(ui->actionKey->hasFocus())ui->actionKey->setText(keyToString(event->key()));
  //    else
  //    if(ui->cancelKey->hasFocus())ui->cancelKey->setText(keyToString(event->key()));
  //    else
  //    if(ui->menuKey->hasFocus())ui->menuKey->setText(keyToString(event->key()));
}

bool SettingsDialog::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    if ((((QKeyEvent *)event)->isAutoRepeat()))
      return true;
    //        if(obj==ui->upKey||
    //                obj==ui->downKey||
    //                obj==ui->leftKey||
    //                obj==ui->rightKey||
    //                obj==ui->actionKey||
    //                obj==ui->cancelKey||
    //                obj==ui->menuKey)
    {
      if (ui->upKey->hasFocus()) {
        upKeyValue = ((QKeyEvent *)event)->key();
        ui->upKey->setText(keyToString(upKeyValue));
      } else if (ui->downKey->hasFocus()) {
        downKeyValue = ((QKeyEvent *)event)->key();
        ui->downKey->setText(keyToString(downKeyValue));
      } else if (ui->rightKey->hasFocus()) {
        rightKeyValue = ((QKeyEvent *)event)->key();
        ui->rightKey->setText(keyToString(rightKeyValue));
      } else if (ui->leftKey->hasFocus()) {
        leftKeyValue = ((QKeyEvent *)event)->key();
        ui->leftKey->setText(keyToString(leftKeyValue));
      } else if (ui->actionKey->hasFocus()) {
        actionKeyValue = ((QKeyEvent *)event)->key();
        ui->actionKey->setText(keyToString(actionKeyValue));
      } else if (ui->cancelKey->hasFocus()) {
        cancelKeyValue = ((QKeyEvent *)event)->key();
        ui->cancelKey->setText(keyToString(cancelKeyValue));
      } else if (ui->menuKey->hasFocus()) {
        menuKeyValue = ((QKeyEvent *)event)->key();
        ui->menuKey->setText(keyToString(menuKeyValue));
      }
      return true;
    }

  } else if (event->type() == QEvent::KeyRelease)
    return true;
  else
    return QObject::eventFilter(obj, event);
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::on_okButton_clicked() {
  key->upKey = upKeyValue;
  key->downKey = downKeyValue;
  key->leftKey = leftKeyValue;
  key->rightKey = rightKeyValue;
  key->actionKey = actionKeyValue;
  key->cancelKey = cancelKeyValue;
  key->menuKey = menuKeyValue;
  DBIO dbio;
  dbio.setKeySettings(upKeyValue, downKeyValue, leftKeyValue, rightKeyValue,
                      actionKeyValue, cancelKeyValue, menuKeyValue);
  accept();
}

void SettingsDialog::on_setDefaultButton_clicked() {
  upKeyValue = Qt::Key_Up;
  downKeyValue = Qt::Key_Down;
  leftKeyValue = Qt::Key_Left;
  rightKeyValue = Qt::Key_Right;
  actionKeyValue = Qt::Key_A;
  cancelKeyValue = Qt::Key_S;
  menuKeyValue = Qt::Key_Space;
  ui->upKey->setText(keyToString(upKeyValue));
  ui->downKey->setText(keyToString(downKeyValue));
  ui->leftKey->setText(keyToString(leftKeyValue));
  ui->rightKey->setText(keyToString(rightKeyValue));
  ui->actionKey->setText(keyToString(actionKeyValue));
  ui->cancelKey->setText(keyToString(cancelKeyValue));
  ui->menuKey->setText(keyToString(menuKeyValue));
}

void SettingsDialog::on_cancelButton_clicked() { reject(); }
