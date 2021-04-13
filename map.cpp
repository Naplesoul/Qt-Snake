#include "map.h"

// 坐标系原点在中间
Map::Map(QObject* parent, int h, int v) : h_size(h), v_size(v)
{
    setParent(parent);


    //生成围墙
    for(int i = 0; i <= h; ++i)
    {
        blocks.push_back(POINT(i, v));
        blocks.push_back(POINT(i, 0));
    }
    for(int i = 1; i < v; ++i)
    {
        blocks.push_back(POINT(0, i));
        blocks.push_back(POINT(h, i));
    }

    //随机生成一些障碍
    for(int i = 0; i < 6; ++i)
    {
        POINT random((rand()%(h_size - 1)) + 1, (rand()%(v_size - 1)) + 1);
        if(!blocks.contains(random))
            blocks.push_back(random);
    }
}

bool Map::insideBlock(POINT p)
{
    if(blocks.contains(p))
        return true;
    return false;
}
