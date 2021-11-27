#pragma once

#include "pre.hpp"


namespace GAME_BASE {
    class object {
        friend bool iscol(const object *const, const object *const);

      protected:
        REAL x, y;
        uint w, h;

      public:
        object(cREAL &X, cREAL &Y);

        REAL getx() const;
        REAL gety() const;
        void setxy(cREAL &X, cREAL &Y);
        uint getw() const;
        uint geth() const;

        bool iscol(const object *const a) const;

        virtual bool update()     = 0;
        virtual void draw() const = 0;
    };

    class item: public object {
      protected:
        REAL dx, dy;

      public:
        item(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY);
    };

    class bullet: public item {
      protected:
        uint damage;

      public:
        bullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cuit &Damage);

        virtual bool update() override;
    };

    class chara: public object {
      protected:
        int health;

      public:
        chara(cREAL &X, cREAL &Y, cint &Health);

        void hurt(cuit &damage);
        bool dead() const;
        int gethealth() const;
        void sethealth(cint &Health);
    };

    bool iscol(const object *const a, const object *const b);
} // namespace GAME_BASE
