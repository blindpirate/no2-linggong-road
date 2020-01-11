#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "scene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), story(NULL),
      key(new KeyboardControl), startButton(NULL), aboutButton(NULL),
      quitButton(NULL), loadButton(NULL), logoHeight(0), currentSelectButton(0),
      userControlOn(false),
      background(QPixmap(QApplication::applicationDirPath() +
                         QString("/data/image/other/background.png"))),
      neverawayLogo(QPixmap(QApplication::applicationDirPath() +
                            QString("/data/image/other/neveraway.png"))),
      outlight(QPixmap(QApplication::applicationDirPath() +
                       QString("/data/image/other/outlight.png"))) {
  ui->setupUi(this);

  DBIO dbio;
  dbio.getKeySettings(key->upKey, key->downKey, key->leftKey, key->rightKey,
                      key->actionKey, key->cancelKey, key->menuKey);

  //    QPalette palette = this->palette();
  //    palette.setBrush(QPalette::Active,QPalette::Window,QBrush(Qt::black));
  //    this->setPalette(palette);
  //    this->setAutoFillBackground(true);

  this->setWindowTitle(QString("凌工路2号 Alpha"));
  this->setWindowIcon(QIcon("lgl.ico"));
  ui->menuBar->setNativeMenuBar(false);
  //    this->ui->action->setDisabled(true);

  //    start->show();
  //    connect(start->aboutButton,SIGNAL(clicked()),this,SLOT(on_about_triggered()));
  //    connect(start->startButton,SIGNAL(clicked()),this,SLOT(onStartGame()));
  //    connect(start->loadButton,SIGNAL(clicked()),this,SLOT(onLoadGame()));
  //    connect(start->quitButton,SIGNAL(clicked()),this,SLOT(onQuitGame()));
  startButton = new StartButton(this, "startpressed", "startnormal", 147, 180);
  loadButton = new StartButton(this, "loadpressed", "loadnormal", 147, 256);
  aboutButton = new StartButton(this, "aboutpressed", "aboutnormal", 147, 332);
  quitButton = new StartButton(this, "quitpressed", "quitnormal", 147, 408);
  startButton->show();
  loadButton->show();
  aboutButton->show();
  quitButton->show();

  connect(aboutButton, SIGNAL(clicked()), this, SLOT(on_about_triggered()));
  connect(startButton, SIGNAL(clicked()), this, SLOT(onStartGame()));
  connect(loadButton, SIGNAL(clicked()), this, SLOT(onLoadGame()));
  connect(quitButton, SIGNAL(clicked()), this, SLOT(onQuitGame()));

  QPropertyAnimation *animation =
      new QPropertyAnimation(this, "logoHeight", this);
  animation->setDuration(2000);
  animation->setStartValue(0);
  animation->setKeyValueAt(0.3, 0);
  animation->setKeyValueAt(0.7, neverawayLogo.height());
  animation->setEndValue(neverawayLogo.height());
  animation->start();
  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  userControlOn = true;
  // s->setFocus();
  //    RoleView *r=new RoleView(this,7,7);
  //    r->setFocusPolicy(Qt::StrongFocus);
  //     //   r->show();
  //    r->setFocus();
}

MainWindow::~MainWindow() {
  delete ui;
  delete story;
  delete key;
}

