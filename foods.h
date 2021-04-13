#ifndef FOODS_H
#define FOODS_H

#include <QMainWindow>
#include <QObject>
#include <QList>
#include <time.h>
#include <stdlib.h>
#include "initialize.h"
#include "map.h"

class Foods: public QObject
{
    Q_OBJECT
public:
    Foods(QObject* parent = nullptr, Map* m = nullptr);
    void randomGenerate();

//  在foodsEaten里添加这个点上的食物
    void eaten(POINT p, QList<FOOD> &foodsEaten);
//  用于告诉AI蛇他的目标
    POINT getFoodLocation();
private:

//  需要知道地图大小（生成范围）和该点是否有障碍
    int h_size;
    int v_size;
    Map* map;
    QList<FOOD> locations;
    friend class Controller;
};

#endif // FOODS_H
