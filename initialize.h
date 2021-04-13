#ifndef INITIALIZE_H
#define INITIALIZE_H
#include <QDebug>


//  定义了一些枚举变量，含义如其名
enum DIRECTION
{
    UP, DOWN, RIGHT, LEFT
};

struct POINT
{
    int x;
    int y;
    POINT(int X = 0, int Y = 0):x(X),y(Y){}
    bool operator==(const POINT &p) const
    {
        if(x == p.x && y == p.y)
            return true;
        return false;
    }
};

enum FOODTYPE
{
    ADDLENGTH, ADDLIFE, ACCELERATE, DECELERATE, FROZEN
};

struct FOOD
{
    POINT location;
    FOODTYPE type;
    int pressedTimes;
    FOOD(POINT const &p, FOODTYPE t):location(p), type(t), pressedTimes(int(t)) {}
    bool operator==(const FOOD &f) const
    {
        if(location == f.location && type == f.type)
            return true;
        return false;
    }
};

enum DISPLAYMODE
{
    START, CHOOSE, PAUSE, CUSTOMIZE, PLAY, GAMEOVER
};

enum PLAYMODE
{
    SOLO, PVP, PVE
};

enum GAMESTATUS
{
    GAMING, PAUSED, READY
};

struct LINE
{
    POINT start;
    POINT end;
    LINE() {}
    LINE(POINT s, POINT e):start(s), end(e) {}
};

#endif // INITIALIZE_H
