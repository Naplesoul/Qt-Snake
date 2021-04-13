#include "controller.h"

GAMESTATUS Controller::status = PAUSED;
PLAYMODE Controller::playMode = SOLO;

Controller::Controller(PLAYMODE m, QWidget *parent, int fps)
{
    setParent(parent);
    playMode = m;
    map = new Map(this);

    foods = new Foods(this, map);

    snake1 = new Snake(this, map);
    snake2 = new Snake(this, map, snake1);

    updateInterval = 1000/(fps);
    wait2StartTimer = new QTimer(this);
    connect(wait2StartTimer, &QTimer::timeout, this, &Controller::wait2Start);

    updateTimer = new QTimer(this);
    updateTimer->start(updateInterval);
    connect(updateTimer, &QTimer::timeout, this, &Controller::update);

    snake1Time = new QTimer(this);
    snake2Time = new QTimer(this);
    connect(snake1Time, &QTimer::timeout, this, &Controller::snake1Resume);
    connect(snake2Time, &QTimer::timeout, this, &Controller::snake2Resume);

    connect(snake1, &Snake::death, this, &Controller::snake1Die);
    connect(snake2, &Snake::death, this, &Controller::snake2Die);

    connect(snake1, &Snake::setReady, this, &Controller::setReady);
    connect(snake2, &Snake::setReady, this, &Controller::setReady);

    connect(wait2StartTimer, &QTimer::timeout, this, &Controller::wait2Start);

    blockImage = new QImage("brick.png");
    addImage = new QImage("addlife.png");
    accImage = new QImage("acc.png");
    decImage = new QImage("dec.png");
    foodImage = new QImage("food.png");
    frozImage = new QImage("froz.png");

    qDebug() << "Controller constructed";
}

//  准备开始
void Controller::setReady()
{
    status = READY;
    seconds2Wait = 3;
    wait2StartTimer->start(1000);
    emit(remainSeconds(seconds2Wait));
}

void Controller::restartGame()
{
    //  直接删除原来的蛇，并生成新蛇，接着重新connect
    delete snake1;
    delete snake2;
    snake1 = new Snake(this, map);
    snake2 = new Snake(this, map, snake1);
    connect(snake1, &Snake::death, this, &Controller::snake1Die);
    connect(snake2, &Snake::death, this, &Controller::snake2Die);

    connect(snake1, &Snake::setReady, this, &Controller::setReady);
    connect(snake2, &Snake::setReady, this, &Controller::setReady);

}

void Controller::resetMap()
{
    //  重置场景（其实应该叫resetScene比较好）
    delete map;
    delete foods;
    map = new Map(this);
    foods = new Foods(this, map);
    restartGame();
}

void Controller::wait2Start()
{
    //  倒计时每过一秒触发
    if(status == PAUSED)
        return;
    --seconds2Wait;
    if(seconds2Wait == 0)
    {
        status = GAMING;
    }
    else
    {
        wait2StartTimer->start(1000);
    }
    emit(remainSeconds(seconds2Wait));
}

void Controller::snake1Accelerate()
{
    snake1Spd = 1;
    snake1Time->start(5000);
}

void Controller::snake2Accelerate()
{
    snake2Spd = 1;
    snake2Time->start(5000);
}

void Controller::snake1Decelerate()
{
    snake1Spd = 4;
    snake1Time->start(5000);
}
void Controller::snake2Decelerate()
{
    snake2Spd = 4;
    snake2Time->start(5000);
}
void Controller::snake1Frozen()
{
    snake1Froz = true;
    snake1Time->start(3000);
}
void Controller::snake2Frozen()
{
    snake2Froz = true;
    snake2Time->start(3000);
}
void Controller::snake1Resume()
{
    snake1Froz = false;
    snake1Spd = 2;
}
void Controller::snake2Resume()
{
    snake2Froz = false;
    snake2Spd = 2;
}


void Controller::snake1Die()
{
    status = PAUSED;
    emit(gameOver(1, playMode));
}

