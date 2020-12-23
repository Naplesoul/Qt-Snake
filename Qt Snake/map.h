#ifndef MAP_H
#define MAP_H

#include <QMainWindow>
#include <QObject>
#include <QVector>
#include <time.h>
#include <stdlib.h>
#include "initialize.h"

class Map : public QObject
{
    Q_OBJECT
public:
    Map(QObject* parent = nullptr, int h = 30, int v = 30);
//  用于判断该点是不是墙
    bool insideBlock(POINT p);
    void addBlock(POINT P);
    void deleteBlock(POINT P);
private:
//  有长宽信息和障碍物位置
    int h_size;
    int v_size;
    QVector<POINT> blocks;
    friend class Foods;
    friend class Snake;
    friend class Controller;
};

#endif // MAP_H
