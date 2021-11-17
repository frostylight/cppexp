#pragma once

#include"pre.hpp"

class object{
protected:
    REAL x, y;

public:
    object(cREAL &X, cREAL &Y);
    virtual REAL getX()const;
    virtual REAL getY()const;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void disable();
    virtual bool getState()const;
};

class item :public object{
protected:
    REAL dx, dy;

public:
    item(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY);
};

class bullet :public item{
protected:
    int damage;

public:
    bullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cint &Damage = 1);
    virtual void update() override;
};

class chara :public object{
protected:
    int health;

public:
    chara(cREAL &X, cREAL &Y, cint &Health);
    virtual void setHealth(cint &Health);
    virtual int getHealth()const;
    virtual bool getState()const override;
};