void Controller::snake2Die()
{
    status = PAUSED;
    emit(gameOver(2, playMode));
}

void Controller::keyPressed(QKeyEvent *k)
{
    //  esc和空格键暂停
    if(k->key() == Qt::Key_Space || k->key() == Qt::Key_Escape)
    {
        Controller::status = PAUSED;
        emit(paused());
        return;
    }

    // 改变targetDirection，同时要防止非法改方向，比如原来是向上，不能改成向下
    switch(k->key())
    {
    case Qt::Key_W:
        if(snake1->direction != DOWN)
            snake1->tarDir = UP;
        break;
    case Qt::Key_A:
        if(snake1->direction != RIGHT)
            snake1->tarDir = LEFT;
        break;
    case Qt::Key_S:
        if(snake1->direction != UP)
            snake1->tarDir = DOWN;
        break;
    case Qt::Key_D:
        if(snake1->direction != LEFT)
            snake1->tarDir = RIGHT;
        break;
    }

    //  上下左右键改另一条蛇
    if(Controller::playMode == PVP)
    {
        switch(k->key())
        {
        case Qt::Key_Up:
            if(snake2->direction != DOWN)
                snake2->tarDir = UP;
            break;
        case Qt::Key_Left:
            if(snake2->direction != RIGHT)
                snake2->tarDir = LEFT;
            break;
        case Qt::Key_Down:
            if(snake2->direction != UP)
                snake2->tarDir = DOWN;
            break;
        case Qt::Key_Right:
            if(snake2->direction != LEFT)
                snake2->tarDir = RIGHT;
            break;
        }
    }

    //  单人模式上下左右改自己
    if(Controller::playMode == SOLO)
    {
        switch(k->key())
        {
        case Qt::Key_Up:
            if(snake1->direction != DOWN)
                snake1->tarDir = UP;
            break;
        case Qt::Key_Left:
            if(snake1->direction != RIGHT)
                snake1->tarDir = LEFT;
            break;
        case Qt::Key_Down:
            if(snake1->direction != UP)
                snake1->tarDir = DOWN;
            break;
        case Qt::Key_Right:
            if(snake1->direction != LEFT)
                snake1->tarDir = RIGHT;
            break;
        }
    }
}


void Controller::update()
{
    //  如果暂停或者预备开始，则不更新
    if(status != GAMING)
    {
        updateTimer->start(updateInterval);
        return;
    }

    //  snake1能动（不冻住且当前帧是spd倍数，应该刷新这条蛇）
    if(!snake1Froz && curFrame % snake1Spd == 0)
    {
        QList<FOOD> eaten1;

        snake1->move();

        //  判断是否撞墙
        if(map->insideBlock((snake1->body).front()))
        {
            snake1->die();
            return;
        }
        //  判断是否撞自己
        if(snake1->insideBody((snake1->body).front()))
        {
            snake1->die();
            return;
        }

        //  处理吃掉的食物
        foods->eaten((snake1->body).front(), eaten1);

        for(auto it = eaten1.begin(); it != eaten1.end(); ++it)
        {
            switch((*it).type)
            {
            case ADDLENGTH:
                snake1->addLength();
                break;
            case ADDLIFE:
                snake1->addLife();
                break;
            case ACCELERATE:
                snake1Accelerate();
                break;
            case DECELERATE:
                snake1Decelerate();
                break;
            case FROZEN:
                snake1Frozen();
                break;
            }

            foods->locations.removeOne(*it);
            foods->randomGenerate();
        }
    }

    //  如果有两条蛇且snake2没冻住且前帧是spd倍数，应该刷新这条蛇
    if(playMode != SOLO && !snake2Froz && curFrame % snake2Spd == 0)
    {
        QList<FOOD> eaten2;

        if(playMode == PVP)
            snake2->move();

        if(playMode == PVE)
            snake2->autoMove(foods->getFoodLocation());

        // 判断是否撞墙
        if(map->insideBlock((snake2->body).front()))
        {
            snake2->die();
            ((QWidget*)parent())->update();
            return;
        }

        //  判断是否撞自己
        if(snake2->insideBody((snake2->body).front()))
        {
            snake2->die();
            ((QWidget*)parent())->update();
            return;
        }

        //  判断是否撞snake1
        if(snake1->inside((snake2->body).front()))
        {
            snake2->die();
            ((QWidget*)parent())->update();
            return;
        }

        // 处理吃掉的食物
        foods->eaten((snake2->body).front(), eaten2);

        for(auto it = eaten2.begin(); it != eaten2.end(); ++it)
        {
            switch((*it).type)
            {
            case ADDLENGTH:
                snake2->addLength();
                break;
            case ADDLIFE:
                snake2->addLife();
                break;
            case ACCELERATE:
                snake2Accelerate();
                break;
            case DECELERATE:
                snake2Decelerate();
                break;
            case FROZEN:
                snake2Frozen();
                break;
            }

            foods->locations.removeOne(*it);
            foods->randomGenerate();
        }
    }

    //  再处理snake1有没有撞到snake2
    if(playMode != SOLO && curFrame % snake1Spd == 0)
    {
        if(snake2->inside((snake1->body).front()))
        {
            snake1->die();
            ((QWidget*)parent())->update();
            return;
        }
    }

    //  设置下一次刷新的倒计时
    updateTimer->start(updateInterval);
    ++curFrame;
    //  使playWindow刷新（调用paint())
    ((QWidget*)parent())->update();
}


