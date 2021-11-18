#include<random>
#include<ctime>

#include"game.hpp"


template<typename T1, typename T2>
inline auto min(const T1 &x, const T2 &y){ return x > y ? y : x; }

template<typename T1, typename T2>
inline auto max(const T1 &x, const T2 &y){ return x > y ? x : y; }

template<typename T>
inline int ei(const T &x){
    if (!x)return 0;
    return x > 0 ? 1 : -1;
}

template<typename T>
inline T fround(const T &x){
    if (abs(x) <= 1)return ei(x);
    else return x;
}


bool object::getState()const{
    return x >= 0 && x <= MapWidth && y >= 0 && y <= MapHeight;
}

playerBullet::playerBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cint &Damage):bullet(X, Y, dX, dY, Damage){};

void playerBullet::draw(){
    if (getState())
        spellcard->drawAround(x, y);
}

enemyBullet::enemyBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cint &Damage):bullet(X, Y, dX, dY, Damage){};

void enemyBullet::draw(){
    if (getState())
        bluebullet->drawAround(x, y);
}

player::player(cREAL &X, cREAL &Y, cint &Health, cREAL &Speed): chara(X, Y, Health), speed(Speed){
    srand(time(NULL));
}

void player::update(){
    if (ishold(VK_UP))
        y = max(y - speed, 0);
    if (ishold(VK_DOWN))
        y = min(y + speed, MapHeight);
    if (ishold(VK_LEFT))
        x = max(x - speed, 0);
    if (ishold(VK_RIGHT))
        x = min(x + speed, MapWidth);
}

void player::draw(){
    static bool lr = 0;
    static uint lt = 0;
    if (ishold(VK_LEFT) ^ ishold(VK_RIGHT)){
        if (ishold(VK_RIGHT) ^ lr){
            lr = !lr;
            lt = 0;
            reimu_lr.reset();
            reimu_t.reset();
        }
        if (++lt <= reimu_t.allt){
            reimu_t.drawAroundFlip(x, y, lr);
            reimu_idle.reset();
        }
        else{
            reimu_lr.drawAroundFlip(x, y, lr);
            reimu_t.reset();
        }
    }
    else{
        lt = 0;
        reimu_idle.drawAroundFlip(x, y, lr);
        reimu_lr.reset();
        reimu_t.reset();
    }
}

enemy::enemy(cREAL &X, cREAL &Y, cREAL &sX, cREAL &sY, cint &sTime, cint &Health):chara(X, Y, Health), sx(sX), sy(sY), lx(X), ly(Y), stayTime(sTime){
    rx = (rand() % 51 - 25) * 0.025;
    ry = (rand() % 25 - 25) * 0.01;
}

void enemy::update(){
    if (!getState())return;
    static bool f = 0;
    if (!f){
        x += fround((sx - x) * 0.025);
        y += fround((sy - y) * 0.025);
        if (abs(sx - x) < 20 && abs(sy - y) < 24)f = 1;
    }
    else{
        if (!stayTime){
            x += fround((lx - x) * 0.025);
            y += fround((ly - y) * 0.025);
            if (abs(lx - x) < 10 && abs(ly - y) < 12)disable();
        }
        else{
            --stayTime;
            x += rx;
            y += ry;
        }
    }
}

void enemy::draw(){
    if (!getState())return;
    enemyblue.drawAround(x, y);
}