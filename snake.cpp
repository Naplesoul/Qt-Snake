#include "snake.h"

Snake::Snake(QObject* parent, Map* m, Snake* e): lives(1), map(m), enemy(e)
{
    setParent(parent);

    while(true)
    {
        POINT random((rand()%(map->h_size - 1)) + 1, (rand()%(map->v_size - 1)) + 1);

        //这个点是墙
        if(map->blocks.contains(random))
            continue;
        else
        {
            body.push_back(random);
            break;
        }
    }
    direction = DIRECTION(rand()%4);
    tarDir = direction;
    qDebug() << "snake constructed at " << body.front().x << body.front().y << Qt::endl;
}


// 移动一格
void Snake::move()
{
    //先处理头部向前的移动
    switch(tarDir)
    {
    case UP:
        body.prepend(POINT(body.front().x, body.front().y - 1));
        break;
    case DOWN:
        body.prepend(POINT(body.front().x, body.front().y + 1));
        break;
    case LEFT:
        body.prepend(POINT(body.front().x - 1, body.front().y));
        break;
    case RIGHT:
        body.prepend(POINT(body.front().x + 1, body.front().y));
        break;
    }
    direction = tarDir;
    qDebug() << "snake move to " << body.front().x << body.front().y << int(direction) << Qt::endl;

    //再处理尾部移动
    //若需要增加长度，则尾部不移动
    if(addLen)
    {
        addLen = false;
        return;
    }
    body.pop_back();
}


void Snake::autoMove(POINT target)
{
    //  对于每种情况自定义搜索方向
    QVector<DIRECTION> searchOrder;
    //  一种贪心算法，优先向食物方向移动
    if(target.x < body.front().x && target.y < body.front().y)
    {
        searchOrder.push_back(UP);
        searchOrder.push_back(LEFT);
        searchOrder.push_back(DOWN);
        searchOrder.push_back(RIGHT);
    }
    else if(target.x < body.front().x && target.y > body.front().y)
    {
        searchOrder.push_back(LEFT);
        searchOrder.push_back(DOWN);
        searchOrder.push_back(UP);
        searchOrder.push_back(RIGHT);
    }
    else if(target.x > body.front().x && target.y < body.front().y)
    {
        searchOrder.push_back(RIGHT);
        searchOrder.push_back(UP);
        searchOrder.push_back(DOWN);
        searchOrder.push_back(LEFT);
    }
    else if(target.x > body.front().x && target.y > body.front().y)
    {
        searchOrder.push_back(DOWN);
        searchOrder.push_back(RIGHT);
        searchOrder.push_back(UP);
        searchOrder.push_back(LEFT);
    }
    else if(target.x == body.front().x && target.y < body.front().y)
    {
        searchOrder.push_back(UP);
        searchOrder.push_back(RIGHT);
        searchOrder.push_back(LEFT);
        searchOrder.push_back(DOWN);
    }
    else if(target.x == body.front().x && target.y > body.front().y)
    {
        searchOrder.push_back(DOWN);
        searchOrder.push_back(RIGHT);
        searchOrder.push_back(LEFT);
        searchOrder.push_back(UP);
    }
    else if(target.x < body.front().x && target.y == body.front().y)
    {
        searchOrder.push_back(LEFT);
        searchOrder.push_back(UP);
        searchOrder.push_back(DOWN);
        searchOrder.push_back(RIGHT);
    }
    else if(target.x > body.front().x && target.y == body.front().y)
    {
        searchOrder.push_back(RIGHT);
        searchOrder.push_back(DOWN);
        searchOrder.push_back(UP);
        searchOrder.push_back(LEFT);
    }
    else
    {
        searchOrder.push_back(UP);
        searchOrder.push_back(LEFT);
        searchOrder.push_back(DOWN);
        searchOrder.push_back(RIGHT);
    }

    POINT up(body.front().x, body.front().y - 1);
    POINT down(body.front().x, body.front().y + 1);
    POINT left(body.front().x - 1, body.front().y);
    POINT right(body.front().x + 1, body.front().y);

    //  根据搜索顺序找路，要求这个方向的下一格没有障碍，也没有自己的身体，也没有对手的身体
    for(auto it = searchOrder.begin(); it != searchOrder.end(); ++it)
    {
        switch(*it)
        {
        case UP:
            if((direction != DOWN) && (!(map->blocks).contains(up)))
            {
                if(!(insideBody(up) || (enemy && enemy->inside(up))))
                {
                    tarDir = UP;
                    move();
                    return;
                }
            }
            break;

        case DOWN:
            if((direction != UP) && (!(map->blocks).contains(down)))
            {
                if(!(insideBody(down) || (enemy && enemy->inside(down))))
                {
                    tarDir = DOWN;
                    move();
                    return;
                }
            }
            break;
        case LEFT:
            if((direction != RIGHT) && (!(map->blocks).contains(left)))
            {
                if(!(insideBody(left) || (enemy && enemy->inside(left))))
                {
                    tarDir = LEFT;
                    move();
                    return;
                }
            }
            break;
        case RIGHT:
            if((direction != LEFT) && (!(map->blocks).contains(right)))
            {
                if(!(insideBody(right) || (enemy && enemy->inside(right))))
                {
                    tarDir = RIGHT;
                    move();
                    return;
                }
            }
            break;
        }
    }
    move();
    return;
}


void Snake::addLife()
{
    ++lives;
}

void Snake::addLength()
{
    addLen = true;
}

void Snake::die()
{
    --lives;
    if(lives == 0)
    {
        emit(death());
        return;
    }

    //进行复活
    emit(setReady());
    while(true)
    {
        //如果是最短长度，则随机出生在一个地方
        if(body.size() == 1)
        {
            while(true)
            {
                POINT random((rand()%(map->h_size - 1)) + 1, (rand()%(map->v_size - 1)) + 1);

                //这个点是墙
                if(map->blocks.contains(random))
                    continue;

                body.clear();
                body.push_back(random);
                break;
            }
            return;
        }

        //去掉头部一格，直至有路可走
        body.pop_front();

        //  找有路的方向
        POINT left(body.front().x - 1, body.front().y);
        if(!(map->blocks).contains(left))
        {
            if(!(insideBody(left) || (enemy && enemy->inside(left))))
            {
                tarDir = LEFT;
                return;
            }
        }

        POINT right(body.front().x + 1, body.front().y);
        if(!(map->blocks).contains(right))
        {
            if(!(insideBody(right) || (enemy && enemy->inside(right))))
            {
                tarDir = RIGHT;
                return;
            }
        }

        POINT up(body.front().x, body.front().y - 1);
        if(!(map->blocks).contains(up))
        {
            if(!(insideBody(up) || (enemy && enemy->inside(up))))
            {
                tarDir = UP;
                return;
            }
        }

        POINT down(body.front().x, body.front().y + 1);
        if(!(map->blocks).contains(down))
        {
            if(!(insideBody(down) || (enemy && enemy->inside(down))))
            {
                tarDir = DOWN;
                return;
            }
        }
    }

}



bool Snake::inside(POINT p)
{
    for(auto it = body.begin(); it != body.end(); ++it)
    {
        if(p == *it)
            return true;
    }
    return false;
}

bool Snake::insideBody(POINT p)
{
    for(auto it = body.begin() + 1; it != body.end(); ++it)
    {
        if(p == *it)
            return true;
    }
    return false;
}