void Controller::paint(QPainter& painter)
{
    //  背景底色
    painter.fillRect(0,0,1240,1240,Qt::white);
    painter.translate(20,20);
    QPen sn1;
    QPen sn2;
    sn1.setColor(Qt::blue);
    sn2.setColor(Qt::red);
    sn1.setWidth(40);
    sn2.setWidth(40);
//    sn1.setJoinStyle(Qt::RoundJoin);

    //  画障碍物
    for(auto it = (map->blocks).begin(); it != (map->blocks).end(); ++it)
    {
        painter.drawImage((*it).x * 40 - 20, (*it).y * 40 - 20, *blockImage);
    }

    //  画食物
    for(auto it = (foods->locations).begin(); it != (foods->locations).end(); ++it)
    {
        switch((*it).type)
        {
        case ADDLENGTH:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *foodImage);
            break;
        case ADDLIFE:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *addImage);
            break;
        case ACCELERATE:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *accImage);
            break;
        case DECELERATE:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *decImage);
            break;
        case FROZEN:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *frozImage);
            break;
        }
//        foodPen.setColor(foodColor);
//        painter.setPen(foodPen);
//        painter.drawPoint((*it).location.x * 40, (*it).location.y * 40);
    }

    //  画snake1
    painter.setPen(sn1);
    for(auto it = (snake1->body).begin(); it != (snake1->body).end(); ++it)
    {
        painter.drawPoint((*it).x * 40, (*it).y * 40);
    }

    //  画snake2
    if(playMode != SOLO)
    {
        painter.setPen(sn2);
        for(auto it = (snake2->body).begin(); it != (snake2->body).end(); ++it)
        {
            painter.drawPoint((*it).x * 40, (*it).y * 40);
        }
    }
}

void Controller::paintScene(QPainter& painter)
{
    //  只花场景不画蛇，逻辑同paint()函数
    painter.fillRect(0,0,1240,1240,Qt::white);
    painter.translate(20,20);
    for(auto it = (map->blocks).begin(); it != (map->blocks).end(); ++it)
    {
        painter.drawImage((*it).x * 40 - 20, (*it).y * 40 - 20, *blockImage);
    }

    for(auto it = (foods->locations).begin(); it != (foods->locations).end(); ++it)
    {
        switch((*it).type)
        {
        case ADDLENGTH:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *foodImage);
            break;
        case ADDLIFE:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *addImage);
            break;
        case ACCELERATE:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *accImage);
            break;
        case DECELERATE:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *decImage);
            break;
        case FROZEN:
            painter.drawImage((*it).location.x * 40 - 20, (*it).location.y * 40 - 20, *frozImage);
            break;
        }
    }
}

