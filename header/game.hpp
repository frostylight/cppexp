#pragma once

#include"object.hpp"
#include"resource.hpp"


const int playerHeight = 40, playerwidth = 24;
const int WinWidth = 640, WinHeight = 480;
const int MapWidth = 400, MapHeight = 480;
const int FrameLimit = 60;

class playerBullet :public bullet{
    public:
    playerBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cint &Damage = 1);
    void draw();
};

class enemyBullet :public bullet{
    public:
    enemyBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cint &Damage = 1);
    void draw();
};

class player :public chara{
    REAL speed;

    public:
    player(cREAL &X, cREAL &Y, cint &Health = 3, cREAL &Speed = 4.5);

    void update();
    void draw();
};

class enemy :public chara{
    REAL sx, sy, lx, ly;
    REAL rx, ry;
    int stayTime;

    public:
    enemy(cREAL &X, cREAL &Y, cREAL &sX, cREAL &sY, cint &sTime, cint &Health = 1);

    void update();
    void draw();
};