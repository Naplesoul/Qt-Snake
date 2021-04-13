#include "mainwindow.h"
//  主要注释位于mainwindow.h
//  本cpp文件中主要是用于传递函数之间的调用，以及按button后的反应

int MainWindow::width = 1240;
int MainWindow::height = 1240;
int PlayWindow::fps = 8;

StartWindow::StartWindow(QWidget *parent)
{
    resize(MainWindow::width,MainWindow::height);
    setParent(parent);

    startGameButton = new QPushButton("Start Game", this);
    startGameButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    startGameButton->move(450, 600);
    connect(startGameButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::showChooseModeWindow);
    qDebug() << "startGameButton constructed" << Qt::endl;

    loadGameButton = new QPushButton("Load Game", this);
    loadGameButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    loadGameButton->move(455, 700);
    connect(loadGameButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::loadGame);
    qDebug() << "loadGameButton constructed" << Qt::endl;

    customizeMapButton = new QPushButton("Customize Map", this);
    customizeMapButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    customizeMapButton->move(410, 800);
    connect(customizeMapButton, &QPushButton::clicked, this, &StartWindow::setCustomizeEntryMode);
    qDebug() << "customizeMapButton constructed" << Qt::endl;

    quitButton = new QPushButton("Quit", this);
    quitButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    quitButton->move(515, 900);
    connect(quitButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::sendQuitGame);
    qDebug() << "customizeMapButton constructed" << Qt::endl;


    qDebug() << "StartWindow constructed" << Qt::endl;

}

void StartWindow::setCustomizeEntryMode()
{
    emit(enterCustomizeMode());
}

ChooseModeWindow::ChooseModeWindow(QWidget *parent)
{
    resize(MainWindow::width,MainWindow::height);
    setParent(parent);

    singlePlayerButton = new QPushButton("SOLO", this);
    singlePlayerButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    singlePlayerButton->move(515, 600);
    connect(singlePlayerButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::setSOLO);

    twoPlayerButton = new QPushButton("PVP", this);
    twoPlayerButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    twoPlayerButton->move(525, 700);
    connect(twoPlayerButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::setPVP);

    fightComputerButton = new QPushButton("PVE", this);
    fightComputerButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    fightComputerButton->move(525,800);
    connect(fightComputerButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::setPVE);

    return2MenuButton = new QPushButton("Return to Menu", this);
    return2MenuButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    return2MenuButton->move(405, 900);
    connect(return2MenuButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::showStartWindow);

    qDebug() << "ChooseModeWindow constructed" << Qt::endl;

}


PauseWindow::PauseWindow(QWidget *parent)
{
    resize(MainWindow::width,MainWindow::height);
    setParent(parent);

    resumeButton = new QPushButton("Resume", this);
    resumeButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    resumeButton->move(505, 600);
    connect(resumeButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::showPlayWindow);

    restartButton = new QPushButton("Restart", this);
    restartButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    restartButton->move(500, 700);
    connect(restartButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::restartGame);

    saveGameButton = new QPushButton("Save Game", this);
    saveGameButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    saveGameButton->move(470, 800);
    connect(saveGameButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::saveGame);

    customizeMapButton = new QPushButton("Customize Map", this);
    customizeMapButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    customizeMapButton->move(435, 900);
    connect(customizeMapButton, &QPushButton::clicked, this, &PauseWindow::setCustomizeEntryMode);

    return2MenuButton = new QPushButton("Return to Menu", this);
    return2MenuButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    return2MenuButton->move(420, 1000);
    connect(return2MenuButton, &QPushButton::clicked, this, &PauseWindow::return2Menu);

    qDebug() << "PauseWindow constructed" << Qt::endl;

}

void PauseWindow::setCustomizeEntryMode()
{
    emit(enterCustomizeMode());
}

void PauseWindow::return2Menu()
{
    emit(pauseReturn2Menu());
}

CustomizeMapWindow::CustomizeMapWindow(QWidget *parent)
{
    resize(MainWindow::width,MainWindow::height);
    setParent(parent);

    qDebug() << "CustomizeMapWindow constructed" << Qt::endl;

}

void CustomizeMapWindow::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    emit(paint(painter));
}

void CustomizeMapWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    x /= 40;
    y /= 40;
    POINT p(x,y);
    if(event->button() == Qt::LeftButton)
    {
        emit(changeBlock(p));
    }
    else if(event->button() == Qt::RightButton)
    {
        emit(changeFood(p));
    }
    update();
}

PlayWindow::PlayWindow(QWidget *parent)
{
    resize(MainWindow::width,MainWindow::height);
    setParent(parent);
    qDebug() << "controller begin to construct";
    controller = new Controller(playMode, this, fps);

    connect(controller, &Controller::gameOver, static_cast<MainWindow*>(parent), &MainWindow::showEndGameWindow);
    connect(controller, &Controller::remainSeconds, this, &PlayWindow::setTitle);
    connect(controller, &Controller::paused, static_cast<MainWindow*>(parent), &MainWindow::showPauseWindow);

    qDebug() << "PlayWindow constructed" << Qt::endl;
}

void PlayWindow::paint(QPainter &painter)
{
    controller->paintScene(painter);
}

void PlayWindow::setPlayMode(PLAYMODE m)
{
    controller->playMode = m;
}

void PlayWindow::setReady()
{
    controller->setReady();
}

void PlayWindow::restartGame()
{
    controller->restartGame();
}

void PlayWindow::resetMap()
{
    controller->resetMap();
}

void PlayWindow::saveGame()
{
    controller->saveGame();
}
void PlayWindow::loadGame()
{
    controller->loadGame();
}

void PlayWindow::setTitle(int sec)
{
    switch(sec)
    {
    case 0:
        emit(setMainTitle("SNAKE!!! -- GAMING"));
        break;
    case 1:
        emit(setMainTitle("READY TO START -- 1s"));
        break;
    case 2:
        emit(setMainTitle("READY TO START -- 2s"));
        break;
    case 3:
        emit(setMainTitle("READY TO START -- 3s"));
        break;
    }
}

void PlayWindow::keyPressed(QKeyEvent *event)
{
    controller->keyPressed(event);
}

void PlayWindow::changeBlock(POINT &p)
{
    controller->changeBlock(p);
}

void PlayWindow::changeFood(POINT &p)
{
    controller->changeFood(p);
}


void PlayWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    controller->paint(painter);
}

EndGameWindow::EndGameWindow(QWidget *parent)
{
    setParent(parent);
    return2MenuButton = new QPushButton("Return to Menu", this);
    return2MenuButton->setStyleSheet("QPushButton{ font-family:'BankGothic Lt Bt';font-size:40px}");
    connect(return2MenuButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::pauseReturn2Menu);

}