void Controller::changeBlock(POINT &p)
{
    //  这个点有block就删了，没有就加
    if(p.x == 0 || p.x == map->h_size || p.y == 0 || p.y == map->v_size)
        return;
    if((map->blocks).contains(p))
        (map->blocks).removeOne(p);
    else
        (map->blocks).push_back(p);
}
void Controller::changeFood(POINT &p)
{
    for(auto it = (foods->locations).begin(); it != (foods->locations).end(); ++it)
    {
        if((*it).location == p)
        {
            //  右击一次食物让它改变到下一种类型，最后一个类型载点一次移除食物
            ++((*it).pressedTimes);
            (*it).pressedTimes = (*it).pressedTimes % 6;
            if((*it).pressedTimes == 5)
                (foods->locations).removeOne(*it);
            else
            {
                (*it).type = FOODTYPE(int((*it).type) + 1);
            }
            return;
        }
    }
    (foods->locations).push_back(FOOD(p, ADDLENGTH));
}

void Controller::saveGame()
{
    using namespace std;
    ofstream gameSave("gameSave.txt");

    //  将地图、食物、蛇的状态全存下来
    gameSave << int(playMode) << endl;

    gameSave << int(snake1->direction) << endl;
    gameSave << int(snake1->tarDir) << endl;
    gameSave << snake1->lives << endl;
    gameSave << (snake1->body).length() << endl;
    for(auto it = (snake1->body).begin(); it != (snake1->body).end(); ++it)
    {
        gameSave << (*it).x << ' ' << (*it).y << endl;
    }

    gameSave << int(snake2->direction) << endl;
    gameSave << int(snake2->tarDir) << endl;
    gameSave << snake2->lives << endl;
    gameSave << (snake2->body).length() << endl;
    for(auto it = (snake2->body).begin(); it != (snake2->body).end(); ++it)
    {
        gameSave << (*it).x << ' ' << (*it).y << endl;
    }

    gameSave << (map->blocks).length() << endl;
    for(auto it = (map->blocks).begin(); it != (map->blocks).end(); ++it)
    {
        gameSave << (*it).x << ' ' << (*it).y << endl;
    }

    gameSave << (foods->locations).length() << endl;
    for(auto it = (foods->locations).begin(); it != (foods->locations).end(); ++it)
    {
        gameSave << (*it).location.x << ' ' << (*it).location.y << ' ' << int((*it).type) << endl;
    }

    gameSave.close();
}

void Controller::loadGame()
{
    using namespace std;
    ifstream gameSave("gameSave.txt");

    //  恢复蛇的状态
    int input;
    gameSave >> input;
    playMode = PLAYMODE(input);

    gameSave >> input;
    snake1->direction = DIRECTION(input);
    gameSave >> input;
    snake1->tarDir = DIRECTION(input);
    gameSave >> input;
    snake1->lives = input;
    gameSave >> input;
    (snake1->body).clear();
    int x;
    int y;
    for(int i = 0; i < input; ++i)
    {
        gameSave >> x >> y;
        (snake1->body).push_back(POINT(x, y));
    }

    gameSave >> input;
    snake2->direction = DIRECTION(input);
    gameSave >> input;
    snake2->tarDir = DIRECTION(input);
    gameSave >> input;
    snake2->lives = input;
    gameSave >> input;
    (snake2->body).clear();
    for(int i = 0; i < input; ++i)
    {
        gameSave >> x >> y;
        (snake2->body).push_back(POINT(x, y));
    }

    gameSave >> input;
    (map->blocks).clear();
    for(int i = 0; i < input; ++i)
    {
        gameSave >> x >> y;
        (map->blocks).push_back(POINT(x ,y));
    }

    gameSave >> input;
    (foods->locations).clear();
    int foodT;
    for(int i = 0; i < input; ++i)
    {
        gameSave >> x >> y >> foodT;
        (foods->locations).push_back(FOOD(POINT(x, y), FOODTYPE(foodT)));
    }

    gameSave.close();
}
