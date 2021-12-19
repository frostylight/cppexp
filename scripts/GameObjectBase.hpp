#pragma once

#include "stdpre.hpp"


namespace ObjectBase {
    //所有物体的基类
    class object {
      protected:
        //物体中心坐标
        REAL _x, _y;

        object(const REAL &x, const REAL &y);
        virtual ~object();

      public:
        //数据更新
        virtual void update() = 0;
        //绘图
        virtual void draw() = 0;
        //是否在地图内
        virtual bool inMap(const REAL &width, const REAL &height) const;

        REAL getx() const;
        REAL gety() const;
    };

    //除角色以外的物体
    //虚继承以与其他类共享基类变量
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
        friend class chara;

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

    //人物
    class chara: virtual public object {
      protected:
        //生命值
        int _health;

        chara(const int &health);

      public:
        //被子弹命中
        void hurt(const bullet *const bt);
        //是否存活
        bool alive() const;
    };

    //玩家
    class player: public chara {
      protected:
        //移动速度
        REAL _speed;

        player(const int &health, const REAL &speed);

      public:
        //复活重生
        virtual void respwan() = 0;
    };

    //敌人
    class enemy: public chara {
      protected:
        using chara::chara;
    };

} // namespace ObjectBase