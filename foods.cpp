#include "foods.h"

// 坐标系原点在中间
Foods::Foods(QObject* parent, Map* m) : h_size(m->h_size), v_size(m->v_size), map(m)
{
    setParent(parent);
    for(int i = 0; i < 8; ++i)
        randomGenerate();
}

void Foods::eaten(POINT p, QList<FOOD> &foodsEaten)
{
    for(auto it = locations.begin(); it != locations.end(); ++it)
    {
        if(p == (*it).location)
        {
            foodsEaten.push_back(*it);
            return;
        }
    }
}

void Foods::randomGenerate()
{
    srand((unsigned)time(NULL));

    bool succeed = false;
    while(!succeed)
    {
        //随机生成一个点
        POINT random((rand()%(map->h_size - 1)) + 1, (rand()%(map->v_size - 1)) + 1);
        //这个点是墙
        if(map->blocks.contains(random))
            continue;
        //这个点已经有food了
        succeed = true;
        for(auto it = locations.begin(); it != locations.end(); ++it)
        {
            if((*it).location.x == random.x && (*it).location.y == random.y)
            {
                succeed = false;
                break;
            }
        }
        if(succeed)
        {
            //生成正常食物的几率为67%，特殊食物为33%
            if(rand() % 3 == 0)
                locations.push_back(FOOD(random, FOODTYPE(rand() % 4 + 1)));
            else
                locations.push_back(FOOD(random, ADDLENGTH));
        }
    }
}

POINT Foods::getFoodLocation()
{
    return locations.back().location;
}
