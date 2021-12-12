#pragma once
#include <cmath>

#include "stdpre.hpp"


namespace Base {
    //所有物体的基类
    class object {
      protected:
        //物体中心坐标
        REAL _x, _y;

      public:
        object(const REAL &x, const REAL &y);

        //数据更新
        virtual void update() = 0;
        //绘图
        virtual void draw() = 0;
    };

    //人物
    class chara: virtual public object {
      protected:
        //生命值
        int _health;

      public:
        chara(const int &health);
    };

    //玩家
    class player: public chara {
      public:
        using chara::chara;
    };

    //敌人
    class enemy: public chara {
      public:
        using chara::chara;
    };

    //物体
    class item: virtual public object {
      protected:
        //每帧位移
        REAL _dx, _dy;

      public:
        item(const REAL &dx, const REAL &dy);

        virtual void update() override;
    };

    //子弹
    class bullet: public item {
      protected:
        //命中伤害
        uint _damage;

      public:
        bullet(const REAL &dx, const REAL &dy, const uint &damage);
    };

    //玩家子弹
    class playerbullet: public bullet {
      public:
        using bullet::bullet;
    };

    //敌人子弹
    class enemybullet: public bullet {
      public:
        using bullet::bullet;
    };

} // namespace Base