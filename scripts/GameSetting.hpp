#pragma once

#include "stdpre.hpp"

namespace Setting {
    constexpr char title[] = "Touhou Project Imperishable Night. (Fake) ver 0.9.5";

    constexpr uint WinWidth = 640, WinHeight = 480;
    constexpr uint halfWinWidth  = WinWidth >> 1,
                   halfWinHeight = WinHeight >> 1;

    constexpr uint MapWidth = 400, MapHeight = WinHeight;
    constexpr uint halfMapWidth  = MapWidth >> 1,
                   halfMapHeight = MapHeight >> 1;

    constexpr uint SpwanX = halfMapWidth,
                   SpwanY = MapHeight * 0.75;

    constexpr REAL MapSpeed       = 0.3;
    constexpr REAL ReimuSpeed     = 4.5;
    constexpr REAL SpellCardSpeed = 15;
    constexpr REAL RoundSpeed     = 1;

    namespace Key {
        constexpr byte AttackKey = 'Z';

    } // namespace Key

    namespace UI {
        constexpr uint SymbolX = halfWinWidth + halfMapWidth,
                       SymbolY = 340;

    } // namespace UI

    constexpr uint SpellCardFix = 7;

} // namespace Setting