void MainWindow::on_about_triggered() {
  AboutDialog dlg;
  dlg.exec();
}
void MainWindow::onStartGame() {
  //    this->ui->action_2->setDisabled(true);
  userControlOn = false;
  story = new StoryScene(this, key, true);
  startButton->close();
  loadButton->close();
  quitButton->close();
  aboutButton->close();
  startButton = NULL;
  loadButton = NULL;
  quitButton = NULL;
  aboutButton = NULL;
  connect(story, SIGNAL(goBackToMainMenu()), this, SLOT(onBackToMainMenu()));
  //    connect(story,SIGNAL(battleSceneBuilt(BattleScene*)),this,SLOT(onBattleSceneBuilt(BattleScene*)));
  //    connect(story,SIGNAL(battleSceneDestroyed()),this,SLOT(onBattleSceneDestroyed()));
}
void MainWindow::onBackToMainMenu() {
  startButton = new StartButton(this, "startpressed", "startnormal", 147, 180);
  loadButton = new StartButton(this, "loadpressed", "loadnormal", 147, 256);
  aboutButton = new StartButton(this, "aboutpressed", "aboutnormal", 147, 332);
  quitButton = new StartButton(this, "quitpressed", "quitnormal", 147, 408);
  startButton->show();
  loadButton->show();
  aboutButton->show();
  quitButton->show();

  connect(aboutButton, SIGNAL(clicked()), this, SLOT(on_about_triggered()));
  connect(startButton, SIGNAL(clicked()), this, SLOT(onStartGame()));
  connect(loadButton, SIGNAL(clicked()), this, SLOT(onLoadGame()));
  connect(quitButton, SIGNAL(clicked()), this, SLOT(onQuitGame()));

  story = NULL;
  this->show();
  this->setFocus();
  this->userControlOn = true;
  this->update();
}
void MainWindow::onLoadGame() {
  userControlOn = false;
  startButton->close();
  loadButton->close();
  quitButton->close();
  aboutButton->close();
  startButton = NULL;
  loadButton = NULL;
  quitButton = NULL;
  aboutButton = NULL;
  story = new StoryScene(this, key, false);

  SaveLoadWidget *w = new SaveLoadWidget(story, key, OperationLoad);
  w->setGeometry(0, 0, WINDOWWIDTH, WINDOWHEIGHT * 2);

  connect(w, SIGNAL(loadFrom(QString)), story, SLOT(loadScene(QString)));
  connect(w, SIGNAL(cancel()), story, SLOT(close()));
  connect(w, SIGNAL(cancel()), this, SLOT(onBackToMainMenu()));
  connect(story, SIGNAL(goBackToMainMenu()), this, SLOT(onBackToMainMenu()));
}
void MainWindow::onQuitGame() { qApp->quit(); }
void MainWindow::on_option_triggered() {
  SettingsDialog dlg(this, key);
  //    if(story)
  //    {
  //        connect(&dlg,SIGNAL(actionKeyChangeTo(int)),story,SLOT(changeActionKeyTo(int)));
  //        connect(&dlg,SIGNAL(cancelKeyChangeTo(int)),story,SLOT(changeCancelKeyTo(int)));
  //        connect(&dlg,SIGNAL(menuKeyChangeTo(int)),story,SLOT(changeMenuKeyTo(int)));
  //        connect(&dlg,SIGNAL(upKeyChangeTo(int)),story,SLOT(changeUpKeyTo(int)));
  //        connect(&dlg,SIGNAL(downKeyChangeTo(int)),story,SLOT(changeDownKeyTo(int)));
  //        connect(&dlg,SIGNAL(leftKeyChangeTo(int)),story,SLOT(changeLeftKeyTo(int)));
  //        connect(&dlg,SIGNAL(rightKeyChangeTo(int)),story,SLOT(changeRightKeyTo(int)));
  //    }
  //    if(battle)
  //    {
  //        connect(&dlg,SIGNAL(actionKeyChangeTo(int)),battle,SLOT(changeActionKeyTo(int)));
  //        connect(&dlg,SIGNAL(cancelKeyChangeTo(int)),battle,SLOT(changeCancelKeyTo(int)));
  //        connect(&dlg,SIGNAL(menuKeyChangeTo(int)),battle,SLOT(changeMenuKeyTo(int)));
  //        connect(&dlg,SIGNAL(upKeyChangeTo(int)),battle,SLOT(changeUpKeyTo(int)));
  //        connect(&dlg,SIGNAL(downKeyChangeTo(int)),battle,SLOT(changeDownKeyTo(int)));
  //        connect(&dlg,SIGNAL(leftKeyChangeTo(int)),battle,SLOT(changeLeftKeyTo(int)));
  //        connect(&dlg,SIGNAL(rightKeyChangeTo(int)),battle,SLOT(changeRightKeyTo(int)));
  //    }
  // if(dlg.exec()==QDialog::Rejected)return;
  dlg.exec();
}
void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  if (story) {
    painter.eraseRect(this->rect());
    return;
  }
  painter.drawPixmap(0, 20, background);
  painter.drawPixmap(55, 67, neverawayLogo.width(), logoHeight, neverawayLogo);
  switch (currentSelectButton) {
  case 0:
    painter.drawPixmap(93, 168, 300, 100, outlight);
    break;
  case 1:
    painter.drawPixmap(93, 242, 300, 100, outlight);
    break;
  case 2:
    painter.drawPixmap(93, 318, 300, 100, outlight);
    break;
  case 3:
    painter.drawPixmap(93, 392, 300, 100, outlight);
    break;
  default:
    qDebug() << currentSelectButton;
    assert(0);
    break;
  }
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
  if (!userControlOn)
    return;
}
void MainWindow::keyReleaseEvent(QKeyEvent *e) {
  if (!userControlOn)
    return;
  if (e->isAutoRepeat())
    return;
  if (e->key() == key->upKey) {
    if (currentSelectButton == 0)
      currentSelectButton = 4;
    currentSelectButton--;
  } else if (e->key() == key->downKey) {
    if (currentSelectButton == 3)
      currentSelectButton = -1;
    currentSelectButton++;
  } else if (e->key() == key->actionKey) {
    switch (currentSelectButton) {
    case 0:
      this->onStartGame();
      break;
    case 1:
      this->onLoadGame();
      break;
    case 2:
      this->on_about_triggered();
      break;
    case 3:
      this->onQuitGame();
      break;
    default:
      qDebug() << currentSelectButton;
      assert(0);
      break;
    }
  }
  this->update();
}

bool MainWindow::eventFilter(
    QObject *obj,
    QEvent *
        event) { //这个函数主要是为了截留鼠标移动的消息,使得可以让鼠标对应的按钮发光
  if (event->type() == QEvent::MouseMove)
  // if(obj==startButton)
  {
    if (obj == startButton) {
      if (startButton->getPressed())
        return QMainWindow::eventFilter(obj, event);
      else {
        currentSelectButton = 0;
        this->update();
        return true;
      }
    } else if (obj == loadButton) {
      if (loadButton->getPressed())
        return QMainWindow::eventFilter(obj, event);
      else {
        currentSelectButton = 1;
        this->update();
        return true;
      }
    } else if (obj == aboutButton) {
      if (aboutButton->getPressed())
        return QMainWindow::eventFilter(obj, event);
      else {
        currentSelectButton = 2;
        this->update();
        return true;
      }
    } else if (obj == quitButton) {
      if (quitButton->getPressed())
        return QMainWindow::eventFilter(obj, event);
      else {
        currentSelectButton = 3;
        this->update();
        return true;
      }
    }
  } else
    return QMainWindow::eventFilter(obj, event);
}

// void MainWindow::onBattleSceneBuilt(BattleScene *newBattleScene)
//{
//    battle=newBattleScene;
//}
// void MainWindow::onBattleSceneDestroyed()
//{
//    battle=NULL;
//}

// void MainWindow::on_action_triggered()
//{
//    assert(story);
//    story->onSaveData();
//}

// void MainWindow::on_action_2_triggered()
//{
//    onLoadGame();
//}
