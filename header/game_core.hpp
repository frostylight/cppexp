#pragma once

#include "game_base.hpp"
#include "game_resource.hpp"

namespace GAME_DEFAULT_SETTING {
    constexpr uint WinWidth = 640, WinHeight = 480;
    constexpr uint halfWinWidth = 320, halfWinHeight = 240;
    constexpr uint MapWidth = 400, MapHeight = 480;
    constexpr uint halfMapWidth = 200, halfMapHeight = 240;
    constexpr uint enemyShotCD = 60, playerShotCD = 6;
} // namespace GAME_DEFAULT_SETTING

namespace GAME_CORE {
    using namespace GAME_BASE;

    class playerBullet final: public bullet {
      public:
        playerBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cuit &Damage = 1);

        bool update() override final;
        void draw() const override final;
    };

    class enemyBullet final: public bullet {
      public:
        enemyBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cuit &Damage = 1);

        bool update() override final;
        void draw() const override final;
    };

    class player final: public chara {
        REAL speed;

      public:
        player(cREAL &X, cREAL &Y, cint &Health = 3, cREAL &Speed = 4.5);

        void updateSize();
        bool update() override final;
        void draw() const override final;
    };

    class enemy final: public chara {
        REAL sx, sy;
        REAL ex, ey;
        uint stayTime;
        uint cd;
        bool f;

      public:
        enemy(cREAL &X, cREAL &Y, cREAL &sX, cREAL &sY, cuit &sTime, cint &Health = 1);

        bool ready() const;
        bool update() override final;
        void draw() const override final;
    };
} // namespace GAME_CORE