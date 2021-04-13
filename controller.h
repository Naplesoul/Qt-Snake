#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <fstream>
#include <istream>
#include <string>
#include "initialize.h"
#include "foods.h"
#include "map.h"
#include "snake.h"

// Controller类是Foods,Snake,Map类的友元，便于直接访问并控制元素
class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(PLAYMODE m = SOLO, QWidget* parent = nullptr, int fps = 60);
    static GAMESTATUS status;
    static PLAYMODE playMode;

//  event由mainwindow类接收并向下调用这个函数
    void keyPressed(QKeyEvent *k);

//  以确定的频率更新内容，包含游戏的主逻辑
    void update();
//  以下两个函数用于返回游戏时预留一定缓冲时间
    void setReady();
    void wait2Start();
//  以下两个函数用于重置游戏
    void restartGame();
    void resetMap();

//  playwindow的paintevent将会调用这个函数
    void paint(QPainter& painter);
//  pausewindow的paintevent将会调用这个函数
    void paintScene(QPainter& painter);

//  用于自定义地图
    void changeBlock(POINT &p);
    void changeFood(POINT &p);

//  以下函数用于控制游戏逻辑，功能如其名
    void snake1Die();
    void snake2Die();
    void snake1Accelerate();
    void snake2Accelerate();
    void snake1Decelerate();
    void snake2Decelerate();
    void snake1Frozen();
    void snake2Frozen();
    void snake1Resume();
    void snake2Resume();

//  用于实现加载和保存存档
    void saveGame();
    void loadGame();

signals:
//  告诉playwindow游戏结束，进入结算界面
    void gameOver(int loser, PLAYMODE mode);
//  告诉playwindow，让它修改titile实现倒计时开始
    void remainSeconds(int sec);
//  告诉playwindow游戏已暂停
    void paused();

private:
    Foods* foods;
    Map* map;
    Snake* snake1;
    Snake* snake2;
//  控制刷新
    QTimer* updateTimer;
    double updateInterval;
//  倒计时时间
    QTimer* wait2StartTimer;
    int seconds2Wait;

//  特殊效果存在倒计时
    QTimer* snake1Time;
    QTimer* snake2Time;

//  注意是数字越小越快，curFrame表示当前帧，当curFrame是speed的整数倍时更新这条蛇
    int snake1Spd = 2;
    int snake2Spd = 2;
    bool snake1Froz = false;
    bool snake2Froz = false;
    long curFrame = 0;

//  绘制的食物和障碍物图片
    QImage* accImage;
    QImage* addImage;
    QImage* decImage;
    QImage* foodImage;
    QImage* frozImage;
    QImage* blockImage;
};

#endif // CONTROLLER_H
