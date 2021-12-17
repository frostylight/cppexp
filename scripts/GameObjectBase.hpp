#pragma once

#include "stdpre.hpp"


namespace ObjectBase {
    //所有物体的基类
    class object {
      protected:
        //物体中心坐标
        REAL _x, _y;

        object(const REAL &x, const REAL &y);

      public:
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

        chara(const int &health);
    };

    //玩家
    class player: public chara {
      protected:
        REAL _speed;

        player(const int &health, const REAL &speed);
    };

    //敌人
    class enemy: public chara {
      protected:
        using chara::chara;
    };

    //物体
    class item: virtual public object {
      protected:
        //每帧位移
        REAL _dx, _dy;

        item(const REAL &dx, const REAL &dy);

      public:
        virtual void update() override;
    };

    //子弹
    class bullet: public item {
      protected:
        //命中伤害
        uint _damage;

        bullet(const REAL &dx, const REAL &dy, const uint &damage);
    };

    //玩家子弹
    class playerbullet: public bullet {
      protected:
        using bullet::bullet;
    };

    //敌人子弹
    class enemybullet: public bullet {
      protected:
        using bullet::bullet;
    };

} // namespace ObjectBase