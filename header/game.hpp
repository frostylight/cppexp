#pragma once

#include"object.hpp"
#include"resource.hpp"


constexpr int WinWidth = 640, WinHeight = 480;
constexpr int MapWidth = 400, MapHeight = 480;
constexpr int playerHealth = 3;
constexpr REAL playerSpeed = 4.5;

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
    player(cREAL &X, cREAL &Y, cint &Health = playerHealth, cREAL &Speed = playerSpeed);

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