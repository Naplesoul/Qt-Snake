#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <math.h>
#include "initialize.h"
#include "controller.h"


//  游戏初始界面
//  有4个buttons：Start Game, Load Game, Customize Map, Quit
class StartWindow : public QWidget
{
    Q_OBJECT
public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow() {}

    void setCustomizeEntryMode();
private:
    QPushButton* startGameButton;
    QPushButton* loadGameButton;
    QPushButton* customizeMapButton;
    QPushButton* quitButton;

signals:
    void enterCustomizeMode();
};




//  初始界面点击start后的选择模式界面
//  有4个buttons：SOLO， PVP， PVE, Return to Menu
class ChooseModeWindow : public QWidget
{
    Q_OBJECT
public:
    ChooseModeWindow(QWidget *parent = nullptr);
    ~ChooseModeWindow() {}

private:
    QPushButton* singlePlayerButton;
    QPushButton* twoPlayerButton;
    QPushButton* fightComputerButton;
    QPushButton* return2MenuButton;
};




//  游戏暂停界面
//  有5个buttons：Resume, Restart, Save Game, Customize Map, Return to Menu
class PauseWindow : public QWidget
{
    Q_OBJECT
public:
    PauseWindow(QWidget *parent = nullptr);
    ~PauseWindow()  {}
    void setCustomizeEntryMode();
    void return2Menu();

private:
    QPushButton* resumeButton;
    QPushButton* restartButton;
    QPushButton* saveGameButton;
    QPushButton* customizeMapButton;
    QPushButton* return2MenuButton;
signals:
    void enterCustomizeMode();
    void pauseReturn2Menu();
};




//  自定义地图界面
//  有1个button：Return to Menu
class CustomizeMapWindow : public QWidget
{
    Q_OBJECT
public:
    CustomizeMapWindow(QWidget *parent = nullptr);
    ~CustomizeMapWindow() {}
    void endEditing();
    DISPLAYMODE entryMode;
protected:
    void paintEvent(QPaintEvent* ) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void paint(QPainter &painter);
    void changeBlock(POINT &p);
    void changeFood(POINT &p);

};





//  游戏中界面
class PlayWindow : public QWidget
{
    Q_OBJECT
public:
    PlayWindow(QWidget *parent = nullptr);
    ~PlayWindow() {}

    static int fps;

    void setPlayMode(PLAYMODE m);
    void keyPressed(QKeyEvent *event);
    void setTitle(int sec);
    void setReady();
    void restartGame();
    void resetMap();
    void paint(QPainter &painter);

    void changeBlock(POINT &p);
    void changeFood(POINT &p);

    void saveGame();
    void loadGame();

protected:
//    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent* ) override;

private:
    Controller* controller;

    PLAYMODE playMode;
signals:
    void setMainTitle(QString s);

};

//游戏结算界面
//  有1个button：Return to Menu
class EndGameWindow : public QWidget
{
    Q_OBJECT
public:
    EndGameWindow(QWidget *parent = nullptr);
    ~EndGameWindow() {}
    int loser = 0;
    PLAYMODE playMode = SOLO;
private:
    QPushButton* return2MenuButton;

protected:
    void paintEvent(QPaintEvent* ) override;
};



//  主界面
//  有5个子界面：startWindow, chooseModeWindow, pauseWindow, customizeMapWindow, playWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}


    //  press button后切换显示界面，用connect函数调用
    void showStartWindow();
    void showChooseModeWindow();
    void showPauseWindow();
    void showCustomizeMapWindow();
    void showPlayWindow();
    void showEndGameWindow(int l, PLAYMODE mode);

    void paint(QPainter &painter);

    void changeBlock(POINT &p);
    void changeFood(POINT &p);

    void restartGame();
    void sendQuitGame();
    void pauseEnterCustomize();
    void startEnterCustomize();
    void pauseReturn2Menu();

    void setSOLO();
    void setPVE();
    void setPVP();

    void saveGame();
    void loadGame();

    static int width;
    static int height;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    DISPLAYMODE displayMode;
    StartWindow* startWindow;
    ChooseModeWindow* chooseModeWindow;
    PauseWindow* pauseWindow;
    CustomizeMapWindow* customizeMapWindow;
    EndGameWindow* endGameWindow;
    PlayWindow* playWindow;

signals:
    void quitGame();

};
#endif // MAINWINDOW_H
