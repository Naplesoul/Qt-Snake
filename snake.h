#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QList>
#include <QVector>
#include <QTimer>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "initialize.h"
#include "map.h"

class Snake: public QObject
{
    Q_OBJECT
public:
//  e代表enemy，可以使AI蛇考虑对手的位置
    Snake(QObject* parent = nullptr, Map* m = nullptr, Snake* e = nullptr);
//  向前移动一格
    void move();
//  自动向前移动
    void autoMove(POINT target);
    void addLife();
    void addLength();
//  减一条命，包含重生逻辑
    void die();
//  判断这个点有没有撞到自己（包括自己的头部）
    bool inside(POINT p);
//  判断这个点有没有撞到自己（不包括自己的头部）
    bool insideBody(POINT p);
signals:
//  告诉controller自己彻底死了
    void death();
//  告诉controller自己没死透，准备倒计时继续
    void setReady();
private:
//  存自己身体位置
    QList<POINT> body;
//  存当前方位，要与tarDir分开保存，为了防止还没走一格连续两次按键改变方向导致蛇有奇异行为
    DIRECTION direction;
//  存下一步准备走的方向
    DIRECTION tarDir;
    int lives;
    bool addLen = false;
    Map* map;
    Snake* enemy;
    friend class Controller;
};

#endif // SNAKES_H
