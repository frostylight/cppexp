#pragma once

#include "pre.hpp"


namespace Setting {
    namespace UI {
        constexpr uint WinWidth = 640, WinHeight = 480;
        constexpr uint MapWidth = 400, MapHeight = WinHeight;

        constexpr uint halfWinWidth = WinWidth >> 1, halfWinHeight = WinHeight >> 1;
        constexpr uint halfMapWidth = MapWidth >> 1, halfMapHeight = halfMapHeight >> 1;
    } // namespace UI

    namespace EnemyE {
        constexpr uint BoxWidth  = 20;
        constexpr uint BoxHeight = 22;

        namespace RoundBullet {
            constexpr uint BoxWidth  = 16;
            constexpr uint BoxHeight = 16;
        } // namespace RoundBullet

        constexpr uint BulletCD = 60;
    } // namespace EnemyE

    namespace Reimu {
        constexpr uint BoxWidth  = 24;
        constexpr uint BoxHeight = 40;

        namespace SpellCard {
            constexpr uint BoxWidth  = 12;
            constexpr uint BoxHeight = 14;
        } // namespace SpellCard

        constexpr uint BulletCD = 6;
        constexpr REAL Speed    = 4.5;
    } // namespace Reimu

} // namespace Setting