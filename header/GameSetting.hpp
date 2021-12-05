#pragma once

#include "pre.hpp"


namespace Setting {
    constexpr char title[] = "Touhou Project Imperishable Night. (Fake) ver 0.9.5";

    constexpr uint FramePerSecond = 60;

    namespace Game {
        constexpr uint WinWidth = 640, WinHeight = 480;
        constexpr uint MapWidth = 400, MapHeight = WinHeight;

        constexpr uint halfWinWidth = WinWidth >> 1, halfWinHeight = WinHeight >> 1;
        constexpr uint halfMapWidth = MapWidth >> 1, halfMapHeight = halfMapHeight >> 1;

        constexpr uint SpawnX = halfMapWidth, SpawnY = MapHeight * 0.75;

        constexpr REAL MapSpeed = 0.3;
    } // namespace Game

    namespace UI {
        namespace TabBar {
            constexpr uint SymbolX = Game::halfMapWidth + Game::halfWinWidth;
            constexpr uint SymbolY = 340;
            constexpr uint titleX = 430, titleY = 405;
            constexpr uint DifficultyX = 550, DifficultyY = 200;
        } // namespace TabBar

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
            constexpr uint CenterFix = 7;
            constexpr REAL Speed     = 15;
        } // namespace SpellCard

        constexpr uint BulletCD = 6;
        constexpr REAL Speed    = 4.5;
    } // namespace Reimu

} // namespace Setting