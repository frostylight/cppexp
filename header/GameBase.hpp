#pragma once

#include "pre.hpp"


namespace Base {
    class object {
        friend bool collide(const object *const obj1, const object *const obj2);

      protected:
        REAL _x, _y;
        bool _state;

      public:
        object(cREAL &x, cREAL &y);

        REAL getx() const;
        REAL gety() const;
        void setx(cREAL &x);
        void sety(cREAL &y);
        void setxy(cREAL &x, cREAL &y);
        //获取碰撞箱宽度
        virtual uint getw() const = 0;
        //获取碰撞箱高度
        virtual uint geth() const = 0;

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
        virtual void draw() const = 0;

        //碰撞检测
        bool collide(const object *const obj) const;
    };

    bool collide(const object *const obj1, const object *const obj2);

    class chara: public object {
      protected:
        int _health;

      public:
        chara(cREAL &x, cREAL &y, cint &health);

        int gethealth() const;
        void sethealth(cint &health);
        void hurt(cuint &damage);
        void heal(cuint &health);
        bool dead() const;

        virtual bool active() const override;
    };

    class player: public chara {
        friend class playerbullet;

      protected:
        REAL _speed;

      public:
        player(cREAL &x, cREAL &y, cint &health, cREAL &speed);

        REAL getspeed() const;
        void setspeed(cREAL &speed);
    };

    class enemy: public chara {
        friend class enemybullet;

      protected:
        uint _cd;
        virtual uint getbulletCD() const = 0;

      public:
        enemy(cREAL &x, cREAL &y, cint &health);

        virtual bool bulletReady() const;

        virtual void update();
    };

    class drop: public object {
      protected:
        REAL _dx, _dy;

      public:
        drop(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy);

        virtual void update() override;
    };

    class bullet: public drop {
      protected:
        uint _damage;

      public:
        bullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage);
    };

    class playerbullet: public bullet {
      public:
        playerbullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage);
        playerbullet(const player &p, cREAL &dx, cREAL &dy, cuint &damage);
    };

    class enemybullet: public bullet {
      public:
        enemybullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage);
        enemybullet(const enemy &e, cREAL &dx, cREAL &dy, cuint &damage);
    };
} // namespace Base