void EndGameWindow::paintEvent(QPaintEvent* )
{
    QFont font("BankGothic Lt Bt", 10);
    QPainter painter(this);
    painter.setFont(font);
    if(playMode == SOLO)
    {
        if(loser == 1)
            painter.drawText(500,400, "YOU LOSE");
        else
            painter.drawText(500,400, "YOU WIN");
    }
    else
    {
        if(loser == 1)
            painter.drawText(480,400, "SNAKE2 WIN");
        else
            painter.drawText(480,400, "SNAKE1 WIN");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), displayMode(START)
{
    resize(width,height);
    setWindowTitle("SNAKE!!!");

    startWindow = new StartWindow(this);
    startWindow->show();
    qDebug() << "startWindow showed";
    chooseModeWindow = new ChooseModeWindow(this);
    chooseModeWindow->hide();

    pauseWindow = new PauseWindow(this);
    pauseWindow->hide();

    customizeMapWindow = new CustomizeMapWindow(this);
    customizeMapWindow->hide();

    endGameWindow = new EndGameWindow(this);
    endGameWindow->hide();

    playWindow = new PlayWindow(this);
    playWindow->hide();

    connect(playWindow, &PlayWindow::setMainTitle, this, &MainWindow::setWindowTitle);

    connect(startWindow, &StartWindow::enterCustomizeMode, this, &MainWindow::startEnterCustomize);
    connect(pauseWindow, &PauseWindow::enterCustomizeMode, this, &MainWindow::pauseEnterCustomize);
    connect(pauseWindow, &PauseWindow::pauseReturn2Menu, this, &MainWindow::pauseReturn2Menu);
    connect(customizeMapWindow, &CustomizeMapWindow::paint, this, &MainWindow::paint);
    connect(customizeMapWindow, &CustomizeMapWindow::changeBlock, this, &MainWindow::changeBlock);
    connect(customizeMapWindow, &CustomizeMapWindow::changeFood, this, &MainWindow::changeFood);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(displayMode == PLAY)
        playWindow->keyPressed(event);
    if(displayMode == CUSTOMIZE && event->key() == Qt::Key_Escape)
    {
        if(customizeMapWindow->entryMode == PAUSE)
        {
            showPauseWindow();
        }
        else
        {
            showStartWindow();
        }
    }
}


void MainWindow::sendQuitGame()
{
    emit quitGame();
}

void MainWindow::pauseEnterCustomize()
{
    customizeMapWindow->entryMode = PAUSE;
    showCustomizeMapWindow();
}

void MainWindow::pauseReturn2Menu()
{
    playWindow->resetMap();
    playWindow->restartGame();
    showStartWindow();
}

void MainWindow::startEnterCustomize()
{
    customizeMapWindow->entryMode = START;
    showCustomizeMapWindow();
}

void MainWindow::showStartWindow()
{
    setWindowTitle("Welcome to SNAKE!!!");
    Controller::status = PAUSED;
    playWindow->hide();
    chooseModeWindow->hide();
    pauseWindow->hide();
    customizeMapWindow->hide();
    endGameWindow->hide();
    startWindow->show();
    displayMode = START;
}

void MainWindow::showChooseModeWindow()
{
    setWindowTitle("CHOOSE MODE");
    Controller::status = PAUSED;
    playWindow->hide();
    startWindow->hide();
    pauseWindow->hide();
    customizeMapWindow->hide();
    endGameWindow->hide();
    chooseModeWindow->show();
    displayMode = CHOOSE;
}

void MainWindow::showPauseWindow()
{
    setWindowTitle("PAUSED");
    Controller::status = PAUSED;
    playWindow->hide();
    startWindow->hide();
    chooseModeWindow->hide();
    customizeMapWindow->hide();
    endGameWindow->hide();
    pauseWindow->show();
    displayMode = PAUSE;
}

void MainWindow::showCustomizeMapWindow()
{
    setWindowTitle("CUSTOMIZE MAP");
    Controller::status = PAUSED;
    playWindow->hide();
    startWindow->hide();
    chooseModeWindow->hide();
    pauseWindow->hide();
    endGameWindow->hide();
    customizeMapWindow->show();
    displayMode = CUSTOMIZE;
}

void MainWindow::showPlayWindow()
{
    Controller::status = READY;
    playWindow->setReady();
    playWindow->show();
    playWindow->setFocus();
    startWindow->hide();
    chooseModeWindow->hide();
    pauseWindow->hide();
    endGameWindow->hide();
    customizeMapWindow->hide();
    displayMode = PLAY;
}

void MainWindow::showEndGameWindow(int l, PLAYMODE mode)
{
    setWindowTitle("GAMEOVER");
    Controller::status = PAUSED;
    playWindow->hide();
    startWindow->hide();
    chooseModeWindow->hide();
    pauseWindow->hide();
    customizeMapWindow->hide();
    endGameWindow->loser = l;
    endGameWindow->playMode = mode;
    endGameWindow->update();
    endGameWindow->show();
    displayMode = GAMEOVER;
}

void MainWindow::paint(QPainter &painter)
{
    playWindow->paint(painter);
}

void MainWindow::changeBlock(POINT &p)
{
    playWindow->changeBlock(p);
}
void MainWindow::changeFood(POINT &p)
{
    playWindow->changeFood(p);
}


void MainWindow::setSOLO()
{
    playWindow->setPlayMode(SOLO);
    showPlayWindow();
}

void MainWindow::setPVP()
{
    playWindow->setPlayMode(PVP);
    showPlayWindow();
}

void MainWindow::setPVE()
{
    playWindow->setPlayMode(PVE);
    showPlayWindow();
}

void MainWindow::restartGame()
{
    playWindow->restartGame();
    showPlayWindow();
}

void MainWindow::saveGame()
{
    playWindow->saveGame();
    showStartWindow();
}

void MainWindow::loadGame()
{
    playWindow->loadGame();
    showPlayWindow();
}
