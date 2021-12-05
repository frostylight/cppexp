#pragma once

#include "pre.hpp"


namespace Base {
    class object {
        friend bool collide(const object *const obj1, const object *const obj2);

      protected:
        //位置坐标
        REAL _x, _y;
        bool _state;

        //获取碰撞箱宽度
        virtual uint getw() const = 0;
        //获取碰撞箱高度
        virtual uint geth() const = 0;

      public:
        object(cREAL &x, cREAL &y);

        REAL getx() const;
        REAL gety() const;
        void setx(cREAL &x);
        void sety(cREAL &y);
        void setxy(cREAL &x, cREAL &y);

        // object是否失效
        virtual bool active() const;
        //使object失效
        void disable();

        //碰撞箱是否在画面内
        bool inscreenB(cREAL &width, cREAL &height) const;
        //是否在画面内
        bool inscreen(cREAL &width, cREAL &height) const;

        //数据更新
        virtual void update() = 0;
        //绘制object
        virtual void draw() = 0;

        //碰撞检测
        bool collide(const object *const obj) const;
    };

    //碰撞检测
    bool collide(const object *const obj1, const object *const obj2);

    class chara: public object {
      protected:
        //生命
        int _health;
        //发射弹幕CD计数
        uint _cd;

        //弹幕CD
        virtual uint getbulletCD() const = 0;

      public:
        chara(cREAL &x, cREAL &y, cint &health);

        int gethealth() const;
        void sethealth(cint &health);
        void hurt(cuint &damage);
        void heal(cuint &health);
        //是否死亡
        bool dead() const;

        //能否发射弹幕
        virtual bool bulletReady() const;

        virtual bool active() const override;

        //更新弹幕CD计数
        virtual void update() override;
    };

    class player: public chara {
        friend class playerbullet;

      protected:
        //移速 (像素每帧)
        REAL _speed;

      public:
        player(cREAL &x, cREAL &y, cint &health, cREAL &speed);

        REAL getspeed() const;
        void setspeed(cREAL &speed);
    };

    class enemy: public chara {
        friend class enemybullet;

      public:
        enemy(cREAL &x, cREAL &y, cint &health);
    };

    class drop: public object {
      protected:
        //每帧坐标移动
        REAL _dx, _dy;

      public:
        drop(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy);

        //更新坐标
        virtual void update() override;
    };

    class bullet: public drop {
      protected:
        //弹幕伤害
        uint _damage;

      public:
        bullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage);
    };

    class playerbullet: public bullet {
      public:
        playerbullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage);
        playerbullet(const player *const p, cREAL &dx, cREAL &dy, cuint &damage);
    };

    class enemybullet: public bullet {
      public:
        enemybullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage);
        enemybullet(const enemy *const e, cREAL &dx, cREAL &dy, cuint &damage);
    };

} // namespace Base